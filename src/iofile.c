#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "defs.h"
#include "log.h"
#include "iofile.h"
#include "good_field_types.h"


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

int get_int_value(TEMPLATE(DYN_ARRAY, char)* array_char, int* value) {
    char* eptr;
    long result = strtol(array_char->data, &eptr, 10);
    if (result == 0) {
        if (errno == EINVAL) {
            ELOG("error while convert string to number");
            return 1;
        }
        if (errno == ERANGE) {
            ELOG("converted integer value is out of range");
            return 1;
        }
    }
    char msg[LOG_MSG_LENGTH_LIMIT];
    sprintf(msg, "converted number -> %ld", result);
    SLOG(msg);
    *value = (int) result; 
    return 0;
}

int get_bool_value(TEMPLATE(DYN_ARRAY, char)* array_char, int* value) {
    int result;
    if (get_int_value(array_char, &result)) {
        return 1;
    }

    if (result != 0 && result != 1) { 
        return 1;
    }
    
    *value = result;
    return 0;
}

TEMPLATE(DYN_ARRAY, int)* create_array_with_types(TEMPLATE(DYN_ARRAY, good_field)* types) {
    int length = types->length;
    TEMPLATE(DYN_ARRAY, int)* int_types = TEMPLATE(create2, int)(length);
    for (int i = 0; i < length; i++) {
        GOOD_FIELD_TYPE type;
        TEMPLATE(get, good_field)(types, i, &type);
        switch (type) {
        case BOOL_GOOD:
            TEMPLATE(append, int)(int_types, BOOL);
            break;
        case NUMBER_GOOD:
            TEMPLATE(append, int)(int_types, INT);
            break;
        case STRING_GOOD:
            TEMPLATE(append, int)(int_types, STRING);
            break;
        default:
            ;char error_msg[ERROR_LENGTH_LIMIT];
            sprintf(error_msg, "unknown type types array from file header-> %d", type);
            ELOG(error_msg);
            return NULL;
        }
    }
    
    return int_types;
}

int process_single_field() {
    
    return 0;
}


