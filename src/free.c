#include "ft_malloc.h"

static bool zone_empty(t_zone *zone)
{
	return (zone->start && zone->start->next == NULL && zone->start->is_free);
}

static bool other_zone_exists(t_zone *zone, int type)
{
	t_zone *cur = g_zones;

	while (cur)
	{
		if (cur != zone && cur->type == type)
			return (true);
		cur = cur->next;
	}
	return (false);
}

static void unlink_zone(t_zone *zone)
{
	t_zone *current = g_zones;
	t_zone *prev = NULL;

	while (current)
	{
		if (current == zone)
		{
			if (prev)
				prev->next = current->next;
			else
				g_zones = current->next;
			return;
		}
		prev = current;
		current = current->next;
	}
}

void free(void *ptr)
{
	t_zone   *zone = NULL;
	t_header *block;

	if (!ptr)
		return;
	block = get_block(ptr, &zone);
	if (!block)
	{
		ft_putstr_fd("free(): invalid pointer\n", 2);
		return;
	}

	if (zone->type == LARGE)
	{
		unlink_zone(zone);
		munmap(zone, zone->size);
		return;
	}

	block->is_free = true;
	merge_free(block);
	if (zone_empty(zone) && other_zone_exists(zone, zone->type))
	{
		unlink_zone(zone);
		munmap(zone, zone->size);
	}
}
