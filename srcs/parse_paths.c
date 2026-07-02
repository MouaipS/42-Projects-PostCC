#include "../includes/ft_ls.h"

char **parse_paths(int ac, char **av, int *nb_paths){
	char **paths;
	int double_dash;
	int count;
	int i;
	int j;
	
	i = 1;
	count = 0;
	double_dash = false;
	while(i < ac){
		if(!double_dash && ft_strcmp(av[i], "--") == 0)
			double_dash = true;
		else if(double_dash || av[i][0] != '-' || av[i][1] == '\0')
			count++;
		i++;
	}
	*nb_paths = count;
	if(count == 0)
		return (NULL);
	paths = malloc(sizeof(char *) * (count+1));
	if(!paths)
		return(NULL);
	double_dash = false;
	i = 1;
	j = 0;
	while(i < ac){
		if(!double_dash && ft_strcmp(av[i], "--") == 0)
			double_dash = true;
		else if(double_dash || av[i][0] != '-' || av[i][1] == '\0'){
			paths[j] = av[i];
			j++;
		}
		i++;
	}
	paths[j] = NULL;
	return(paths);
}