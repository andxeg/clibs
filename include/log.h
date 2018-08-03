#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define SLOG(msg) \
    _log(stdout, __FILE__, __LINE__, __func__, msg)

#define ELOG(msg) \
    _log(stderr, __FILE__, __LINE__, __func__, msg)

void _log(FILE* fd, const char* file, int line, const char* func,           const char* msg) {
    fprintf(fd, "[%s:%d] In function %s: %s\n", file, line, func, msg);
}

#endif

