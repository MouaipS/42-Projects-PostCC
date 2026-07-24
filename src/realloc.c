#include "ft_malloc.h"

void *realloc(void *ptr, size_t size)
{
	t_zone   *zone = NULL;
	t_header *block;
	void     *newptr;
	size_t   asize;

	if (!ptr)
		return (malloc(size));
	if (size == 0)
		return (free(ptr), NULL);
	block = get_block(ptr, &zone);
	if (!block)
	{
		ft_putstr_fd("realloc(): invalid pointer\n", 2);
		return (NULL);
	}
	asize = ft_align(size);
	if (asize == 0)
		return (NULL);

	if (asize <= block->size)
	{
		if (zone->type != LARGE)
		{
			size_t old = block->size;
			split_block(block, asize);
			if (block->size != old && block->next && block->next->is_free)
				merge_free(block->next);
		}
		block->req_size = size;
		return (ptr);
	}

	if (zone->type != LARGE && block->next && block->next->is_free
		&& block->size + ft_align(sizeof(t_header)) + block->next->size >= asize)
	{
		block->size += ft_align(sizeof(t_header)) + block->next->size;
		block->next = block->next->next;
		if (block->next)
			block->next->prev = block;
		split_block(block, asize);
		block->req_size = size;
		return (ptr);
	}

	newptr = malloc(size);
	if (!newptr)
		return (NULL);
	ft_memcpy(newptr, ptr, block->size < asize ? block->size : asize);
	free(ptr);
	return (newptr);
}