int read_goods(FILE* file, FILE_SCHEMA* schema) {
    TEMPLATE(DYN_ARRAY, good_field)* types = &schema->header->types;
    TEMPLATE(DYN_ARRAY, int)* min_limit = &schema->header->length_min;
    TEMPLATE(DYN_ARRAY, int)* max_limit = &schema->header->length_max; 
    TEMPLATE(LIST, dyn_array_vop)* goods = schema->goods;
    
    // create array with types
    int length = types->length;
    TEMPLATE(DYN_ARRAY, int)* int_types = create_array_with_types(types);
    if (int_types == NULL) {
        ELOG("cannot convert good field types to int types");
        return 1;
    }
    TEMPLATE(DYN_ARRAY, vop) array_void;
    TEMPLATE(create, vop)(length, &array_void);
    
    int c;
    int inside = 0; // flag in word or not
    int good_num = 1;
    int curr_field = 0;
    TEMPLATE(DYN_ARRAY, char) array_char;
    TEMPLATE(create, char)(DEFAULT_FIELD_SIZE, &array_char);
    while ((c = getc(file)) != EOF) {
        if (c == '\n') {
           printf("curr_field -> %d\n", curr_field);
           // add last field
           printf("add last field\n");
           if (inside) {
                int len = TEMPLATE(array_length, char)(&array_char);
                int min, max;
                TEMPLATE(get, int)(min_limit, curr_field, &min);
                TEMPLATE(get, int)(max_limit, curr_field, &max);
                if (len < min || len > max) {
                    char error_msg[ERROR_LENGTH_LIMIT];
                    char* field;
                    TEMPLATE(get, string)(&schema->header->fields, curr_field, &field);
                    sprintf(error_msg, "good #%d has incorrect value in field %s", good_num, field);
                    ELOG(error_msg);
                    TEMPLATE(destroy, char)(&array_char);
                    if (TEMPLATE(size_list, dyn_array_vop)(goods) == 0) {
                        TEMPLATE(destroy2, int)(int_types);
                    }
                    return 1;
                }
               
                int type;
                int* val = (int *)malloc(sizeof(int));
                TEMPLATE(get, int)(int_types, curr_field, &type);
                TEMPLATE(append, char)(&array_char, '\0');
                switch (type) {
                case BOOL:
                     if (get_bool_value(&array_char, val)) {
                         char error_msg[ERROR_LENGTH_LIMIT];
                         sprintf(error_msg, "cannot convert %s to bool value", array_char.data);
                         ELOG(error_msg);
                         TEMPLATE(destroy, char)(&array_char);
                         if (TEMPLATE(size_list, dyn_array_vop)(goods) == 0) {
                             TEMPLATE(destroy2, int)(int_types);
                         }
                         return 1;     
                     }
                     printf("append to void array\n");
                     TEMPLATE(append, vop)(&array_void, (void *)val);
                     break;
                case INT:
                     if (get_int_value(&array_char, val)) {
                         char error_msg[ERROR_LENGTH_LIMIT];
                         sprintf(error_msg, "cannot convert %s to integer value", array_char.data);
                         ELOG(error_msg);
                         TEMPLATE(destroy, char)(&array_char);
                         if (TEMPLATE(size_list, dyn_array_vop)(goods) == 0) {
                             TEMPLATE(destroy2, int)(int_types);
                         }
                         return 1;
                     }
                     printf("append to void array\n");
                     TEMPLATE(append, vop)(&array_void, (void *)val);
                     break;
                case STRING:
                     if (TEMPLATE(shrink_to_fit, char)(&array_char)) {
                         ELOG("cannot shrink input field");
                         TEMPLATE(destroy, char)(&array_char);
                         if (TEMPLATE(size_list, dyn_array_vop)(goods) == 0) {
                             TEMPLATE(destroy2, int)(int_types);
                         }
                         return 1;
                     }
                     
                     printf("append to void array\n");
                     TEMPLATE(append, vop)(&array_void, (void*)array_char.data); 
                     break;
                default: 
                    ELOG("unknown type");
                    break;
                } 

                TEMPLATE(create, char)(DEFAULT_FIELD_SIZE, &array_char);
                ++curr_field;
            }
            inside = 0;

           // add good to the list
           if (array_void.length == 0) {
               SLOG("Warning: empty line");
               inside = curr_field = 0;
               continue;
           }
           array_void.types = int_types;
           TEMPLATE(add_to_list, dyn_array_vop)(goods, array_void);
           TEMPLATE(create, vop)(types->length, &array_void);
           ++good_num;
           curr_field = 0;
           continue;
        }
        
        // process single field value
        printf("process single symbol in field value, curr_field -> %d\n", curr_field);
        if (c == GOODS_FILE_SEPARATOR) {
            printf("curr_field -> %d\n", curr_field);
            if (inside) {
                int len = TEMPLATE(array_length, char)(&array_char);
                int min, max;
                TEMPLATE(get, int)(min_limit, curr_field, &min);
                TEMPLATE(get, int)(max_limit, curr_field, &max);
                if (len < min || len > max) {
                    char error_msg[ERROR_LENGTH_LIMIT];
                    char* field;
                    TEMPLATE(get, string)(&schema->header->fields, curr_field, &field);
                    sprintf(error_msg, "good #%d has incorrect value in field %s", good_num, field);
                    ELOG(error_msg);
                    TEMPLATE(destroy, char)(&array_char);
                    if (TEMPLATE(size_list, dyn_array_vop)(goods) == 0) {
                        TEMPLATE(destroy2, int)(int_types);
                    }
                    return 1;
                }
               
                int type;
                int* val = (int *)malloc(sizeof(int));
                TEMPLATE(get, int)(int_types, curr_field, &type);
                TEMPLATE(append, char)(&array_char, '\0');
                switch (type) {
                case BOOL:
                     if (get_int_value(&array_char, val)) {
                         char error_msg[ERROR_LENGTH_LIMIT];
                         sprintf(error_msg, "cannot convert %s to bool value", array_char.data);
                         ELOG(error_msg);
                         TEMPLATE(destroy, char)(&array_char);
                         if (TEMPLATE(size_list, dyn_array_vop)(goods) == 0) {
                             TEMPLATE(destroy2, int)(int_types);
                         }
                         return 1;     
                     }
                     TEMPLATE(append, vop)(&array_void, (void *)val);
                     break;
                case INT:
                     if (get_int_value(&array_char, val)) {
                         char error_msg[ERROR_LENGTH_LIMIT];
                         sprintf(error_msg, "cannot convert %s to integer value", array_char.data);
                         ELOG(error_msg);
                         TEMPLATE(destroy, char)(&array_char);
                         if (TEMPLATE(size_list, dyn_array_vop)(goods) == 0) {
                             TEMPLATE(destroy2, int)(int_types);
                         }
                         return 1;
                     }
                     TEMPLATE(append, vop)(&array_void, (void *)val);
                     break;
                case STRING:
                     if (TEMPLATE(shrink_to_fit, char)(&array_char)) {
                         ELOG("cannot shrink input field");
                         TEMPLATE(destroy, char)(&array_char);
                         if (TEMPLATE(size_list, dyn_array_vop)(goods) == 0) {
                             TEMPLATE(destroy2, int)(int_types);
                         }
                         return 1;
                     }

                     TEMPLATE(append, vop)(&array_void, (void*)array_char.data); 
                     break;
                default: 
                    ELOG("unknown type");
                    break;
                } 

                TEMPLATE(create, char)(DEFAULT_FIELD_SIZE, &array_char);
                ++curr_field;
            }
            inside = 0;
        } else if (isalpha(c) || isdigit(c) || c == ' ' || c == '.') {
            inside = 1;
            TEMPLATE(append, char)(&array_char, c);
        } else {
            ELOG("unknown symbol in field");
            TEMPLATE(destroy, char)(&array_char); 
            return 1;
        }
    }
    
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

