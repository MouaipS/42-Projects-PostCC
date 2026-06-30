#include "ft_nm.h"

/*
** Affiche tous les symboles de sym_array dans le format nm :
**   <adresse_hex> <lettre> <nom>\n
**
** L'adresse est affichée sur 16 chiffres hex en 64-bit (ex: 0000000000401234)
** ou 8 chiffres en 32-bit (ex: 08048234).
** Si le symbole n'a pas d'adresse connue (has_value == 0), des espaces remplacent l'adresse.
*/
void print_list(t_data *data)
{
	const t_sym *tmpSymb;
	int          width;
	int          i;
	char         buf[32];

	/* La largeur de la colonne d'adresse dépend de la classe ELF :
	** 64-bit → 16 chiffres hex, 32-bit → 8 chiffres hex */
	width = (data->is_64_or_32 == true) ? 16 : 8;

	i = 0;
	while (i < data->count)
	{
		tmpSymb = &data->sym_array[i];

		/* Les symboles sans adresse connue affichent des espaces :
		**   'U' : symbole indéfini (référence externe)
		**   'u' : inutilisé en pratique mais géré par sécurité
		**   'w' : weak indéfini (référence faible non résolue) */
		if (!tmpSymb->has_value
		    || tmpSymb->letter == 'U'
		    || tmpSymb->letter == 'u'
		    || tmpSymb->letter == 'w')
		{
			int j = 0;
			while (j < width)
			{
				write(1, " ", 1);
				j++;
			}
		}
		else
		{
			/* Formater l'adresse en hexadécimal avec zéros en tête */
			if (data->is_64_or_32 == true)
				snprintf(buf, sizeof(buf), "%016llx", (unsigned long long)tmpSymb->value);
			else
				snprintf(buf, sizeof(buf), "%08lx", (unsigned long)tmpSymb->value);
			write(1, buf, width);
		}

		/* Format : " <lettre> <nom>\n" */
		write(1, " ", 1);
		write(1, &tmpSymb->letter, 1);
		write(1, " ", 1);
		write(1, tmpSymb->name, strlen(tmpSymb->name));
		write(1, "\n", 1);
		i++;
	}
}

/*
** Comparaison de deux noms de symboles pour le tri, en ignorant les underscores
** PARTOUT dans le nom (pas seulement en tête) et en ignorant la casse.
**
** Exemples de l'effet :
**   "print_list" → se compare comme "printlist"
**   "printf@GLIBC_2.2.5" → '@' reste, pas de underscore à sauter
**   Résultat : '@'(64) < 'l'(108) → "printf@..." < "printlist" → printf@ en premier
**
** C'est le comportement du vrai nm : sans ce skip global,
** '_'(95) < 'f'(102) mettrait "print_list" avant "printf@..." à tort.
*/
static int cmp_sym_name(const char *a, const char *b)
{
	while (*a || *b)
	{
		/* Sauter tous les underscores consécutifs dans les deux noms */
		while (*a == '_') a++;
		while (*b == '_') b++;

		/* Comparaison insensible à la casse.
		** Le cast (unsigned char) est indispensable : tolower attend [0, 255] ou EOF,
		** un char signé avec valeur > 127 donnerait un undefined behavior sans lui. */
		int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
		if (d != 0)
			return d;

		/* Avancer seulement si on n'est pas en fin de chaîne */
		if (*a) a++;
		if (*b) b++;
	}
	return 0;
}

/*
** Fonction de comparaison pour qsort(3).
** qsort passe des void* vers les éléments du tableau ; on les recaste en t_sym*.
**
** Ordre de tri :
**   1. cmp_sym_name : alphabétique, insensible à la casse, underscores ignorés
**   2. strcmp sur les noms originaux en cas d'égalité (tie-breaker déterministe)
*/
int compar_sym(const void *a, const void *b)
{
	const t_sym *sa;
	const t_sym *sb;
	int          ret;

	sa  = a;
	sb  = b;
	ret = cmp_sym_name(sa->name, sb->name);
	if (ret != 0)
		return ret;

	/* Deux noms identiques après filtrage (ex: "foo_bar" et "foobar") :
	** on tombe sur strcmp pour avoir un ordre stable et reproductible. */
	return strcmp(sa->name, sb->name);
}

