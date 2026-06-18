#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int X = 5;

#define CODE "#include <stdio.h>%1$c#include <stdlib.h>%1$c#include <string.h>%1$c%1$cint X = %4$d;%1$c%1$c#define CODE %2$c%3$s%2$c%1$c%1$cint main(void)%1$c{%1$c%5$cchar self[100];%1$c%5$cchar src[100];%1$c%5$cchar exec[100];%1$c%5$cchar cmd[300];%1$c%5$cFILE *f;%1$c%1$c%5$csprintf(self, %2$cSully_%%d.c%2$c, X);%1$c%5$cif (!strcmp(self, __FILE__))%1$c%5$c%5$cX--;%1$c%5$cif (X < 0)%1$c%5$c%5$creturn (0);%1$c%5$csprintf(src, %2$cSully_%%d.c%2$c, X);%1$c%5$csprintf(exec, %2$cSully_%%d%2$c, X);%1$c%5$cf = fopen(src, %2$cw%2$c);%1$c%5$cif (f == NULL)%1$c%5$c%5$creturn (1);%1$c%5$cfprintf(f, CODE, 10, 34, CODE, X, 9);%1$c%5$cfclose(f);%1$c%5$csprintf(cmd, %2$ccc -Wall -Wextra -Werror -o %%s %%s%2$c, exec, src);%1$c%5$csystem(cmd);%1$c%5$csprintf(cmd, %2$c./%%s%2$c, exec);%1$c%5$csystem(cmd);%1$c%5$creturn (0);%1$c}%1$c"

int main(void)
{
	char self[100];
	char src[100];
	char exec[100];
	char cmd[300];
	FILE *f;

	sprintf(self, "Sully_%d.c", X);
	if (!strcmp(self, __FILE__))
		X--;
	if (X < 0)
		return (0);
	sprintf(src, "Sully_%d.c", X);
	sprintf(exec, "Sully_%d", X);
	f = fopen(src, "w");
	if (f == NULL)
		return (1);
	fprintf(f, CODE, 10, 34, CODE, X, 9);
	fclose(f);
	sprintf(cmd, "cc -Wall -Wextra -Werror -o %s %s", exec, src);
	system(cmd);
	sprintf(cmd, "./%s", exec);
	system(cmd);
	return (0);
}
