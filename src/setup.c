#include "ft_nm.h"

void ft_setup(t_data *data, const char *filename){
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd < 0){
        free(data->header_struct);
	    free(data->symtab_struct);
        ft_error(filename, strerror(errno));
    }
    if (fstat(fd, &data->buff) < 0){
        close(fd);
        free(data->header_struct);
	    free(data->symtab_struct);
        ft_error(filename, strerror(errno));
    }
    if(data->buff.st_size < (off_t)EI_NIDENT){
        close(fd);
        free(data->header_struct);
	    free(data->symtab_struct);
        ft_error(filename, "file format not recognized");
    }
    data->map = mmap(NULL, (size_t)data->buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(data->map == MAP_FAILED){
        free(data->header_struct);
	    free(data->symtab_struct);
        close(fd);
        ft_error(filename, strerror(errno));
    }
    close(fd);
}

void ft_check_file(t_data *data, const char *filename){
    u_int8_t *index;
    index = data->map;
    if (index[0] != 0x7f || index[1] != 'E' || index[2] != 'L' || index[3] != 'F'){
        free(data->header_struct);
	    free(data->symtab_struct);
        munmap(data->map, data->buff.st_size);
        ft_error(filename, "file format not recognized");
    }
    if(index[EI_CLASS] == 1){
        data->is_64_or_32 = false;
        if((size_t)data->buff.st_size < sizeof(Elf32_Ehdr))
        {
            free(data->header_struct);
	        free(data->symtab_struct);
            munmap(data->map, data->buff.st_size);
            ft_error(filename, "file format not recognized");
        }
    } else if(index[EI_CLASS] == 2){
        data->is_64_or_32 = true;
        if((size_t)data->buff.st_size < sizeof(Elf64_Ehdr))
        {
            free(data->header_struct);
	        free(data->symtab_struct);
            munmap(data->map, data->buff.st_size);
            ft_error(filename, "file format not recognized");
        }
    } else{
        free(data->header_struct);
	    free(data->symtab_struct);
        munmap(data->map, data->buff.st_size);
        ft_error(filename, "file format not recognized");
    }
}