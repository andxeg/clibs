#include <stdio.h>
#include <stdlib.h>

#define T char*

int main() {
    T a = malloc(10);
    a[0] = 'a';
    a[1] = 'b';
    a[9] = 'c';
    printf("%s\n", a);
    return 0;
}

