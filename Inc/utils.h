#ifndef UTILS_H
#define UTILS_H


//int string_match(const char* str, const char* pattern);

// cannot separate to *.h and *.c
int string_match(const char* str, const char* pattern) {
    if (*str == '\0' && *pattern == '\0')
        return 1;

    if (*str == *pattern || *pattern == '?') {
        return string_match(str + 1, pattern + 1);
    } else if (*pattern == '*') {
        while (*pattern == '*' && *pattern != '\0')
            ++pattern;

        if (*pattern == '\0')
            return 1;

        int i;
        // you can optimize it using cache
        // cache will be store str, pattern and result of match
        // instead deep recursion
        for (i = 0; i < strlen(str); i++) {
            if (string_match(str+i, pattern))
                return 1;
        }
        return 0;
    }

    return 0;
}

#endif
