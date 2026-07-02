#ifndef FT_LS_H
# define FT_LS_H

#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <printf.h>
#include "../libft/libft.h"

typedef struct s_flags {
	bool l;
	bool R;
	bool a;
	bool r;
	bool t;
}		t_flags;

typedef struct s_file{
	char *name;
	char *path;
	struct stat data;

}		t_file;

t_flags		*parse_flags(int ac, char **av);
char		**parse_paths(int ac, char **av, int *nb_paths);


#endif