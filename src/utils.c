#include "ft_nm.h"

void print_list(t_data *data){
	const t_sym *tmpSymb;
	int width;
	int i = 0;
	char		buf[32];
	if(data->is_64_or_32 == true){
		width = 16;
		while(i < data->count){
			tmpSymb = &data->sym_array[i];
			if(!tmpSymb->has_value || tmpSymb->letter == 'U' || tmpSymb->letter == 'u' || tmpSymb->letter == 'w'){
				int j = 0;
				while(j < width){
					write(1, " ", 1);
					j++;
				}
			}
			else{
				snprintf(buf, sizeof(buf), "%016llx",
					(unsigned long long)tmpSymb->value);
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
}




int compar_sym(const void *a, const void* b){
    const t_sym *sa = a;
    const t_sym *sb = b;
    const char  *na = sa->name;
    const char  *nb = sb->name;

    // Saute les underscores
    while (*na == '_') na++;
    while (*nb == '_') nb++;

    int ret = strcasecmp(na, nb);
    if (ret != 0)
        return ret;
    // À égalité, strcmp normal pour ordre stable
    return strcmp(sa->name, sb->name);
}

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
				data->symtab_struct->strtab_size = header_string_table->sh_size;
			}
		}
		i++;
	}
}

void ft_error(const char *error){
	printf("Error : %s\n", error);
	exit(0);
}