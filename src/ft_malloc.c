#include "./includes/ft_malloc.h"

t_zone *g_zones = NULL;

t_header *alloc_large(size_t size){
	//calculer la taille de la zone ( sizeof t_block + t_header + size demandée)
	size_t max_size = ft_align(sizeof(t_zone)) + ft_align(sizeof(t_header))+ size;
	size_t page = (size_t)sysconf(_SC_PAGE_SIZE);
	max_size = ((max_size + page -1) / page) * page;

	//demander la zone avec mmap
	void *ptr = mmap(NULL, max_size, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, 0,0);
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


void *ft_malloc(size_t size){
	t_header	*header;
	size_t align_size;

	if(size == 0)
		return NULL;
	
	align_size = ft_align(size);
	if(select_zone(align_size) == LARGE) {
		header = alloc_large(align_size);
	}
	if(!header)
		return NULL;
	return((char *)header + ft_align(sizeof(t_header)));
}