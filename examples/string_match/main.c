#include <time.h>
#include <stdio.h>
#include <string.h>

#define STRING_MAX_LEN 1024
#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

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
    // printf("string: %20s, pattern: %20s\n", str, pattern);
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
    } else {
        return 0;
    }

    return 0;
}

int file_read_check(FILE* file, int iRet) {
    return iRet == 0 || ferror(file);
}

int check_string(char* str, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (str[i] == '*' || str[i] == '?') {
            return 1;
        }
    }
    return 0;
}


typedef struct {
    char* str;
    char* pattern;
    char result;
} TEST_CASE;

int test_from_input() {
    int iRet = 0;
    char str[STRING_MAX_LEN];
    char pattern[STRING_MAX_LEN];
    
    printf("Write string:\n");
    iRet = scanf("%s", str);
    if (file_read_check(stdin, iRet)) {
        fprintf(stderr, "cannot read string\n");
        return 1;
    }

    if (check_string(str, strlen(str))) {
        fprintf(stderr, "string cannot contain '*' and '?'\n");
    }
    
    printf("Write pattern:\n");
    iRet = scanf("%s", pattern);
    if (file_read_check(stdin, iRet)) {
        fprintf(stderr, "cannot read pattern\n");
        return 1;
    }

    if (string_match(str, pattern)) {
        printf("'%s' match '%s'\n", pattern, str);
    } else {
        printf("'%s' don't match '%s'\n", pattern, str);
    }

    return 0;
}

int main(int argc, char** argv) {
    TEST_CASE test_cases[] = {
        {"aabrashabra", "?ab*sh*?", 1},
        {"geeks", "g*ks", 1},
        {"geeksforgeeks*", "ge?ks*", 1},
        {"gee", "g*k", 0},
        {"pqrst", "*pqrs", 0},
        {"abcdhghgbcd", "abc*bcd", 1},
        {"abcd", "abc*c?d", 0},
        {"abcd", "*c*d", 1},
        {"abcd", "*?c*d", 1},
        {"abcccd", "*ccd", 1},
        {"mississipissippi", "*issip*ss*", 1},
        {"xxxx*zzzzzzzzy*f", "xxxx*zzy*fffff", 0},
        {"xxxx*zzzzzzzzy*f", "xxx*zzy*f", 1},
        {"xxxxzzzzzzzzyf", "xxxx*zzy*fffff", 0},
        {"xxxxzzzzzzzzyf", "xxxx*zzy*f", 1},
        {"xyxyxyzyxyz", "xy*z*xyz", 1},
        {"mississippi", "*sip*", 1},
        {"xyxyxyxyz", "xy*xyz", 1},
        {"mississippi", "mi*sip*", 1},
        {"ababac", "*abac*", 1},
        {"ababac", "*abac*", 1},
        {"aaazz", "a*zz*", 1},
        {"a12b12", "*12*23", 0},
        {"a12b12", "a12b", 0},
        {"a12b12", "*12*12*", 1},

        // More double wildcard scenarios.
        {"XYXYXYZYXYz", "XY*Z*XYz", 1},
        {"missisSIPpi", "*SIP*", 1},
        {"mississipPI", "*issip*PI", 1},
        {"xyxyxyxyz", "xy*xyz", 1},
        {"miSsissippi", "mi*sip*", 1},
        {"miSsissippi", "mi*Sip*", 0},
        {"abAbac", "*Abac*", 1},
        {"abAbac", "*Abac*", 1},
        {"aAazz", "a*zz*", 1},
        {"A12b12", "*12*23", 0},
        {"a12B12", "*12*12*", 1},
        {"oWn", "*oWn*", 1},

        // Completely tame {no wildcards} cases.
        {"bLah", "bLah", 1},
        {"bLah", "bLaH", 0},

        // Simple mixed wildcard tests suggested by IBMer Marlin Deckert.
        {"a", "*?", 1},
        {"ab", "*?", 1},
        {"abc", "*?", 1},

        // More mixed wildcard tests including coverage for 0 positives.
        {"a", "??", 0},
        {"ab", "?*?", 1},
        {"ab", "*?*?*", 1},
        {"abc", "?**?*?", 1},
        {"abc", "?**?*&?", 0},
        {"abcd", "?b*??", 1},
        {"abcd", "?a*??", 0},
        {"abcd", "?**?c?", 1},
        {"abcd", "?**?d?", 0},
        {"abcde", "?*b*?*d*?", 1},

        // Single-character-match cases.
        {"bLah", "bL?h", 1},
        {"bLaaa", "bLa?", 0},
        {"bLah", "bLa?", 1},
        {"bLaH", "?Lah", 0},
        {"bLaH", "?LaH", 1},

        // Many-wildcard scenarios.
        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
        aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab", 
            "a*a*a*a*a*a*aa*aaa*a*a*b", 1},
        {"abababababababababababababababababababaacacacacaca\
        cacadaeafagahaiajakalaaaaaaaaaaaaaaaaaffafagaagggagaaaaaaaab", 
            "*a*b*ba*ca*a*aa*aaa*fa*ga*b*", 1},
        {"abababababababababababababababababababaacacacacaca\
        cacadaeafagahaiajakalaaaaaaaaaaaaaaaaaffafagaagggagaaaaaaaab", 
            "*a*b*ba*ca*a*x*aaa*fa*ga*b*", 0},
        {"abababababababababababababababababababaacacacacaca\
        cacadaeafagahaiajakalaaaaaaaaaaaaaaaaaffafagaagggagaaaaaaaab", 
            "*a*b*ba*ca*aaaa*fa*ga*gggg*b*", 0},
        {"abababababababababababababababababababaacacacacaca\
        cacadaeafagahaiajakalaaaaaaaaaaaaaaaaaffafagaagggagaaaaaaaab", 
            "*a*b*ba*ca*aaaa*fa*ga*ggg*b*", 1},
        {"aaabbaabbaab", "*aabbaa*a*", 1},
        {"aaaaaaaaaaaaaaaaa", 
            "*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*", 1},
        {"aaaaaaaaaaaaaaaa", 
            "*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*", 0},
        {"abc", "********a********b********c********", 
            1},
        {"abc", "********a********b********b********", 
            0},
        {"abc", "***a*b*c***", 1}
    };
    
    int i;
    int iRet;
    int passed = 0;
    double delta = 0.0;
    struct timespec mt1, mt2;
    for (i = 0; i < ARRAY_LEN(test_cases); i++) {
        TEST_CASE test_case = test_cases[i];
        if (check_string(test_case.str, strlen(test_case.str))) {
            printf("Test #%d was skipped. '%s' contains '*' or '?'\n",
                   i + 1, test_case.str); 
        }
        printf("Launch test #%d: string: %s, pattern: %s\n", 
                i + 1, test_case.str, test_case.pattern);
        clock_gettime(CLOCK_REALTIME, &mt1);
        iRet = string_match(test_case.str, test_case.pattern);
        clock_gettime(CLOCK_REALTIME, &mt2);
        delta = (mt2.tv_sec - mt1.tv_sec) + ((double)(mt2.tv_nsec - mt1.tv_nsec) / 1000000000);
        if (iRet != test_case.result) {
            printf("Test #%d was failed\n\n", i + 1);
        } else {
            ++passed;
            printf("Test #%d was passed. Time: %f\n\n", i + 1, delta);
        }
    }

    printf("TOTAL: %ld, PASSED: %d, FAILED: %ld\n",
           ARRAY_LEN(test_cases), passed, ARRAY_LEN(test_cases) - passed);
    return 0;
}

