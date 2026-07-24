#include "ft_malloc.h"

static void put_str(const char *s)
{
	size_t len = 0;

	while (s[len])
		len++;
		
	write(1, s, len);
}

static void put_nbr(size_t n)
{
	char buf[24];
	int  i = 0;

	if (n == 0)
		return ((void)write(1, "0", 1));
	while (n > 0)
	{
		buf[i++] = '0' + (n % 10);
		n /= 10;
	}
	while (i > 0)
		write(1, &buf[--i], 1);
}

static void put_addr(void *ptr)
{
	char          buf[16];
	unsigned long n = (unsigned long)ptr;
	const char    *hex = "0123456789abcdef";
	int           i = 0;

	put_str("0x");
	if (n == 0)
		return ((void)write(1, "0", 1));
	while (n > 0)
	{
		buf[i++] = hex[n % 16];
		n /= 16;
	}
	while (i > 0)
		write(1, &buf[--i], 1);
}

static const char *type_name(int type)
{
	if (type == TINY)
		return ("TINY");
	if (type == SMALL)
		return ("SMALL");
	return ("LARGE");
}

static size_t print_zone(t_zone *zone)
{
	t_header *block = zone->start;
	size_t   subtotal = 0;
	void     *start;

	put_str(type_name(zone->type));
	put_str(": ");
	put_addr(zone);
	put_str("\n");
	while (block)
	{
		if (!block->is_free)
		{
			start = (char *)block + ft_align(sizeof(t_header));
			put_addr(start);
			put_str(" - ");
			put_addr((char *)start + block->req_size);
			put_str(" : ");
			put_nbr(block->req_size);
			put_str("\n");
			subtotal += block->req_size;
		}
		block = block->next;
	}
	return (subtotal);
}

static size_t print_type(int type)
{
	t_zone *zone;
	size_t total = 0;
	int    count = 0;
	int    i;

	zone = g_zones;
	while (zone)
	{
		count++;
		zone = zone->next;
	}
	while (count > 0)
	{
		zone = g_zones;
		i = 1;
		while (i < count)
		{
			zone = zone->next;
			i++;
		}
		if (zone->type == type)
			total += print_zone(zone);
		count--;
	}
	return (total);
}

void show_alloc_mem(void)
{
	size_t total = 0;

	total += print_type(TINY);
	total += print_type(SMALL);
	total += print_type(LARGE);
	put_str("Total : ");
	put_nbr(total);
	put_str(" bytes\n");
}
