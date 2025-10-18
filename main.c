#include "unistd.h"
#include <stdio.h>
#include <stdlib.h>

ssize_t ft_write(int fd, const void *buff, size_t count);
size_t ft_strlen(const char *s);
char *ft_strcpy(char *dest, char *src);
int ft_strcmp(const char *s1, const char *s2);
ssize_t ft_read(int fd, void *buf, size_t count);
char *ft_strdup(char *src);


int main() {
	char *test_strlen = "test1";
	char *copytest1 = malloc(ft_strlen(test_strlen) +1);
	ft_write(1, "salut\n" , 6);
	printf("Resultat de ft_strlen : %ld\n", ft_strlen(test_strlen));
	printf("Resultat de ft_strlen : %ld\n", ft_strlen("test2\n"));
	copytest1 = ft_strcpy(copytest1, test_strlen);
	printf("Resultat de ft_strcpy : %s\n", copytest1);
	free(copytest1);

	const char *og1 = "hello";
	const char *og2 = "hello";
	const char *og3 = "World";
	int res1 =  ft_strcmp(og1, og2);
	int res2 = ft_strcmp(og1, og3);
	printf("res1 = %d et res2 =  %d. (h[104] -W[87] = 17) \n", res1, res2);

	char buf[32];
    ssize_t ret;

    ret = ft_read(0, buf, 31);
    if (ret == -1)
        perror("ft_read");
    else {
        buf[ret] = '\0';
        printf("Read %zd bytes: %s\n", ret, buf);
    }
}