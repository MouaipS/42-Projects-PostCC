#include "ft_nm.h"

void ft_setup_bonus(t_data *data, const char *filename){
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd < 0){
        free(data->header_struct);
	    free(data->symtab_struct);
        ft_error(strerror(errno));
    }
    if (fstat(fd, &data->buff) < 0){
        close(fd);
        free(data->header_struct);
	    free(data->symtab_struct);
        ft_error(strerror(errno));
    }
    if(data->buff.st_size < (off_t)EI_NIDENT){ //16 octets minumum pour avoir les magic 
        close(fd);
        free(data->header_struct);
	    free(data->symtab_struct);
        ft_error("file too small");
    }
    data->map = mmap(NULL, (size_t)data->buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(data->map == MAP_FAILED){
        close(fd);
        free(data->header_struct);
	    free(data->symtab_struct);
        ft_error(strerror(errno));
    }
    close(fd);
}

void ft_check_file_bonus(t_data *data, const char *filename){
    u_int8_t *index;
    (void)filename;
    index = data->map;
    if (index[0] != 0x7f || index[1] != 'E' || index[2] != 'L' || index[3] != 'F'){
        free(data->header_struct);
	    free(data->symtab_struct);
        munmap(data->map, data->buff.st_size);
        ft_error("Not a ELF file");
    }
    if(index[EI_CLASS] == 1){ //EI_CLASS == 4
        data->is_64_or_32 = false;
        if((size_t)data->buff.st_size < sizeof(Elf32_Ehdr)) //taille minimum du header 52 octets
        {
            free(data->header_struct);
	        free(data->symtab_struct);
            munmap(data->map, data->buff.st_size);
            ft_error("Invalid file's class");
        }
    } else if(index[EI_CLASS] == 2){
        data->is_64_or_32 = true;
        if((size_t)data->buff.st_size < sizeof(Elf64_Ehdr)) //taille minimum du header 64 octets
        {
            free(data->header_struct);
	        free(data->symtab_struct);
            munmap(data->map, data->buff.st_size);
            ft_error("Invalid file's class");
        }
    } else{
        free(data->header_struct);
	    free(data->symtab_struct);
        munmap(data->map, data->buff.st_size);
        ft_error("Invalid file's class");
    }
} 