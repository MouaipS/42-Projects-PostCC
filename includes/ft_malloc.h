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
	size_t 	 				size;		//capacite utilisable (alignee)
	size_t					req_size;	//taille demandee par l'appelant
	bool 	 				is_free;
	struct s_header 	 	*next;
	struct s_header 		*prev;
}		 					t_header;

typedef struct s_zone {
	int 					type;
	size_t					size;
	struct s_zone			*next;
	t_header				*start;
} 							t_zone;

extern t_zone *g_zones;
//malloc.c
void *malloc(size_t size);

//utils.c
size_t 		ft_align(size_t size);
int 		select_zone(size_t size);
t_zone 		*create_zone(int type);
t_header 	*find_free_block(int type, size_t size);
void 		split_block(t_header *block, size_t size);
t_header	*get_block(void *ptr, t_zone **out_zone);
void		merge_free(t_header *block);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		ft_putstr_fd(const char *s, int fd);

t_header *alloc_large(size_t align_size);
t_header *alloc_tiny_small(int type, size_t size);

//free.c
void		free(void *ptr);

//realloc.c
void		*realloc(void *ptr, size_t size);

//show_alloc_mem.c
void		show_alloc_mem(void);

#endif 