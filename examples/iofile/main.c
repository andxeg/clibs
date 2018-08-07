#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SEPARATOR '|'

int read_fields(FILE* file) {
    char c;
    size_t res;
    while (1) {
        res = fread(&c, 1, 1, file);
        if (res != 1 || ferror(file)) {
            printf("cannot read symbol\n");
            return 1;
        }
        
        if (c == '\n') {
            printf("read end of line\n");
            break;
        } else if (feof(file)) {
            printf("read end of file\n");
        }
        else { 
            putchar(c);
        }
    }
    putchar('\n');
    return 0;
}

int read_types(FILE* file) {
    return 0;
}

int read_limits(FILE* file) {
    return 0;
}

int read_goods(FILE* file) {
    return 0;
}


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("error in input parameters\n");
        printf("type %s <filename>\n", argv[0]); 
        return 1;
    }

    char* filename = argv[1];
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("cannot open file '%s'\n", filename);
        return 1;
    }

    read_fields(file);
    read_types(file);
    read_limits(file);
    read_limits(file);
    read_goods(file);
    return 0;
}

