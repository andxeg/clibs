#ifndef FILE_SCHEMA_H
#define FILE_SCHEMA_H

#include "defs.h"
#include "lists.h"
#include "dynamic_arrays.h"

typedef struct {
    TEMPLATE(DYN_ARRAY, string) fields;
    TEMPLATE(DYN_ARRAY, good_field) types;
    TEMPLATE(DYN_ARRAY, int) length_min;
    TEMPLATE(DYN_ARRAY, int) length_max;
} FILE_HEADER;

typedef struct {
    FILE_HEADER* header;
    TEMPLATE(LIST, dyn_array_vop)* goods;
} FILE_SCHEMA;


FILE_SCHEMA* create_file_schema();
void destroy_file_schema(FILE_SCHEMA* schema);
void print_file_schema(FILE_SCHEMA* schema);

#endif

