#include "./includes/ft_malloc.h"

//int get_type()

/**
 * @return align space with 16 bytes step
 */
size_t ft_aligne(size_t size){
	return((size +15) /16) *16)
}

int select_zone(size_t size){
	if(size <= TINY_SIZE){
		return TINY;
	} else if( size > LARGE_SIZE){
		return LARGE;
	} else
		return SMALL;
}