#include <string.h>
#include "utils.h"


/*
 * Function check match pattern str or not
 *
 * str: C-style string with '\0' at the end.
 *      str don't contain '*' and '?'
 *
 * pattern: C-style string with '\0' at the end.
 *          Pattern can contain '*' and '?'
 *          '*' match null or more any characters
 *          '?' match one any character
 *
 *  return: 0 - if pattern don't match str
 *          1 - if pattern match str
 *
*/
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
