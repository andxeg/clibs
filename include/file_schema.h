#ifndef FILE_SCHEMA_H
#define FILE_SCHEMA_H

#include "dynamic_arrays.h"

typedef struct  {
    TEMPLATE(DYN_ARRAY, string) fields;
    TEMPLATE(DYN_ARRAY, good_field) types;
    TEMPLATE(DYN_ARRAY, int) length_min;
    TEMPLATE(DYN_ARRAY, int) lenght_max;
} FILE_SCHEMA_GOODS;

/*
typedef struct {
    FILE_SCHEMA_GOODS* file_schema;
    
} FILE_ENTRY;
*/

#endif
