#include "ft_nm.h"

/*
** Version bonus de print_list.
** Ajoute deux filtres avant l'affichage selon les flags actifs :
**   -g (flags.g) : n'afficher que les symboles globaux
**   -u (flags.u) : n'afficher que les symboles indéfinis
**
** Ces filtres s'appliquent au moment de l'affichage et non au moment
** de la construction de sym_array, afin de rester compatible avec -p
** (pas de tri) qui doit conserver l'ordre original du fichier.
*/
void print_list(t_data *data)
{
	const t_sym *tmpSymb;
	int          width;
	int          i;
	char         buf[32];

	width = (data->is_64_or_32 == true) ? 16 : 8;

	i = 0;
	while (i < data->count)
	{
		tmpSymb = &data->sym_array[i];

		/* -g / --extern-only : masquer les symboles locaux (lettres minuscules)
		** sauf U, W, w qui sont globaux par nature malgré leur casse particulière */
		if (data->flags.g)
		{
			char l = tmpSymb->letter;
			if (l != 'U' && l != 'W' && l != 'w' && islower((unsigned char)l))
			{
				i++;
				continue;
			}
		}

		/* -u / --undefined-only : n'afficher que U (indéfini) et w (weak indéfini) */
		if (data->flags.u)
		{
			if (tmpSymb->letter != 'U' && tmpSymb->letter != 'w')
			{
				i++;
				continue;
			}
		}

		/* Affichage de l'adresse ou des espaces selon has_value */
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
			if (data->is_64_or_32 == true)
				snprintf(buf, sizeof(buf), "%016llx", (unsigned long long)tmpSymb->value);
			else
				snprintf(buf, sizeof(buf), "%08lx", (unsigned long)tmpSymb->value);
			write(1, buf, width);
		}

		write(1, " ", 1);
		write(1, &tmpSymb->letter, 1);
		write(1, " ", 1);
		write(1, tmpSymb->name, strlen(tmpSymb->name));
		write(1, "\n", 1);
		i++;
	}
}

/*
** Comparaison de noms insensible à la casse, underscores ignorés partout.
** Identique à la version mandatory. Voir src/utils.c pour le détail.
*/
static int cmp_sym_name(const char *a, const char *b)
{
	while (*a || *b)
	{
		while (*a == '_') a++;
		while (*b == '_') b++;
		int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
		if (d != 0)
			return d;
		if (*a) a++;
		if (*b) b++;
	}
	return 0;
}

/* Comparaison pour qsort : ordre alphabétique normal. */
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
	return strcmp(sa->name, sb->name);
}

/*
** Comparaison inversée pour -r / --reverse-sort.
** Inverser a et b dans l'appel à compar_sym suffit à inverser le tri
** sans dupliquer la logique de comparaison.
*/
int revers_compar_syn(const void *a, const void *b)
{
	return (compar_sym(b, a));
}

/* Dispatcher 32/64 bits vers la bonne version de symbols*. */
void parse_symbol(t_data *data)
{
	if (data->is_64_or_32 == true)
		symbols64(data);
	else
		symbols32(data);
}

/*
** Version bonus de find_tabs_64.
** Identique à la version mandatory. Voir src/utils.c pour le détail.
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
			data->symtab_struct->symtab = data->map + tmpShdr->sh_offset;
			data->symtab_struct->size   = tmpShdr->sh_size;

			/* Fallback si sh_entsize vaut 0 (comportement non standard de certains linkers) */
			if (tmpShdr->sh_entsize == 0)
				data->symtab_struct->symbole_size = sizeof(Elf64_Sym);
			else
				data->symtab_struct->symbole_size = tmpShdr->sh_entsize;

			/* sh_link = index de la .strtab associée dans la Section Header Table */
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
** Version bonus de find_tabs_32. Identique à la version mandatory.
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

/* Gestion d'erreur : stderr + format nm + exit. */
void ft_error(const char *filename, const char *error)
{
	fprintf(stderr, "ft_nm: %s: %s\n", filename, error);
	exit(1);
}
