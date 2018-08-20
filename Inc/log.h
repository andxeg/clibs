#ifndef LOG_H
#define LOG_H

#include "Goal.h"
#include <GL_File.h>
#include <stdio.h>

extern T_GL_HFILE error_log_file;
extern T_GL_HFILE log_file;

#define SLOG(msg) \
    _log(log_file, __FILE__, __LINE__, __func__, msg)

#define ELOG(msg) \
    _log(error_log_file, __FILE__, __LINE__, __func__, msg)

#define PRINT_TO_LOG(msg) \
	print_to_log(log_file, msg)

#define PRINT_TO_ERROR_LOG(msg) \
	print_to_log(error_log_file, msg)

void _log(T_GL_HFILE fd, const char* file, int line, const char* func, const char* msg);

void print_message(T_GL_HGRAPHIC_LIB hGraphicLib, const char* message);
void print_to_log(T_GL_HFILE fd, char* message);
void print_to_error_log(T_GL_HFILE fd, char* message);

#endif
