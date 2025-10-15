#include "unistd.h"
#include <stdio.h>

ssize_t ft_write(int fd, const void *buff, size_t count);
size_t ft_strlen(const char *s);


int main() {
	char *test_strlen = "test1";
	ft_write(1, "salut\n" , 6);
	printf("Resultat de ft_strlen : %ld\n", ft_strlen(test_strlen));
	printf("Resultat de ft_strlen : %ld\n", ft_strlen("test2\n"));
}