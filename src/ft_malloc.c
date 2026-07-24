#include "ft_malloc.h"

t_zone *g_zones = NULL;

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
	size_t overhead = ft_align(sizeof(t_zone)) + ft_align(sizeof(t_header));
	size_t page = (size_t)sysconf(_SC_PAGESIZE);
	size_t max_size;

	if(size > (size_t)-1 - overhead)
		return (NULL);
	max_size = overhead + size;
	if(max_size > (size_t)-1 - (page - 1))
		return (NULL);
	max_size = ((max_size + page -1) / page) * page;

	void *ptr = mmap(NULL, max_size, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1,0);
	if(ptr == MAP_FAILED)
		return (NULL);
		
	t_zone *zone = (t_zone *)ptr;
	zone->type = LARGE;
	zone->size = max_size;
	zone->next = NULL;

	t_header *header = (t_header *)((char *)ptr + ft_align(sizeof(t_zone)));
	header->is_free = false;
	header->size = size;
	header->next = NULL;
	header->prev = NULL;
	
	zone->start = header;
	zone->next = g_zones;
	g_zones = zone;
	return(header);
}


void *malloc(size_t size){
	t_header	*header = NULL;
	size_t align_size;

	if(size == 0)
		return NULL;
	
	align_size = ft_align(size);
	if(align_size == 0)
		return NULL;
	int type = select_zone(align_size);
	if(type == LARGE) {
		header = alloc_large(align_size);
	}
	else{
		header = alloc_tiny_small(type, align_size);
	}	
	if(!header)
		return NULL;

	header->req_size = size;
	return((char *)header + ft_align(sizeof(t_header)));
}