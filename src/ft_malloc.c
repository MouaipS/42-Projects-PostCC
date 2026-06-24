#include "ft_malloc.h"

t_header *alloc_tiny_small(int type, size_t size)
{
    t_header *block;
    t_zone   *zone;

    block = find_free_block(type, size);
    if (!block)
    {
        zone = create_zone(type);
        if (!zone)
            return (NULL);                 
        block = zone->start;
    }
    split_block(block, size);
    block->is_free = false;
    return (block);
}

t_header *alloc_large(size_t size){
	//calculer la taille de la zone ( sizeof t_block + t_header + size demandée)
	size_t max_size = ft_align(sizeof(t_zone)) + ft_align(sizeof(t_header))+ size;
	size_t page = (size_t)sysconf(_SC_PAGESIZE);
	max_size = ((max_size + page -1) / page) * page;

	//demander la zone avec mmap
	void *ptr = mmap(NULL, max_size, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1,0);
	//verifier si echec
	if(ptr == MAP_FAILED)
		return (NULL);
		
	//faire le header de la zone
	t_zone *zone = (t_zone *)ptr;
	zone->type = LARGE;
	zone->size = max_size;
	zone->next = NULL;

	//faire le header du bloc
	t_header *header = (t_header *)((char *)ptr + ft_align(sizeof(t_zone)));
	header->is_free = false;
	header->size = size;
	header->next = NULL;
	header->prev = NULL;
	
	zone->start = header;
	//rattacher la zone à la liste chainée
	zone->next = g_zones;
	g_zones = zone;
	//renvoyer le bloc
	return(header);
}


void *malloc(size_t size){
	t_header	*header = NULL;
	size_t align_size;

	if(size == 0)
		return NULL;
	
	align_size = ft_align(size);
	int type = select_zone(align_size);
	if(type == LARGE) {
		header = alloc_large(align_size);
	}
	else{
		header = alloc_tiny_small(type, align_size);
	}	
	if(!header)
		return NULL;
	return((char *)header + ft_align(sizeof(t_header)));
}