#include "ft_nm.h"

void parse_symbol(t_data *data){
	if(data->is_64_or_32 == true)
		symbols64(data);
	else
		symbols32(data);
}

void find_tabs(t_data *data, Elf64_Shdr *section_header_table){
	int i = 0;
	Elf64_Shdr *tmpShdr;
	Elf64_Shdr *header_string_table;
	while(i < data->header_struct->nb_sections){
		tmpShdr = &section_header_table[i];
		if(tmpShdr->sh_type == SHT_SYMTAB){
			data->symtab_struct->symtab = data->map + tmpShdr->sh_offset;
			data->symtab_struct->symbole_size = tmpShdr->sh_entsize;
			data->symtab_struct->size = tmpShdr->sh_size;
			if(tmpShdr->sh_entsize == 0) //taille du fichier erronee
				data->symtab_struct->symbole_size = sizeof(Elf64_Shdr);
			data->symtab_struct->symbole_size = tmpShdr->sh_entsize;
			if(tmpShdr->sh_link < data->header_struct->nb_sections){
				header_string_table = &section_header_table[tmpShdr->sh_link];
				data->symtab_struct->strtab = data->map + header_string_table->sh_offset;
				data->symtab_struct->strtab_size = tmpShdr->sh_size;
			}
		}
		i++;
	}
}

void ft_error(const char *error){
	printf("Error : %s\n", error);
	exit(0);
}