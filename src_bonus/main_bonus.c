#include "ft_nm.h"

/*
** Analyse les arguments en ligne de commande pour extraire les flags avant les noms de fichiers.
** Supporte les formes courtes (-a, -g, -u, -r, -p) et longues (--debug-syms, etc.),
** ainsi que les combinaisons (-gr pour -g -r simultanément).
**
** Retourne l'index du premier argument qui n'est pas un flag,
** c'est-à-dire le premier nom de fichier à traiter.
**
** On s'arrête dès qu'un argument ne commence pas par '-' ou vaut exactement "-" :
** cela permet d'écrire "ft_nm_bonus -g fichier" sans ambiguïté.
*/
static int parse_flags(t_data *data, int ac, char **av)
{
	int        i;
	const char *c;

	i = 1;
	while (i < ac && av[i][0] == '-' && av[i][1] != '\0')
	{
		c = av[i] + 1;

		if (*c == '-')
		{
			if (strcmp(av[i], "--extern-only") == 0)
				data->flags.g = true;
			else if (strcmp(av[i], "--undefined-only") == 0)
				data->flags.u = true;
			else if (strcmp(av[i], "--reverse-sort") == 0)
				data->flags.r = true;
			else if (strcmp(av[i], "--no-sort") == 0)
				data->flags.p = true;
			else if (strcmp(av[i], "--debug-syms") == 0)
				data->flags.a = true;
			else
			{
				printf("ft_nm: unknown option %s\n", av[i]);
				exit(1);
			}
			i++;
			continue;
		}

		while (*c)
		{
			if      (*c == 'g') data->flags.g = true;
			else if (*c == 'u') data->flags.u = true;
			else if (*c == 'r') data->flags.r = true;
			else if (*c == 'p') data->flags.p = true;
			else if (*c == 'a') data->flags.a = true;
			else
			{
				printf("ft_nm: unknown option -%c\n", *c);
				exit(1);
			}
			c++;
		}
		i++;
	}
	return i; /* Index du premier argument fichier */
}

/*
** Traite un fichier ELF avec les flags déjà parsés dans data->flags.
** La structure data est réutilisée entre les fichiers (les flags sont préservés),
** mais header_struct, symtab_struct et sym_array sont réalloués pour chaque fichier.
*/
static void process_nm(const char *filename, t_data *data)
{
	data->filename = filename;

	data->header_struct = malloc(sizeof(t_elf));
	if (!data->header_struct)
		ft_error(filename, "memory exhausted");

	data->symtab_struct = malloc(sizeof(t_symtab));
	if (!data->symtab_struct)
	{
		free(data->header_struct);
		ft_error(filename, "memory exhausted");
	}

	ft_setup_bonus(data, filename);
	ft_check_file_bonus(data, filename);

	/* process_64 et process_32 sont partagés avec la partie mandatory
	** mais appellent les versions bonus de symbols64/32 (via la compilation séparée)
	** qui tiennent compte des flags pour le filtrage et le tri. */
	if (data->is_64_or_32 == true)
		process_64(data);
	else
		process_32(data);

	free(data->header_struct);
	free(data->symtab_struct);
	free(data->sym_array);
	munmap(data->map, data->buff.st_size);
}

int main(int ac, char **av)
{
	t_data data;
	int    files_list;

	memset(&data, 0, sizeof(data));


	files_list = parse_flags(&data, ac, av);

	if (files_list >= ac)
	{
		process_nm("a.out", &data);
	}
	else
	{
		int is_multi = (ac - files_list > 1);
		for (; files_list < ac; files_list++)
		{
			if (is_multi)
			{
				write(1, "\n", 1);
				write(1, av[files_list], strlen(av[files_list]));
				write(1, ":\n", 2);
			}
			process_nm(av[files_list], &data);
		}
	}
	return (0);
}
