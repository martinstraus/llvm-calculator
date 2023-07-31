#include <stdio.h>

#define BUFFER_SIZE 1024

int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }
    FILE* file = fopen(argv[1], "r");
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        printf("%s", buffer);
    }
    fclose(file);
    return 0;
}