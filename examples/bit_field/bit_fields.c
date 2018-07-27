#include <stdio.h>
#include <string.h>

typedef struct {
    unsigned int a;
    unsigned int b;
} ST1;

typedef struct {
    unsigned int a : 1;
    unsigned int b : 1;
} ST2;

int main(int argc, char *argv[]) {
    ST1 st1;
    ST2 st2;
    fprintf(stdout, "%zu\n", sizeof(st1)); 
    fprintf(stdout, "%zu\n", sizeof (st2));
    return 0;
}

