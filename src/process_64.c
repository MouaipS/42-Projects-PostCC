#include "ft_nm.h"

void symbols64(t_data *data){
	Elf64_Sym *actual_symbol;
	size_t nb_symbol = data->symtab_struct->size / data->symtab_struct->symbole_size;
	data->sym_array = malloc(sizeof(t_sym) * nb_symbol);
	size_t i = 0;
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
		printf("%s\n", name);
		i++;
	}
}

void process_64(t_data *data){
	Elf64_Ehdr *elf_header = data->map; //Definie les 64 premiers octets comme le header;
	Elf64_Shdr *elf_section_header_table = data->map + elf_header->e_shoff; //Debut de la section header table
	
	data->header_struct->nb_sections = elf_header->e_shnum; //nombre de sections  //count of section headers
	data->header_struct->shstrtab_index = elf_header->e_shstrndx; //index of the names'section in the table //dans Section header table
	Elf64_Shdr *shsrtab_header = &elf_section_header_table[data->header_struct->shstrtab_index]; //header de la section "annuaire des noms des symboles"
	data->header_struct->offset_shstrtab = (char *)data->map + shsrtab_header->sh_offset; //On part de map + offset de shsrtab pour acceder aux données
	find_tabs(data, elf_section_header_table);
	parse_symbol(data);
}