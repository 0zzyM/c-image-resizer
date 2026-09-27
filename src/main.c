#include <stdio.h>
#include <jpeglib.h>

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

    struct jpeg_decompress_struct image;
    struct jpeg_error_mgr error;

    printf("Image opened successfully.\n");

    image.err = jpeg_std_error(&error);

    jpeg_create_decompress(&image);

    jpeg_stdio_src(&image, file);

    jpeg_read_header(&image, TRUE);



    printf("Width of the img: %u\n", image.image_width);
    printf("Height of the img: %u\n", image.image_height);
    printf("Num of color components img has: %u\n",image.num_components);

    fclose(file);    

    printf("Image closed successfully.\n");

    return 0;
    
}