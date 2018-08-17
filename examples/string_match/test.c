#include <stdio.h>
#include <string.h>
#include <time.h> // clock_gettime

int main() {
    struct timespec mt1, mt2;
    long int tt;
    double tt_sec;
    clock_gettime(CLOCK_REALTIME, &mt1);
    printf("Seconds: %ld\n", mt1.tv_sec);
    printf("Nanoseconds: %ld\n", mt1.tv_nsec);
    unsigned long long sum = 0;
    for (int i = 0; i < 1000; i++) {
        ++sum;
    }

    clock_gettime(CLOCK_REALTIME, &mt2);
    tt = 1000000000 * (mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec);
    printf("%ld nanoseconds were passed\n", tt);
    tt_sec = tt / 1000000000.0;
    printf("%f seconds were passed\n", tt_sec);
    return 0;
}

