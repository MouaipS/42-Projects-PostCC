#include "ft_nm.h"

static void process_nm(const char *filename){
	t_data data;
	data.header_struct = malloc(sizeof(t_elf));
	if(!data.header_struct){
		ft_error("Malloc failed.");
	}
    data.symtab_struct = malloc(sizeof(t_symtab));
	if(!data.symtab_struct){
		free(data.header_struct);
		ft_error("Malloc failed.");
	}

	ft_setup(&data, filename);
	ft_check_file(&data, filename);
	if(data.is_64_or_32 == true)
		process_64(&data);
	else
		process_32(&data);
	free(data.header_struct);
	free(data.symtab_struct);
	free(data.sym_array);
	munmap(data.map, data.buff.st_size);
}

int main(int ac, char **av){
	if(ac < 2){
		process_nm("a.out");
	} else{
		for(int i = 1; i < ac; i++){
			process_nm(av[i]);
		}
	}
	return(0);
}