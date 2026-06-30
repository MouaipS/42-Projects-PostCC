#include "ft_nm.h"

/*
** Version 32-bit de parse_letter_64.
** La logique est identique, seuls les types changent :
**   Elf64_Sym → Elf32_Sym
**   Elf64_Ehdr → Elf32_Ehdr
**   Elf64_Shdr → Elf32_Shdr
**   uint64_t flags → uint32_t flags  (les flags ELF32 tiennent sur 32 bits)
**   uint32_t type  → uint16_t type   (sh_type est uint32 en 64-bit, uint16 en 32-bit)
**
** Voir parse_letter_64 pour le détail de la logique.
*/
char parse_letter_32(t_data *data, Elf32_Sym *actual_symbol, int count)
{
	uint16_t   index_section;
	Elf32_Ehdr *elf_header;
	Elf32_Shdr *section;
	Elf32_Shdr *sh;
	uint32_t   flags;
	uint16_t   type;
	char       c;

	index_section = actual_symbol->st_shndx;
	elf_header    = data->map;
	section       = data->map + elf_header->e_shoff;

	/* Cas spéciaux d'index de section réservés par la spec ELF */
	if (index_section == SHN_UNDEF)
		c = 'U'; /* Référence externe non résolue */
	else if (index_section == SHN_ABS)
		c = 'A'; /* Adresse absolue */
	else if (index_section == SHN_COMMON)
		c = 'C'; /* Variable commune non allouée */
	else if (index_section >= data->header_struct->nb_sections)
		c = '?'; /* Index invalide */
	else
	{
		sh    = &section[index_section];
		flags = sh->sh_flags;
		type  = sh->sh_type;

		/* Priorité : BSS > TEXT > DATA > RODATA > non-alloué */
		if (type == SHT_NOBITS && (flags & SHF_ALLOC))
			c = 'B'; /* BSS : alloué en mémoire, pas de données dans le fichier */
		else if ((flags & SHF_EXECINSTR) && (flags & SHF_ALLOC))
			c = 'T'; /* Code exécutable */
		else if ((flags & SHF_WRITE) && (flags & SHF_ALLOC))
			c = 'D'; /* Données initialisées modifiables */
		else if (flags & SHF_ALLOC)
			c = 'R'; /* Lecture seule (constantes, chaînes littérales...) */
		else
			c = 'N'; /* Pas alloué en mémoire (sections de debug) */
	}

	/* WEAK : écrase la lettre et retourne immédiatement sans passer par tolower */
	if (data->sym_array[count].st_info == STB_WEAK)
	{
		c = (actual_symbol->st_shndx == SHN_UNDEF) ? 'w' : 'W';
		return (c);
	}

	/* LOCAL : même lettre qu'un symbole global mais en minuscule */
	if (data->sym_array[count].st_info == STB_LOCAL)
		c = (char)tolower((unsigned char)c);

	return (c);
}

/*
** Version 32-bit de symbols64.
** Parcourt les entrées Elf32_Sym de .symtab, filtre et remplit sym_array.
** Voir symbols64 pour le détail complet.
*/
void symbols32(t_data *data)
{
	Elf32_Sym  *actual_symbol;
	size_t     nb_symbol;
	size_t     i;
	size_t     count;
	int        stype;
	const char *name;

	nb_symbol = data->symtab_struct->size / data->symtab_struct->symbole_size;

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
		/* Cast en uint8_t* pour un déplacement octet par octet contrôlé */
		actual_symbol = (Elf32_Sym *)((uint8_t *)data->symtab_struct->symtab
		                              + i * data->symtab_struct->symbole_size);

		stype = ELF32_ST_TYPE(actual_symbol->st_info);

		/* STT_FILE et STT_SECTION ne sont pas affichés par nm sans -a */
		if (stype == STT_FILE || stype == STT_SECTION)
		{
			i++;
			continue;
		}

		/* Vérifier que l'offset du nom est dans les limites de .strtab */
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
		data->sym_array[count].st_info = ELF32_ST_BIND(actual_symbol->st_info);
		data->sym_array[count].letter  = parse_letter_32(data, actual_symbol, count);
		data->sym_array[count].value   = actual_symbol->st_value;
		data->sym_array[count].has_value =
		    (actual_symbol->st_shndx != SHN_UNDEF
		     && data->sym_array[count].st_info != STB_WEAK)
		    || actual_symbol->st_value != 0;

		count++;
		i++;
	}

	data->count = count;
	qsort(data->sym_array, count, sizeof(t_sym), compar_sym);
}

/*
** Point d'entrée pour un ELF 32-bit.
** Identique à process_64 mais avec les types Elf32_*.
** En 32-bit, le ELF Header fait 52 octets (contre 64 en 64-bit).
*/
void process_32(t_data *data)
{
	Elf32_Ehdr *elf_header;
	Elf32_Shdr *elf_section_header_table;
	Elf32_Shdr *shsrtab_header;

	elf_header                = data->map;
	elf_section_header_table  = data->map + elf_header->e_shoff;

	data->header_struct->nb_sections    = elf_header->e_shnum;
	data->header_struct->shstrtab_index = elf_header->e_shstrndx;

	shsrtab_header = &elf_section_header_table[data->header_struct->shstrtab_index];
	data->header_struct->offset_shstrtab = (char *)data->map + shsrtab_header->sh_offset;

	find_tabs_32(data, elf_section_header_table);
	parse_symbol(data);
	print_list(data);
}
