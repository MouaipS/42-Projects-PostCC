#include "ft_nm.h"

static void process_nm(const char *filename){
	t_data data;
	data.header_struct = malloc(sizeof(t_elf));
    data.symtab_struct = malloc(sizeof(t_symtab));

	ft_setup(&data, filename);
	ft_check_file(&data, filename);
	if(data.is_64_or_32 == true)
		process_64(&data);
	else
		process_32(&data);
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