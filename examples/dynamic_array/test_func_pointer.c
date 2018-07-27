#include <stdio.h>
#include <stdlib.h>

typedef void (*func)(int);

void print(int a) {
    printf("%d\n", a);
}

int main(int argc, char * * argv) {
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    
    func f = print;
    f(1);
    f(2);
    return 0;
}

