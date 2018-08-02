#include <stdio.h>
#include <stdlib.h>
#include "file_schema.h"


FILE_SCHEMA* create_file_schema() {
    FILE_SCHEMA* schema = (FILE_SCHEMA *) malloc(sizeof(FILE_SCHEMA));

    schema->header = (FILE_HEADER *) malloc(sizeof(FILE_HEADER));
    TEMPLATE(create, string)(INITIAL_FIELDS_COUNT, &schema->header->fields);
    TEMPLATE(create, good_field)(INITIAL_FIELDS_COUNT, &schema->header->types);
    TEMPLATE(create, int)(INITIAL_FIELDS_COUNT, &schema->header->length_min);
    TEMPLATE(create, int)(INITIAL_FIELDS_COUNT, &schema->header->length_max);
 
    schema->goods = TEMPLATE(create_list, dyn_array_vop)();
    
    return schema; 
}

void destroy_file_schema(FILE_SCHEMA* schema) {
    TEMPLATE(destroy_list, dyn_array_vop)(schema->goods);
    TEMPLATE(destroy, string)(&schema->header->fields);
    TEMPLATE(destroy, good_field)(&schema->header->types);
    TEMPLATE(destroy, int)(&schema->header->length_min);
    TEMPLATE(destroy, int)(&schema->header->length_max);
    free(schema->header);
    free(schema);
}

void print_file_schema(FILE_SCHEMA* schema) {
    printf("==========FILE HEADER==========\n");
    printf("------------FIELDS-------------\n");
    TEMPLATE(print, string)(&schema->header->fields);
    printf("--------------TYPES------------\n");
    TEMPLATE(print, good_field)(&schema->header->types);
    printf("------------LENGTH_MIN---------\n");
    TEMPLATE(print, int)(&schema->header->length_min);
    printf("------------LENGTH_MAX---------\n");
    TEMPLATE(print, int)(&schema->header->length_max);

    printf("==============GOODS============\n");
    TEMPLATE(print_list, dyn_array_vop)(schema->goods);
}

