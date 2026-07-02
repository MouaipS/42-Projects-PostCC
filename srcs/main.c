#include "../includes/ft_ls.h"

int main(int ac, char **av) {
	t_flags	*flags;
	char	**paths;
	int		nb_paths;

	flags = parse_flags(ac, av);
	paths = parse_paths(ac, av, &nb_paths);
	if(paths == NULL){
		exit(EXIT_FAILURE);
	} else if (nb_paths == 0) {
		printf("Pas de path\n");
		if (flags->l == true)
			printf("Flag l\n");
		else if (flags->R == true)
			printf("Flag R\n");
		else if (flags->a == true)
			printf("Flag a\n");
		else if (flags->r == true)
			printf("Flag r\n");
		else if (flags->t == true)
			printf("Flag t\n");
		else {
			printf("Pas de flag\n");
		}
	} else {
		if (flags->l == true)
			printf("Flag l\n");
		else if (flags->R == true)
			printf("Flag R\n");
		else if (flags->a == true)
			printf("Flag a\n");
		else if (flags->r == true)
			printf("Flag r\n");
		else if (flags->t == true)
			printf("Flag t\n");
		int i =0;
		while(i < nb_paths){
			printf("path [%d] : %s\n", i, paths[i]);
			i++;
		}
	}
}
