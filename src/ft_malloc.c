#include "./includes/ft_malloc.h"


t_zone *alloc_large(size_t size){
	//calculer la taille de la zone ( sizeof t_block + t_header + size demandée)
	//demander la zone avec mmap
	//verifier si echec
	//faire le header de la zone
	//faire le header du bloc
	//rattacher la zone à la liste chainée
	//renvoyer le bloc
}


void *ft_malloc(size_t size){
	t_zone	*zone;
	size_t align_size;

	if(size == 0)
		return NULL;
	
	align_size = ft_align(size);
	if(select_zone(align_size) == LARGE) {
		zone = alloc_large(align_size);
	}
}