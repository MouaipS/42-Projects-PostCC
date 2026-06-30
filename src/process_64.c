#include "ft_nm.h"

/*
** Détermine la lettre nm d'un symbole 64-bit à partir de sa section.
**
** La logique suit deux étapes :
**   1. Déterminer la lettre de base depuis st_shndx et les flags de la section
**   2. Ajuster selon le binding : WEAK écrase tout, LOCAL met en minuscule
**
** Paramètres :
**   data          : état global (accès au fichier mappé et à nb_sections)
**   actual_symbol : entrée Elf64_Sym en cours d'analyse
**   count         : index dans sym_array (déjà rempli jusqu'à st_info)
*/
char parse_letter_64(t_data *data, Elf64_Sym *actual_symbol, int count)
{
	uint16_t   index_section;
	Elf64_Ehdr *elf_header;
	Elf64_Shdr *section;
	Elf64_Shdr *sh;
	uint64_t   flags;
	uint32_t   type;
	char       c;

	index_section = actual_symbol->st_shndx;

	/* Relire le header ELF et la Section Header Table depuis le fichier mappé
	** pour accéder aux attributs de la section du symbole. */
	elf_header = data->map;
	section    = data->map + elf_header->e_shoff;

	/* Cas spéciaux : valeurs réservées de st_shndx qui ne correspondent
	** pas à une vraie section dans la Section Header Table. */
	if (index_section == SHN_UNDEF)
		c = 'U'; /* Symbole indéfini : référence externe non encore résolue */
	else if (index_section == SHN_ABS)
		c = 'A'; /* Adresse absolue : ne change pas lors du link (ex: constantes) */
	else if (index_section == SHN_COMMON)
		c = 'C'; /* Variable commune non allouée : sera résolue au link */
	else if (index_section >= data->header_struct->nb_sections)
		c = '?'; /* Index hors limites : fichier corrompu ou non standard */
	else
	{
		/* Cas normal : lire les flags de la section pour déduire la lettre.
		** L'ordre des if/else if est important : les flags peuvent se combiner,
		** et certains critères sont plus spécifiques que d'autres. */
		sh    = &section[index_section];
		flags = sh->sh_flags;
		type  = sh->sh_type;

		if (type == SHT_NOBITS && (flags & SHF_ALLOC))
			c = 'B'; /* BSS : section allouée en RAM mais absente du fichier (non initialisée) */
		else if ((flags & SHF_EXECINSTR) && (flags & SHF_ALLOC))
			c = 'T'; /* Text : code machine exécutable */
		else if ((flags & SHF_WRITE) && (flags & SHF_ALLOC))
			c = 'D'; /* Data : données initialisées modifiables en mémoire */
		else if (flags & SHF_ALLOC)
			c = 'R'; /* Rodata : données en lecture seule (constantes, vtables...) */
		else
			c = 'N'; /* Non alloué : section de debug, pas visible à l'exécution */
	}

	/* WEAK : le binding faible écrase la lettre de section et retourne immédiatement.
	** On ne passe PAS par le tolower ci-dessous, car 'W'/'w' ont leur propre casse. */
	if (data->sym_array[count].st_info == STB_WEAK)
	{
		/* Weak indéfini (w) vs weak défini (W) : même logique que U vs T */
		c = (actual_symbol->st_shndx == SHN_UNDEF) ? 'w' : 'W';
		return (c);
	}

	/* LOCAL : un symbole local a la même lettre que son équivalent global,
	** mais en minuscule. Ex: T (global) → t (local). */
	if (data->sym_array[count].st_info == STB_LOCAL)
		c = (char)tolower((unsigned char)c);
	/* Le cast (unsigned char) est indispensable avant tolower :
	** un char signé avec valeur > 127 serait un undefined behavior sans lui. */

	return (c);
}

