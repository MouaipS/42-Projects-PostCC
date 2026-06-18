#include <stdio.h>
#define FILE "Grace_kid.c"
#define CODE "#include <stdio.h>%c#define FILE %c%s%c%c#define CODE %c%s%c%c#define FUNC(mode) int main(){fprintf(fopen(FILE, mode), CODE, 10, 34, FILE, 34, 10, 34, CODE, 34, 10, 10, 34, 119, 34, 10, 47, 47);}%cFUNC(%c%c%c)%c%c*COMMENT*%c"
#define FUNC(mode) int main(){fprintf(fopen(FILE, mode), CODE, 10, 34, FILE, 34, 10, 34, CODE, 34, 10, 10, 34, 119, 34, 10, 47, 47);}
FUNC("w")
/*COMMENT*/