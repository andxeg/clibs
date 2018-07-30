#ifndef FILE_SCHEMA_H
#define FILE_SCHEMA_H

#include "dynamic_arrays.h"

typedef struct {
    TEMPLATE(DYN_ARRAY, string) fields;
    TEMPLATE(DYN_ARRAY, good_field) types;
    TEMPLATE(DYN_ARRAY, int) length_min;
    TEMPLATE(DYN_ARRAY, int) lenght_max;
} FILE_HEADER;

typedef struct {
    TEMPLATE(DYN_ARRAY, vop) field_values;
    struct GOOD *next;
    struct GOOD *prev;
} GOOD;

typedef GOOD GOOD_LIST;

typedef struct {
    FILE_HEADER *header;
    GOOD_LIST *goods;   
} FILE_SCHEMA;

#endif
