#include "ft_malloc.h"

//int get_type()

/**
 * @return align space with 16 bytes step
 */
size_t ft_align(size_t size){
	return(((size +15) /16) *16);
}

int select_zone(size_t size){
	if(size <= TINY_SIZE_MAX){
		return TINY;
	} else if( size > SMALL_SIZE_MAX){
		return LARGE;
	} else
		return SMALL;
}

t_zone *create_zone(int type)
{
    size_t  block_max;
    size_t  block_size;
    size_t  max_size;
    size_t  page;
    void    *ptr;
    t_zone  *zone;
    t_header *header;

    if (type == TINY)	block_max = TINY_SIZE_MAX;
    else			  	block_max = SMALL_SIZE_MAX;

    //size d'un bloc complet
    block_size = ft_align(sizeof(t_header)) + ft_align(block_max);

    //taille de toute une zone
    max_size = block_size * 100 + ft_align(sizeof(t_zone));
    page = (size_t)sysconf(_SC_PAGESIZE);
    max_size = ((max_size + page - 1) / page) * page;

    ptr = mmap(NULL, max_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED)
        return (NULL);

    zone = (t_zone *)ptr;
    zone->type = type;
    zone->size = max_size;

    header = (t_header *)((char *)ptr + ft_align(sizeof(t_zone)));
    header->is_free = true;
    header->size = max_size - ft_align(sizeof(t_zone)) - ft_align(sizeof(t_header));
    header->next = NULL;
    header->prev = NULL;
    zone->start = header;

    zone->next = g_zones;
    g_zones = zone;

    return (zone);
}

t_header *find_free_block(int type, size_t size)
{
    t_zone   *zone = g_zones;
    t_header *block;

    while (zone)
    {
        if(zone->type == type){
			block = zone->start;
			while(block){
				if(block->is_free && block->size >= size)
					return(block);
				block = block->next;
			}
		}
        zone = zone->next;
    }
    return (NULL);
}

void split_block(t_header *block, size_t size){
	if(block->size < size + ft_align(sizeof(t_header))+16)
		return;
	//calculer adresse du nouveau block
	t_header *new_start = (t_header *)((char *)block + ft_align(sizeof(t_header)) + size);
	//remplir le nouveau bloc
	new_start->is_free = true;
	new_start->size = block->size - size - ft_align(sizeof(t_header));
	//inserer dans la liste chainée
	new_start->next = block->next;
	new_start->prev = block;
	if(block->next)
		block->next->prev = new_start;
	block->next = new_start;
	//reduire la taille de l'ancien bloc
	block->size = size;
}

