#include "ft_nm.h"

static int parse_flags(t_data *data,int ac, char **av){
	int i = 1;
	while(i < ac && av[i][0] == '-' && av[i][1] != '\0'){
		const char *c = av[i] +1;
		if(*c == '-'){
			if(strcmp(av[i], "--extern-only") == 0)
				data->flags.g = true;
			else if(strcmp(av[i], "--undefined-only") == 0)
				data->flags.u = true;
			else if(strcmp(av[i], "--reverse-sort") == 0)
				data->flags.r = true;
			else if(strcmp(av[i], "--no-sort") == 0)
				data->flags.p = true;
			else if(strcmp(av[i], "--debug-syms") == 0)
				data->flags.a = true;
			else{
				printf("ft_nm: unknown option %s\n", av[i]);
				exit(1);
			}
			i++;
			continue;			
		}
		while(*c){
			if(*c == 'g')
				data->flags.g = true;
			else if(*c == 'u')
				data->flags.u = true;
			else if(*c == 'r')
				data->flags.r = true;
			else if(*c == 'p')
				data->flags.p = true;
			else if(*c == 'a')
				data->flags.a = true;
			else{
				printf("ft_nm: unknown option -%c\n", *c);
				exit(1);
			}
			c++;
		}
		i++;
	}
	return i;
}

static void process_nm(const char *filename,t_data *data){
	data->filename = filename;
	data->header_struct = malloc(sizeof(t_elf));
	if(!data->header_struct){
		ft_error(filename, "memory exhausted");
	}
    data->symtab_struct = malloc(sizeof(t_symtab));
	if(!data->symtab_struct){
		free(data->header_struct);
		ft_error(filename, "memory exhausted");
	}

	ft_setup_bonus(data, filename);
	ft_check_file_bonus(data, filename);
	if(data->is_64_or_32 == true)
		process_64(data);
	else
		process_32(data);
	free(data->header_struct);
	free(data->symtab_struct);
	free(data->sym_array);
	munmap(data->map, data->buff.st_size);
}

int main(int ac, char **av){
	t_data data;
	int files_list;
	files_list = parse_flags(&data,ac , av);
	if(files_list < ac){
		process_nm("a.out", &data);
	} else{
		for(; files_list < ac; files_list++){
			process_nm(av[files_list], &data);
		}
	}
	return(0);
}