/*
** Dispatcher : appelle la version 64 ou 32 bits de la construction du sym_array
** selon la classe ELF détectée lors de ft_check_file.
*/
void parse_symbol(t_data *data)
{
	if (data->is_64_or_32 == true)
		symbols64(data);
	else
		symbols32(data);
}

/*
** Parcourt la Section Header Table 64-bit pour localiser .symtab et .strtab.
**
** .symtab est identifiée par sh_type == SHT_SYMTAB.
** La .strtab associée est indiquée par sh_link de la .symtab :
** c'est l'index de la section des noms dans la Section Header Table.
** On ne suppose pas que c'est toujours la même section (un binaire peut en avoir plusieurs).
**
** Fallback sur sh_entsize : certains compilateurs mettent 0 alors que la spec
** demande sizeof(Elf64_Sym). Sans ce fallback, la division size/symbole_size
** serait une division par zéro.
*/
void find_tabs_64(t_data *data, Elf64_Shdr *section_header_table)
{
	int        i;
	Elf64_Shdr *tmpShdr;
	Elf64_Shdr *header_string_table;

	i = 0;
	while (i < data->header_struct->nb_sections)
	{
		tmpShdr = &section_header_table[i];

		if (tmpShdr->sh_type == SHT_SYMTAB)
		{
			/* Pointeur direct dans le fichier mappé : pas de copie des données */
			data->symtab_struct->symtab = data->map + tmpShdr->sh_offset;
			data->symtab_struct->size   = tmpShdr->sh_size;

			/* sh_entsize = taille d'une entrée Elf64_Sym.
			** Certains linkers le laissent à 0*/
			if (tmpShdr->sh_entsize == 0)
				data->symtab_struct->symbole_size = sizeof(Elf64_Sym);
			else
				data->symtab_struct->symbole_size = tmpShdr->sh_entsize;

			/* sh_link = index de la .strtab correspondante dans la section table */
			if (tmpShdr->sh_link < data->header_struct->nb_sections)
			{
				header_string_table          = &section_header_table[tmpShdr->sh_link];
				data->symtab_struct->strtab      = data->map + header_string_table->sh_offset;
				data->symtab_struct->strtab_size = header_string_table->sh_size;
			}
		}
		i++;
	}

	/* Si .symtab ou .strtab n'ont pas été trouvées, le binaire est strippé
	** ou n'a pas de table de symboles (ex: /bin/ls après strip). */
	if (!data->symtab_struct->symtab || !data->symtab_struct->strtab)
	{
		free(data->header_struct);
		free(data->symtab_struct);
		ft_error(data->filename, "no symbols");
	}
}

/*
** Version 32-bit de find_tabs_64.
** Logique identique avec les types Elf32_Shdr et sizeof(Elf32_Sym).
*/
void find_tabs_32(t_data *data, Elf32_Shdr *section_header_table)
{
	int        i;
	Elf32_Shdr *tmpShdr;
	Elf32_Shdr *header_string_table;

	i = 0;
	while (i < data->header_struct->nb_sections)
	{
		tmpShdr = &section_header_table[i];

		if (tmpShdr->sh_type == SHT_SYMTAB)
		{
			data->symtab_struct->symtab = data->map + tmpShdr->sh_offset;
			data->symtab_struct->size   = tmpShdr->sh_size;

			if (tmpShdr->sh_entsize == 0)
				data->symtab_struct->symbole_size = sizeof(Elf32_Sym);
			else
				data->symtab_struct->symbole_size = tmpShdr->sh_entsize;

			if (tmpShdr->sh_link < data->header_struct->nb_sections)
			{
				header_string_table              = &section_header_table[tmpShdr->sh_link];
				data->symtab_struct->strtab      = data->map + header_string_table->sh_offset;
				data->symtab_struct->strtab_size = header_string_table->sh_size;
			}
		}
		i++;
	}

	if (!data->symtab_struct->symtab || !data->symtab_struct->strtab)
	{
		free(data->header_struct);
		free(data->symtab_struct);
		ft_error(data->filename, "no symbols");
	}
}

/*
** Affiche un message d'erreur sur stderr au format nm (ft_nm: fichier: message)
** puis termine le programme. L'affichage sur stderr (et non stdout) est important :
** les erreurs ne doivent pas polluer la sortie normale qui peut être redirigée.
*/
void ft_error(const char *filename, const char *error)
{
	fprintf(stderr, "ft_nm: %s: %s\n", filename, error);
	exit(1);
}
