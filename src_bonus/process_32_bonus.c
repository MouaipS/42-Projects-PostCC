#include "ft_nm.h"

/*
** Version bonus de parse_letter_32.
** Logique identique à la version mandatory.
** Voir src/process_32.c et src/process_64.c pour le détail.
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
** Version bonus de symbols32.
** Différences par rapport à la version mandatory :
**
**   1. Flag -a (flags.a) :
**      Avec -a, les STT_FILE et STT_SECTION sont conservés.
**      En 32-bit, le filtrage de STT_FILE/SECTION est dans le premier if,
**      contrairement à la version 64-bit qui les filtre toujours d'abord
**      puis les réintroduit. L'effet final est identique.
**
**   2. Tri conditionnel : -p, -r ou tri normal (voir symbols64 pour le détail).
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
		actual_symbol = (Elf32_Sym *)((uint8_t *)data->symtab_struct->symtab
		                              + i * data->symtab_struct->symbole_size);
		stype = ELF32_ST_TYPE(actual_symbol->st_info);

		/* Sans -a : ignorer STT_FILE et STT_SECTION (méta-symboles du linker) */
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

		if (name[0] == '\0' && (stype != STT_FILE || !data->flags.a))
		{
			i++;
			continue;
		}

		data->sym_array[count].name = name;

		/* STT_FILE avec -a : binding forcé à LOCAL, lettre 'a' */
		if (stype == STT_FILE)
			data->sym_array[count].st_info = STB_LOCAL;
		else
			data->sym_array[count].st_info = ELF32_ST_BIND(actual_symbol->st_info);

		data->sym_array[count].letter = (stype == STT_FILE)
		                                ? 'a'
		                                : parse_letter_32(data, actual_symbol, count);
		data->sym_array[count].value     = actual_symbol->st_value;
		data->sym_array[count].has_value =
		    (actual_symbol->st_shndx != SHN_UNDEF
		     && data->sym_array[count].st_info != STB_WEAK)
		    || actual_symbol->st_value != 0;
		count++;
		i++;
	}

	data->count = count;

	if (!data->flags.p)
	{
		if (data->flags.r)
			qsort(data->sym_array, count, sizeof(t_sym), revers_compar_syn);
		else
			qsort(data->sym_array, count, sizeof(t_sym), compar_sym);
	}
}

/*
** Version bonus de process_32.
** Identique à la version mandatory.
*/
void process_32(t_data *data)
{
	Elf32_Ehdr *elf_header;
	Elf32_Shdr *elf_section_header_table;
	Elf32_Shdr *shsrtab_header;

	elf_header               = data->map;
	elf_section_header_table = data->map + elf_header->e_shoff;

	data->header_struct->nb_sections    = elf_header->e_shnum;
	data->header_struct->shstrtab_index = elf_header->e_shstrndx;

	shsrtab_header = &elf_section_header_table[data->header_struct->shstrtab_index];
	data->header_struct->offset_shstrtab = (char *)data->map + shsrtab_header->sh_offset;

	find_tabs_32(data, elf_section_header_table);
	parse_symbol(data);
	print_list(data);
}
