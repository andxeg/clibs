#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "defs.h"
#include "log.h"
#include "iofile.h"


int read_file_with_goods(const char* filename, FILE_SCHEMA* schema) {
    errno = 0;
    FILE* input = fopen(filename, "r");
    int error_num = errno;
    if (input == NULL) {
        ELOG("error while read input file");
        char* error_msg = strerror(error_num);
        ELOG(error_msg);
        return 1;
    }
    
    if (read_file_header(input, schema)) {
        fclose(input);
        return 1;
    }

    if (read_goods(input, schema)) {
        fclose(input);
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
    if (schema->header->fields.length != schema->header->types.length) {
        ELOG("fields count is not equal to types count");
        return 1;
    }
    
    if (read_limits(file, &schema->header->length_min)) {
        return 1;
    }
    
    if (schema->header->length_min.length != schema->header->fields.length) {
        ELOG("min-limits count is not equal to fields count");
        return 1;
    }

    if (read_limits(file, &schema->header->length_max)) {
        return 1;
    }
    
    if (schema->header->length_max.length != schema->header->fields.length) {
        ELOG("max-limits count is not equal to fields count");
        return 1;
    }
 
    return 0;
}

int read_fields(FILE* file, FILE_SCHEMA* schema) {
    int c;
    char* raw_data = NULL;
    int inside = 0;
    TEMPLATE(DYN_ARRAY, char) array_char;
    TEMPLATE(create, char)(DEFAULT_FIELD_SIZE, &array_char);
    while ((c = getc(file)) != EOF && c != '\n') {
        if (c == GOODS_FILE_SEPARATOR) {
            if (inside) {
                TEMPLATE(append, char)(&array_char, '\0');
                if (TEMPLATE(shrink_to_fit, char)(&array_char)) {
                    ELOG("cannot shrink input field");
                    TEMPLATE(destroy, char)(&array_char); 
                    return 1;
                }
                raw_data = TEMPLATE(get_raw_data, char)(&array_char);
                printf("field -> %s\n", raw_data);
                TEMPLATE(append, string)(&schema->header->fields, raw_data);
                TEMPLATE(create, char)(DEFAULT_FIELD_SIZE, &array_char);
            }
            inside = 0;
        } else if (isalpha(c) || isdigit(c) || c == ' ' || c == '.' || c == '_') {
            inside = 1;
            TEMPLATE(append, char)(&array_char, c);
        } else {
            ELOG("unknown symbol in field");
            TEMPLATE(destroy, char)(&array_char); 
            return 1;
        }
    }
    
    // last field
    if (array_char.length != 0) {
         TEMPLATE(append, char)(&array_char, '\0');
         if (TEMPLATE(shrink_to_fit, char)(&array_char)) {
             ELOG("cannot shrink input field");
             TEMPLATE(destroy, char)(&array_char); 
             return 1;
         }
         raw_data = TEMPLATE(get_raw_data, char)(&array_char);
         printf("field -> %s\n", raw_data);
         TEMPLATE(append, string)(&schema->header->fields, raw_data);
    }
    return 0;
}

int read_types(FILE* file, FILE_SCHEMA* schema) {
    int c;
    char* raw_data = NULL;
    int inside = 0;
    TEMPLATE(DYN_ARRAY, char) array_char;
    TEMPLATE(create, char)(DEFAULT_FIELD_SIZE, &array_char);
    while ((c = getc(file)) != EOF && c != '\n') {
        if (c == GOODS_FILE_SEPARATOR) {
            if (inside) {
                TEMPLATE(append, char)(&array_char, '\0');
                raw_data = TEMPLATE(get_raw_data, char)(&array_char);
                printf("field -> %s\n", raw_data);
                if (strcmp(raw_data, "STRING\0") == 0) {
                    TEMPLATE(append, good_field)(&schema->header->types, STRING_GOOD);
                } else if (strcmp(raw_data, "NUMBER\0") == 0) {
                    TEMPLATE(append, good_field)(&schema->header->types, NUMBER_GOOD);
                } else if(strcmp(raw_data, "BOOL\0") == 0) {
                    TEMPLATE(append, good_field)(&schema->header->types, BOOL_GOOD);
                } else {
                    ELOG("unknown type of field");
                    TEMPLATE(destroy, char)(&array_char);
                    return 1;
                }
                TEMPLATE(recreate, char)(DEFAULT_FIELD_SIZE, &array_char);
            }
            inside = 0;
        } else if (isalpha(c) || isdigit(c) || c == ' ' || c == '.' || c == '_') {
            inside = 1;
            TEMPLATE(append, char)(&array_char, c);
        } else {
            ELOG("unknown symbol in field");
            TEMPLATE(destroy, char)(&array_char); 
            return 1;
        }
    }
    
    // last type value
    if (array_char.length != 0) {
        TEMPLATE(append, char)(&array_char, '\0');
        raw_data = TEMPLATE(get_raw_data, char)(&array_char);
        printf("field -> %s\n", raw_data);
        if (strcmp(raw_data, "STRING\0") == 0) {
            TEMPLATE(append, good_field)(&schema->header->types, STRING_GOOD);
        } else if (strcmp(raw_data, "NUMBER\0") == 0) {
            TEMPLATE(append, good_field)(&schema->header->types, NUMBER_GOOD);
        } else if(strcmp(raw_data, "BOOL\0") == 0) {
            TEMPLATE(append, good_field)(&schema->header->types, BOOL_GOOD);
        } else {
            ELOG("unknown type of field");
            TEMPLATE(destroy, char)(&array_char);
            return 1;
        }
    }
    TEMPLATE(destroy, char)(&array_char);
    return 0;
}

int read_limits(FILE* file, TEMPLATE(DYN_ARRAY, int)* limits) {
    int c;
    int num;
    while (fscanf(file, "%d", &num)) {
        printf("number -> %d", num);
        c = getc(file);
        if (c == GOODS_FILE_SEPARATOR) {
            TEMPLATE(append, int)(limits, num);
        } else if (c == EOF || c == '\n') {
            TEMPLATE(append, int)(limits, num);
            break;
        } else {
            char error_msg[ERROR_LENGTH_LIMIT];
            sprintf(error_msg, "unknown symbol -> %c", c);
            ELOG(error_msg);
            return 1;
        }
    }
    
    return 0;
}

int read_goods(FILE* file, FILE_SCHEMA* schema) {
    
    return 0;
}

int write_file_with_goods(const char* filename, FILE_SCHEMA* schema) {
    return 0;
}

int write_file_header(FILE* file, FILE_SCHEMA* schema) {
    return 0;
}

int write_goods(FILE* file, FILE_SCHEMA* schema) {
    return 0;
}

