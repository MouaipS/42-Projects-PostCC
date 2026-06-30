#include "ft_nm.h"

char parse_letter_64(t_data *data, Elf64_Sym *actual_symbol, int count){
	uint16_t index_section = actual_symbol->st_shndx;
	Elf64_Ehdr *elf_header = data->map;
	Elf64_Shdr *section = data->map + elf_header->e_shoff;
	Elf64_Shdr *sh;
	uint64_t	flags;
	uint32_t	type;
	char c;
	if(index_section == SHN_UNDEF)
		c = 'U';
	else if(index_section == SHN_ABS)
		c = 'A';
	else if(index_section == SHN_COMMON)
		c = 'C';
	else if(index_section >= data->header_struct->nb_sections)
		c = '?';
	else {

		sh = &section[index_section];
		flags = sh->sh_flags;
		type = sh->sh_type;
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
	if (data->sym_array[count].st_info == STB_WEAK){
		if(actual_symbol->st_shndx == SHN_UNDEF)
			c = 'w';
		else
			c = 'W';
		return (c);
	}
	if(data->sym_array[count].st_info == STB_LOCAL)
		c = (char)tolower((unsigned char)c);
	return (c);
}

void symbols64(t_data *data){
	Elf64_Sym *actual_symbol;
	size_t nb_symbol = data->symtab_struct->size / data->symtab_struct->symbole_size;
	data->sym_array = malloc(sizeof(t_sym) * nb_symbol);
	if(!data->sym_array){
		free(data->header_struct);
		free(data->symtab_struct);
		munmap(data->map, data->buff.st_size);
		ft_error(data->filename, "memory exhausted");
	}
	size_t i = 0;
	size_t count;
	count = 0;
	while(i < nb_symbol){
		actual_symbol = (Elf64_Sym *)((uint8_t *)data->symtab_struct->symtab + i * data->symtab_struct->symbole_size);
		int stype = ELF64_ST_TYPE(actual_symbol->st_info);
		if (stype == STT_FILE || stype == STT_SECTION){
			i++;
			continue ;
		}
		if (actual_symbol->st_name >= data->symtab_struct->strtab_size){
			i++;
			continue ;
		}
		const char *name = data->symtab_struct->strtab + actual_symbol->st_name;
		if (name[0] == '\0') {
			i++;
			continue;
		}
		data->sym_array[count].name = name;
		data->sym_array[count].st_info = ELF64_ST_BIND(actual_symbol->st_info);
		data->sym_array[count].letter = parse_letter_64(data, actual_symbol, count);
		data->sym_array[count].value = actual_symbol->st_value;
		data->sym_array[count].has_value = (actual_symbol->st_shndx != SHN_UNDEF
			&& data->sym_array[count].st_info != STB_WEAK) || actual_symbol->st_value != 0;
		count++;
		i++;
	}
	data->count = count;
	qsort(data->sym_array, count, sizeof(t_sym), compar_sym);
}

void process_64(t_data *data){
	Elf64_Ehdr *elf_header = data->map; //Definie les 64 premiers octets comme le header;
	Elf64_Shdr *elf_section_header_table = data->map + elf_header->e_shoff; //Debut de la section header table
	
	data->header_struct->nb_sections = elf_header->e_shnum; //nombre de sections  //count of section headers
	data->header_struct->shstrtab_index = elf_header->e_shstrndx; //index of the names'section in the table //dans Section header table
	Elf64_Shdr *shsrtab_header = &elf_section_header_table[data->header_struct->shstrtab_index]; //header de la section "annuaire des noms des symboles"
	data->header_struct->offset_shstrtab = (char *)data->map + shsrtab_header->sh_offset; //On part de map + offset de shsrtab pour acceder aux données
	find_tabs_64(data, elf_section_header_table);
	parse_symbol(data);
	print_list(data);
}