#include "ft_nm.h"

void print_list(t_data *data){
	const t_sym *tmpSymb;
	int width;
	int i = 0;
	char		buf[32];
	if(data->is_64_or_32 == true)
		width = 16;
	else
		width = 8;
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
			if(data->is_64_or_32 == true)
				snprintf(buf, sizeof(buf), "%016llx", (unsigned long long)tmpSymb->value);
			else
				snprintf(buf, sizeof(buf), "%08lx",	(unsigned long)tmpSymb->value);
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

static int cmp_sym_name(const char *a, const char *b)
{
    while (*a || *b) {
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

int compar_sym(const void *a, const void* b){
    const t_sym *sa = a;
    const t_sym *sb = b;
    int ret = cmp_sym_name(sa->name, sb->name);
    if (ret != 0)
        return ret;
    return strcmp(sa->name, sb->name);
}

void parse_symbol(t_data *data){
	if(data->is_64_or_32 == true)
		symbols64(data);
	else
		symbols32(data);
}

void find_tabs_64(t_data *data, Elf64_Shdr *section_header_table){
	int i = 0;
	Elf64_Shdr *tmpShdr;
	Elf64_Shdr *header_string_table;
	while(i < data->header_struct->nb_sections){
		tmpShdr = &section_header_table[i];
		if(tmpShdr->sh_type == SHT_SYMTAB){
			data->symtab_struct->symtab = data->map + tmpShdr->sh_offset;
			data->symtab_struct->size = tmpShdr->sh_size;
			if(tmpShdr->sh_entsize == 0)
                data->symtab_struct->symbole_size = sizeof(Elf64_Sym); // sizeof Sym, pas Shdr
            else
                data->symtab_struct->symbole_size = tmpShdr->sh_entsize;
			if(tmpShdr->sh_link < data->header_struct->nb_sections){
				header_string_table = &section_header_table[tmpShdr->sh_link];
				data->symtab_struct->strtab = data->map + header_string_table->sh_offset;
				data->symtab_struct->strtab_size = header_string_table->sh_size;
			}
		}
		i++;
	}
	if (!data->symtab_struct->symtab || !data->symtab_struct->strtab){
        free(data->header_struct);
	    free(data->symtab_struct);
        ft_error(data->filename, "no symbols");
	}
}


void find_tabs_32(t_data *data, Elf32_Shdr *section_header_table){
	int i = 0;
	Elf32_Shdr *tmpShdr;
	Elf32_Shdr *header_string_table;
	while(i < data->header_struct->nb_sections){
		tmpShdr = &section_header_table[i];
		if(tmpShdr->sh_type == SHT_SYMTAB){
			data->symtab_struct->symtab = data->map + tmpShdr->sh_offset;
			data->symtab_struct->size = tmpShdr->sh_size;
			if(tmpShdr->sh_entsize == 0)
                data->symtab_struct->symbole_size = sizeof(Elf32_Sym); // sizeof Sym, pas Shdr
            else
                data->symtab_struct->symbole_size = tmpShdr->sh_entsize;
			if(tmpShdr->sh_link < data->header_struct->nb_sections){
				header_string_table = &section_header_table[tmpShdr->sh_link];
				data->symtab_struct->strtab = data->map + header_string_table->sh_offset;
				data->symtab_struct->strtab_size = header_string_table->sh_size;
			}
		}
		i++;
	}
	if (!data->symtab_struct->symtab || !data->symtab_struct->strtab){
		free(data->header_struct);
	    free(data->symtab_struct);
        ft_error(data->filename, "no symbols");
	}
}

void ft_error(const char *filename, const char *error){
	fprintf(stderr, "ft_nm: %s: %s\n", filename, error);
	exit(1);
}