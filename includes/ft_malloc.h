#ifndef FT_MALLOC_H
# define FT_MALLOC_H

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h>



# define TINY 1
# define SMALL 2
# define LARGE 3

# define TINY_SIZE_MAX 128
# define SMALL_SIZE_MAX 1024

//header = bloc
//zone = page

typedef struct s_header {
	size_t 	 				size; //espace pris dans ce bloc
	bool 	 				is_free;
	struct s_header 	 	*next;
	struct s_header 		*prev;
}		 					t_header; //header de block

typedef struct s_zone {
	int 					type; //TYNY, SMALL OR LARGE
	size_t					size; //taille totale de la zone
	struct s_zone			*next;
	t_header				*start;
} 							t_zone; //header de zone

extern t_zone *g_zones;
//malloc.c
void *malloc(size_t size);

//utils.c
size_t ft_align(size_t size);
int select_zone(size_t size);
t_zone *create_zone(int type);
t_header *find_free_block(int type, size_t size);
void split_block(t_header *block, size_t size);

t_header *alloc_large(size_t align_size);
t_header *alloc_tiny_small(int type, size_t size);

#endif 