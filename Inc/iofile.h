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

int write_file_schema_to_file(const char* filename, FILE_SCHEMA* schema, int file_type, T_GL_HGRAPHIC_LIB hGraphicLib);
int write_header_to_file(T_GL_HFILE file, FILE_SCHEMA* schema, T_GL_HGRAPHIC_LIB hGraphicLib);
int write_fields_to_file(T_GL_HFILE file, FILE_SCHEMA* schema, T_GL_HGRAPHIC_LIB hGraphicLib);
int write_types_to_file(T_GL_HFILE file, FILE_SCHEMA* schema, T_GL_HGRAPHIC_LIB hGraphicLib);
int write_limits_to_file(T_GL_HFILE file, FILE_SCHEMA* schema, T_GL_HGRAPHIC_LIB hGraphicLib);
int write_goods_to_file(T_GL_HFILE file, FILE_SCHEMA* schema, T_GL_HGRAPHIC_LIB hGraphicLib);

#endif

