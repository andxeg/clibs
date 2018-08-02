#include <stdio.h>
#include <stdlib.h>
#include "lists.h"
#include "iofile.h"
#include "file_schema.h"

#define DEFAULT_FILENAME "./test_data/goods.txt"

int main(int argc, char** argv) {
    FILE_SCHEMA* schema = create_file_schema();
    if (argc > 2) {
        fprintf(stderr, "error in input parameters\n");
        fprintf(stderr, "type %s <file with goods>\n", argv[0]);
    }
    
    char* filename = (argc == 2) ? argv[1] : DEFAULT_FILENAME;

    if (read_file_with_goods(filename, schema)) {
        return 1;
    }
    
    destroy_file_schema(schema);   
    return 0;
}
