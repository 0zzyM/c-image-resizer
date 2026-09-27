#include <stdio.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Please provide path of a single img.\n");
        return 1;
    }

    // fopen() returns a pointer to a FILE object managed in memory.
    // This FILE object represents the open file; it does not contain the whole image.
    // The actual image data stays on disk until we read it.
    FILE *file = fopen(argv[1], "rb");


    //fopen() returns NULL if it cannot open the file
    if (file == NULL) {
        printf("Something went wrong, failed to open the image.\n");
        return 1;
    }

    printf("Image opened successfully.\n");

    fclose(file);    

    printf("Image closed successfully.\n");

    return 0;
    
}