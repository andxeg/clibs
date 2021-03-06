#ifndef IOFILE_H
#define IOFILE_H 

#include <stdio.h>
#include "file_schema.h"

int read_file_with_goods(const char* filename, FILE_SCHEMA* schema);
int read_file_header(FILE* file, FILE_SCHEMA* schema);
int read_fields(FILE* file, FILE_SCHEMA* schema);
int read_types(FILE* file, FILE_SCHEMA* schema);
int read_limits(FILE* file, TEMPLATE(DYN_ARRAY, int)* limits);
int read_goods(FILE* file, FILE_SCHEMA* schema);

int write_file_with_goods(const char* filename, FILE_SCHEMA* schema);
int write_file_header(FILE* file, FILE_SCHEMA* schema);
int write_goods(FILE* file, FILE_SCHEMA* schema);

#endif

