#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "defs.h"
#include "iofile.h"


int read_file_with_goods(const char* filename, FILE_SCHEMA* schema) {
    errno = 0;
    FILE* input = fopen(filename, "r");
    int error_num = errno;
    if (input == NULL) {
        fprintf(stderr, "read_file_with_goods: error while read input file \n");
        char* error_msg = strerror(error_num);
        printf("File : %s\nLine : %d\nCurrent Function : %s\nFailed function : %s\nError message %s\n", __FILE__, __LINE__, __func__, "fopen\0", error_msg);
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
    if (read_fields(file, schema)) {
        return 1;
    }

    if (read_types(file, schema)) {
        return 1;
    }

    if (read_limits(file, schema)) {
        return 1;
    }
   
    return 0;
}

int read_fields(FILE* file, FILE_SCHEMA* schema) {
    int c;
    int inside = 0;
    TEMPLATE(DYN_ARRAY, char) array_char;
 /*
    while ((c = getchar()) != EOF && c != '\n') {
        if (c == GOODS_FILE_SEPARATOR) {
        ;    
        } else if (isalpha(c) || isdigit(c) || c == ' ' || c == '.') {
        ;
        }
    }
 */   
    return 0;
}

int read_types(FILE* file, FILE_SCHEMA* schema) {
    return 0;
}

int read_limits(FILE* file, FILE_SCHEMA* schema) {
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

