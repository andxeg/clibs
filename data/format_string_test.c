#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 5

int main(int argc, char** argv) {
    char* template = (char* ) malloc(sizeof(char) * 2 * MAX_SIZE + 1);
    int i;
    for (i = 0; i < MAX_SIZE * 2; i += 2) {
        template[i] = '%';
        template[i+1] = 'd';
    }
    template[i] = '\0';

    printf("template -> %s\n", template);
    printf(template, 1, 2, 3, 4, 5);
    return 0;
}
