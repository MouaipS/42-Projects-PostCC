#include "ft_nm.h"

/*
** Version bonus de parse_letter_64.
** Logique identique à la version mandatory.
** Voir src/process_64.c pour le détail complet.
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
	elf_header    = data->map;
	section       = data->map + elf_header->e_shoff;

	if (index_section == SHN_UNDEF)
		c = 'U';
	else if (index_section == SHN_ABS)
		c = 'A';
	else if (index_section == SHN_COMMON)
		c = 'C';
	else if (index_section >= data->header_struct->nb_sections)
		c = '?';
	else
	{
		sh    = &section[index_section];
		flags = sh->sh_flags;
		type  = sh->sh_type;

		if (type == SHT_NOBITS && (flags & SHF_ALLOC))
			c = 'B';
		else if ((flags & SHF_EXECINSTR) && (flags & SHF_ALLOC))
			c = 'T';
		else if ((flags & SHF_WRITE) && (flags & SHF_ALLOC))
			c = 'D';
		else if (flags & SHF_ALLOC)
			c = 'R';
		else
			c = 'N';
	}

	if (data->sym_array[count].st_info == STB_WEAK)
	{
		c = (actual_symbol->st_shndx == SHN_UNDEF) ? 'w' : 'W';
		return (c);
	}
	if (data->sym_array[count].st_info == STB_LOCAL)
		c = (char)tolower((unsigned char)c);
	return (c);
}

/*
** Version bonus de symbols64.
** Différences par rapport à la version mandatory :
**
**   1. Flag -a (flags.a) :
**      Les STT_FILE et STT_SECTION ne sont plus filtrés inconditionnellement.
**      Avec -a, ils sont inclus dans sym_array avec la lettre 'a' pour STT_FILE.
**      Sans -a, le comportement est identique à la version mandatory.
**
**   2. Tri conditionnel :
**      - flags.p (-p / --no-sort) : aucun tri, on conserve l'ordre du fichier
**      - flags.r (-r / --reverse-sort) : tri inversé via revers_compar_syn
**      - sinon : tri alphabétique normal via compar_sym
*/
void symbols64(t_data *data)
{
	Elf64_Sym  *actual_symbol;
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
		actual_symbol = (Elf64_Sym *)((uint8_t *)data->symtab_struct->symtab
		                              + i * data->symtab_struct->symbole_size);
		stype = ELF64_ST_TYPE(actual_symbol->st_info);

		/* Sans -a : STT_FILE et STT_SECTION sont filtrés comme en mandatory */
		if (stype == STT_FILE || stype == STT_SECTION)
		{
			if (!data->flags.a)
			{
				i++;
				continue;
			}
		}

		if (actual_symbol->st_name >= data->symtab_struct->strtab_size)
		{
			i++;
			continue;
		}

		name = data->symtab_struct->strtab + actual_symbol->st_name;

		/* Les symboles sans nom ne sont gardés qu'en -a ET seulement si STT_FILE */
		if (name[0] == '\0' && (stype != STT_FILE || !data->flags.a))
		{
			i++;
			continue;
		}

		data->sym_array[count].name = name;

		/* STT_FILE avec -a : binding forcé à LOCAL, lettre 'a' (minuscule de 'A') */
		if (stype == STT_FILE)
			data->sym_array[count].st_info = STB_LOCAL;
		else
			data->sym_array[count].st_info = ELF64_ST_BIND(actual_symbol->st_info);

		data->sym_array[count].letter = (stype == STT_FILE)
		                                ? 'a'
		                                : parse_letter_64(data, actual_symbol, count);
		data->sym_array[count].value     = actual_symbol->st_value;
		data->sym_array[count].has_value =
		    (actual_symbol->st_shndx != SHN_UNDEF
		     && data->sym_array[count].st_info != STB_WEAK)
		    || actual_symbol->st_value != 0;
		count++;
		i++;
	}

	data->count = count;

	/* Tri conditionnel selon les flags :
	**   -p → pas de tri (conserver l'ordre .symtab)
	**   -r → tri inversé
	**   sinon → tri alphabétique */
	if (!data->flags.p)
	{
		if (data->flags.r)
			qsort(data->sym_array, count, sizeof(t_sym), revers_compar_syn);
		else
			qsort(data->sym_array, count, sizeof(t_sym), compar_sym);
	}
}

/*
** Version bonus de process_64.
** Identique à la version mandatory : les différences de comportement
** (flags -a, -r, -p) sont gérées dans symbols64 et print_list.
*/
void process_64(t_data *data)
{
	Elf64_Ehdr *elf_header;
	Elf64_Shdr *elf_section_header_table;
	Elf64_Shdr *shsrtab_header;

	elf_header               = data->map;
	elf_section_header_table = data->map + elf_header->e_shoff;

	data->header_struct->nb_sections    = elf_header->e_shnum;
	data->header_struct->shstrtab_index = elf_header->e_shstrndx;

	shsrtab_header = &elf_section_header_table[data->header_struct->shstrtab_index];
	data->header_struct->offset_shstrtab = (char *)data->map + shsrtab_header->sh_offset;

	find_tabs_64(data, elf_section_header_table);
	parse_symbol(data);
	print_list(data);
}
