#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define LOAD_COUNT_OFFSET 5

int print_curr_time(FILE* file) {
    time_t raw_time;
    struct tm* time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);
    return (fprintf(file, "%s", asctime(time_info)) != 1) ? 1 : 0;
}

int file_empty(FILE* file) {
    if (file == NULL) 
        return -1;

    long size;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    return size == 0;
}

int clear_file(FILE* file) {
    long size;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    
    char* spaces = (char *) malloc(size);
    if (spaces == NULL)
        return 1;

    memset(spaces, ' ', size);
    fwrite(spaces, 1, size, file);
    rewind(file);
    return 0;
}

int convert_to_long(const char* str, long* result) {
    char* end;
    *result = strtol(str+LOAD_COUNT_OFFSET, &end, 10);
    if (*end == '\n')
        ++end;
    if (*end != '\0') return 1;
    return 0;
}

int update_reboot_info(FILE* file, const char* filename) {
    char time_created[MAX_LINE];
    char time_last_update[MAX_LINE];
    char load_count[MAX_LINE];

    rewind(file); 
    fgets(time_created, MAX_LINE, file);
    fgets(time_last_update, MAX_LINE, file);
    fgets(load_count, MAX_LINE, file);
    
    printf("Time created: %s", time_created);
    printf("Last update: %s", time_last_update);
    printf("Load count: %s", load_count);
    
    long count;
    if (convert_to_long(load_count, &count)) {
        fprintf(stderr, "Cannot convert load count to number\n");
        return 1;
    }
    
    clear_file(file);
    fprintf(file, "%s", time_created);
    print_curr_time(file);
    fprintf(file, "Load: %ld\n", count + 1);
    return 0;
}

int create_reboot_info(FILE* file) {
    rewind(file);
    print_curr_time(file);
    print_curr_time(file);
    fprintf(file, "Load: %ld\n", 1L);
    return 0;
}

int add_reboot_info(const char* filename) {
    FILE* file = fopen(filename, "r+");
    if (file == NULL) {
        fprintf(stderr, "cannot open file '%s'\n", filename);
        return 1;
    }
    
    if (file_empty(file) == 0) {
        printf("File is not empty. Update reboot info\n");
        update_reboot_info(file, filename);
    } else {
        printf("File is empty. Create reboot info.\n");
        create_reboot_info(file);
    }

    printf("DONE\n");
    fclose(file);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "error in input parameters\n");
        printf("Type %s <file for reboot info>\n", argv[0]);
        exit(127);
    }

    add_reboot_info(argv[1]);
    return 0;
}