/*
** Parcourt toutes les entrées Elf64_Sym de .symtab, filtre celles inutiles,
** et remplit data->sym_array avec les symboles à afficher.
** Trie le tableau avec qsort à la fin.
*/
void symbols64(t_data *data)
{
	Elf64_Sym *actual_symbol;
	size_t     nb_symbol;
	size_t     i;
	size_t     count;
	int        stype;
	const char *name;

	/* Nombre total d'entrées dans .symtab = taille totale / taille d'une entrée */
	nb_symbol = data->symtab_struct->size / data->symtab_struct->symbole_size;

	/* Allouer le tableau au maximum possible ; count sera inférieur car on filtre */
	data->sym_array = malloc(sizeof(t_sym) * nb_symbol);
	if (!data->sym_array)
	{
		free(data->header_struct);
		free(data->symtab_struct);
		munmap(data->map, data->buff.st_size);
		ft_error(data->filename, "memory exhausted");
	}

	i     = 0;
	count = 0;
	while (i < nb_symbol)
	{
		/* Cast en uint8_t* pour avancer EXACTEMENT de symbole_size octets.*/
		actual_symbol = (Elf64_Sym *)((uint8_t *)data->symtab_struct->symtab
		                              + i * data->symtab_struct->symbole_size);

		/* ELF64_ST_TYPE extrait les 4 bits de poids faible de st_info */
		stype = ELF64_ST_TYPE(actual_symbol->st_info);

		if (stype == STT_FILE || stype == STT_SECTION)
		{
			i++;
			continue;
		}

		if (actual_symbol->st_name >= data->symtab_struct->strtab_size)
		{
			i++;
			continue;
		}

		name = data->symtab_struct->strtab + actual_symbol->st_name;

		if (name[0] == '\0')
		{
			i++;
			continue;
		}

		data->sym_array[count].name    = name;
		data->sym_array[count].st_info = ELF64_ST_BIND(actual_symbol->st_info);
		data->sym_array[count].letter  = parse_letter_64(data, actual_symbol, count);
		data->sym_array[count].value   = actual_symbol->st_value;

		/* has_value : faut-il afficher l'adresse ou des espaces ?
		** Un symbole indéfini (SHN_UNDEF) ou weak indéfini n'a pas d'adresse connue,
		** SAUF si st_value est non nul (cas rare mais valide selon la spec). */
		data->sym_array[count].has_value = (actual_symbol->st_shndx != SHN_UNDEF && data->sym_array[count].st_info != STB_WEAK) || actual_symbol->st_value != 0;
			count++;
		i++;
	}

	data->count = count;

	qsort(data->sym_array, count, sizeof(t_sym), compar_sym);
}

/*
** Point d'entrée pour le traitement d'un ELF 64-bit.
** Lit le ELF Header pour localiser la Section Header Table et la .shstrtab,
** puis enchaîne les étapes de traitement.
*/
void process_64(t_data *data)
{
	Elf64_Ehdr *elf_header;
	Elf64_Shdr *elf_section_header_table;
	Elf64_Shdr *shsrtab_header;

	elf_header = data->map;

	/* La Section Header Table commence à l'offset e_shoff dans le fichier */
	elf_section_header_table = data->map + elf_header->e_shoff;

	/* Sauvegarder les informations du header dont on a besoin dans la suite */
	data->header_struct->nb_sections    = elf_header->e_shnum;
	data->header_struct->shstrtab_index = elf_header->e_shstrndx;

	/* .shstrtab est la section qui stocke les NOMS des sections (".text", ".data"...).
	** On la repère via e_shstrndx, puis on pointe sur ses données dans le fichier mappé. */
	shsrtab_header = &elf_section_header_table[data->header_struct->shstrtab_index];
	data->header_struct->offset_shstrtab = (char *)data->map + shsrtab_header->sh_offset;

	find_tabs_64(data, elf_section_header_table); /* localise .symtab et .strtab */
	parse_symbol(data);                           /* construit sym_array trié */
	print_list(data);                             /* affiche les symboles */
}
