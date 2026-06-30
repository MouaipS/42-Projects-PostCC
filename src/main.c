#include "ft_nm.h"

/*
** Traite un seul fichier ELF de bout en bout :
**   1. Alloue les structures intermédiaires sur le tas
**   2. Ouvre et mappe le fichier en mémoire (ft_setup)
**   3. Valide le magic ELF et détecte la classe 32/64 (ft_check_file)
**   4. Délègue au parseur 32 ou 64 bits selon la classe détectée
**   5. Libère toutes les ressources dans l'ordre correct
**
*/
static void process_nm(const char *filename)
{
	t_data data;
	data.filename = filename;

	data.header_struct = malloc(sizeof(t_elf));
	if (!data.header_struct)
		ft_error(filename, "memory exhausted");

	data.symtab_struct = malloc(sizeof(t_symtab));
	if (!data.symtab_struct)
	{
		free(data.header_struct);
		ft_error(filename, "memory exhausted");
	}

	ft_setup(&data, filename);         /* ouvrir + mmap le fichier */
	ft_check_file(&data, filename);    /* valider ELF + détecter 32/64 */

	/* Brancher sur la version 32 ou 64 bits selon is_64_or_32 */
	if (data.is_64_or_32 == true)
		process_64(&data);
	else
		process_32(&data);

	free(data.header_struct);
	free(data.symtab_struct);
	free(data.sym_array);
	munmap(data.map, data.buff.st_size);
}

int main(int ac, char **av)
{
	if (ac < 2) process_nm("a.out");
	else
	{
		for (int i = 1; i < ac; i++)
			process_nm(av[i]);
	}
	return (0);
}
