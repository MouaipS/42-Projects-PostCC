#ifndef FT_MALLOC_H
# define FT_MALLOC_H

#include <unistd.h>
#include <stdbool.h>


# define TINY 1
# define SMALL 2
# define LARGE 3

//TODO DEFINIR LES TAILLES
# define TINY_SIZE //definir une valeur
# define SMALL_SIZE //definir une valeur
# define LARGE_SIZE 


typedef struct s_header {
	size_t 	 				size; //espace pris dans ce bloc
	bool 	 				is_free;
	struct t_header 	 	*next;
	struct t_header 		*prev;
}		 					t_header; //header de block

typedef struct s_zone {
	int 					type; //TYNY, SMALL OR LARGE
	size_t					size; //taille totale de la zone
	struct t_zone			*next;
	t_header				*start;
} 							t_zone; //header de zone

//malloc.c
void ft_malloc(size_t size);

//utils.c
size_t ft_align(size_t size);


t_zone *alloc_large(size_t align_size);
t_zone *alloc_tiny_small(size_t align_size);

#endif 