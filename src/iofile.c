#include <errno.h>
#include "defs.h"
#include "iofile.h"


int read_file_with_goods(const char* filename, FILE_SCHEMA* schema) {
    errno = 0;
    FILE* input = fopen(filename, "r");
    if (input == NULL) {
        fprintf(stderr, "read_file_with_goods: error while read input file \n");
        char error_st[ERROR_LENGTH_LIMIT];
        sprintf(error_st, "File : %s\nLine : %d\nCurrent Function : %s\nFailed function : %s\nError message ", __FILE__, __LINE__, __func__, "fopen\0");
        perror(error_st);
        return 1;
    }
    
    if (read_file_header(input, schema)) {
        return 1;
    }

    if (read_goods(input, schema)) {
        return 1;
    }
    
    fclose(input);
    return 0;
} 

int read_file_header(FILE* file, FILE_SCHEMA* schema) {
//    int c;   
    return 0;
}

int read_goods(FILE* file, FILE_SCHEMA* schema) {
    return 0;
}

int write_file_header(FILE* file, FILE_SCHEMA* schema) {
    return 0;
}

int write_goods(FILE* file, FILE_SCHEMA* schema) {
    return 0;
}

