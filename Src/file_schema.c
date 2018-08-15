#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "log.h"
#include "file_schema.h"


FILE_SCHEMA* create_file_schema() {
    FILE_SCHEMA* schema = (FILE_SCHEMA *) umalloc(sizeof(FILE_SCHEMA));

    schema->header = (FILE_HEADER *) umalloc(sizeof(FILE_HEADER));
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
    ufree(schema->header);
    ufree(schema);
}

void print_file_schema(FILE_SCHEMA* schema) {
	char msg[LOG_MSG_LENGTH_LIMIT];
    sprintf(msg, "==========FILE HEADER==========\n"); PRINT_TO_LOG(msg);
    sprintf(msg, "------------FIELDS-------------\n"); PRINT_TO_LOG(msg);
    TEMPLATE(print, string)(&schema->header->fields);
    sprintf(msg, "--------------TYPES------------\n"); PRINT_TO_LOG(msg);
    TEMPLATE(print, good_field)(&schema->header->types);
    sprintf(msg, "------------LENGTH_MIN---------\n"); PRINT_TO_LOG(msg);
    TEMPLATE(print, int)(&schema->header->length_min);
    sprintf(msg, "------------LENGTH_MAX---------\n"); PRINT_TO_LOG(msg);
    TEMPLATE(print, int)(&schema->header->length_max);

    sprintf(msg, "==============GOODS============\n"); PRINT_TO_LOG(msg);
    TEMPLATE(print_list, dyn_array_vop)(schema->goods);
}

int is_empty_file_schema(FILE_SCHEMA* schema) {
	return TEMPLATE(size_list, dyn_array_vop)(schema->goods) == 0 &&
		   TEMPLATE(array_length, string)(&schema->header->fields) == 0 &&
		   TEMPLATE(array_length, good_field)(&schema->header->types) == 0 &&
		   TEMPLATE(array_length, int)(&schema->header->length_min) == 0 &&
		   TEMPLATE(array_length, int)(&schema->header->length_max) == 0;
}
