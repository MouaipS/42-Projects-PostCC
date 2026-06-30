#include "ft_nm.h"

/*
** Ouvre le fichier en lecture seule, récupère sa taille via fstat,
** puis mappe tout son contenu en mémoire avec mmap.
**
** data->map pointe directement sur les octets du fichier,
** ce qui permet de naviguer dans les structures ELF par simple arithmétique
** de pointeurs, sans copie.
*/
void ft_setup(t_data *data, const char *filename)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		free(data->header_struct);
		free(data->symtab_struct);
		ft_error(filename, strerror(errno));
	}

	if (fstat(fd, &data->buff) < 0)
	{
		close(fd);
		free(data->header_struct);
		free(data->symtab_struct);
		ft_error(filename, strerror(errno));
	}

	/* EI_NIDENT = 16 : taille minimale pour lire le magic et la classe ELF.
	** Sans cette vérification, l'accès à index[EI_CLASS] dans ft_check_file
	** pourrait lire hors des limites du fichier. */
	if (data->buff.st_size < (off_t)EI_NIDENT)
	{
		close(fd);
		free(data->header_struct);
		free(data->symtab_struct);
		ft_error(filename, "file format not recognized");
	}

	data->map = mmap(NULL, (size_t)data->buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (data->map == MAP_FAILED)
	{
		free(data->header_struct);
		free(data->symtab_struct);
		close(fd);
		ft_error(filename, strerror(errno));
	}

	close(fd);
}

/*
** Vérifie que le fichier mappé est bien un ELF valide et détermine
** s'il est 32 ou 64 bits. En cas d'erreur, munmap est appelé avant exit
** car data->map est déjà valide à ce stade.
*/
void ft_check_file(t_data *data, const char *filename)
{
	u_int8_t *index;

	index = data->map;

	/* Le magic ELF est toujours 0x7f 'E' 'L' 'F' aux 4 premiers octets.
	** Tout fichier ne commençant pas par cette séquence n'est pas un ELF. */
	if (index[0] != 0x7f || index[1] != 'E' || index[2] != 'L' || index[3] != 'F')
	{
		free(data->header_struct);
		free(data->symtab_struct);
		munmap(data->map, data->buff.st_size);
		ft_error(filename, "file format not recognized");
	}

	/* EI_CLASS (index 4) indique la classe du fichier :
	**   ELFCLASS32 (1) : architecture 32-bit, adresses sur 4 octets
	**   ELFCLASS64 (2) : architecture 64-bit, adresses sur 8 octets */
	if (index[EI_CLASS] == 1)
	{
		data->is_64_or_32 = false;
		/* Vérifier que le fichier est assez grand pour contenir un ELF Header 32-bit (52 octets) */
		if ((size_t)data->buff.st_size < sizeof(Elf32_Ehdr))
		{
			free(data->header_struct);
			free(data->symtab_struct);
			munmap(data->map, data->buff.st_size);
			ft_error(filename, "file format not recognized");
		}
	}
	else if (index[EI_CLASS] == 2)
	{
		data->is_64_or_32 = true;
		/* Vérifier que le fichier est assez grand pour contenir un ELF Header 64-bit (64 octets) */
		if ((size_t)data->buff.st_size < sizeof(Elf64_Ehdr))
		{
			free(data->header_struct);
			free(data->symtab_struct);
			munmap(data->map, data->buff.st_size);
			ft_error(filename, "file format not recognized");
		}
	}
	else
	{
		/* Classe inconnue : ni 32 ni 64 bits */
		free(data->header_struct);
		free(data->symtab_struct);
		munmap(data->map, data->buff.st_size);
		ft_error(filename, "file format not recognized");
	}
}
