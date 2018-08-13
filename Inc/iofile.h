#ifndef IOFILE_H
#define IOFILE_H

#include <GL_Types.h>
#include <stdio.h>
#include "file_schema.h"


int read_file_with_goods(const char* filename, FILE_SCHEMA* schema, T_GL_HGRAPHIC_LIB hGraphicLib);
int file_type(T_GL_HFILE file, T_GL_HGRAPHIC_LIB hGraphicLib);
int assign_schema_header(FILE_SCHEMA* schema, T_GL_HGRAPHIC_LIB hGraphicLib);
int read_file_header(T_GL_HFILE file, FILE_SCHEMA* schema, T_GL_HGRAPHIC_LIB hGraphicLib);
int read_fields(T_GL_HFILE file, FILE_SCHEMA* schema, T_GL_HGRAPHIC_LIB hGraphicLib);
int read_types(T_GL_HFILE file, FILE_SCHEMA* schema, T_GL_HGRAPHIC_LIB hGraphicLib);
int read_limits(T_GL_HFILE file, TEMPLATE(DYN_ARRAY, int)* limits, T_GL_HGRAPHIC_LIB hGraphicLib);
int read_goods(T_GL_HFILE file, FILE_SCHEMA* schema, T_GL_HGRAPHIC_LIB hGraphicLib);

int write_file_with_goods(const char* filename, FILE_SCHEMA* schema);
int write_file_header(T_GL_HFILE file, FILE_SCHEMA* schema);
int write_goods(T_GL_HFILE file, FILE_SCHEMA* schema);

#endif

