#include "../includes/ft_ls.h"


int parse_arg(char *arg, t_flags *flags){
	int i;

	i = 1;
	while(arg[i]){
		if (arg[i] == 'l')
			flags->l = true;
		else if (arg[i] == 'R')
			flags->R = true;
		else if (arg[i] == 'a')
			flags->a = true;
		else if (arg[i] == 'r')
			flags->r = true;
		else if (arg[i] == 't')
			flags->t = true;
		else {
			//TODO error message with illegal option
			return(-1);
		}
		i++;
	}
	return (0);
}



t_flags *parse_flags(int ac, char **av){
	t_flags *flags;
	int i;
	bool double_dash;

	flags = malloc(sizeof(t_flags));
	if (!flags)
		return (NULL);
	ft_memset(flags, 0, sizeof(t_flags));
	double_dash = false;
	i = 1;
	while(i < ac){
		if(!double_dash && ft_strcmp(av[i], "--") == 0)
			double_dash = true;
		else if(!double_dash && av[i][0] == '-' && av[i][1] != '\0'){
			if(parse_arg(av[i], flags) == -1){
				free(flags);
				exit(EXIT_FAILURE);
			}
		}
		i++;
	}
	return (flags);
}