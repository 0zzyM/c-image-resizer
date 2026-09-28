#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Please provide path of a single img.\n");
        return 1;
    }

    // fopen() returns a pointer to a FILE object managed in memory.
    // This FILE object represents the open file; it does not contain the whole image.
    // The actual image data stays on disk until we read it.
    FILE *file = fopen(argv[1], "rb");

    // fopen() returns NULL if it cannot open the file
    if (file == NULL)
    {
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
    printf("Num of color components img has: %d\n", image.num_components);

    jpeg_start_decompress(&image);

    size_t buffer_size =
        image.output_width *
        image.output_height *
        image.output_components;

    unsigned char *buffer = malloc(buffer_size);

    if (buffer == NULL)
    {
        printf("Failed to allocate image buffer.\n");

        jpeg_destroy_decompress(&image);
        fclose(file);
        return 1;
    }

    size_t row_size = image.output_width * image.output_components;

    while (image.output_scanline < image.output_height)
    {
        JSAMPROW row_pointer[1];
        row_pointer[0] =
            buffer + image.output_scanline * row_size;

        jpeg_read_scanlines(&image, row_pointer, 1);
    }

    printf("First pixel values: %u %u %u\n",
           buffer[0],
           buffer[1],
           buffer[2]);

    struct jpeg_compress_struct output;
    struct jpeg_error_mgr output_error;

    output.err = jpeg_std_error(&output_error);

    jpeg_create_compress(&output);

    FILE *output_file = fopen("output.jpg", "wb");

    if (output_file == NULL)
    {
        printf("Failed to create output image.\n");

        jpeg_destroy_compress(&output);
        free(buffer);
        jpeg_destroy_decompress(&image);
        fclose(file);

        return 1;
    }

    jpeg_stdio_dest(&output, output_file);

    output.image_width = image.output_width;
    output.image_height = image.output_height;
    output.input_components = image.output_components;
    output.in_color_space = JCS_RGB;

    jpeg_set_defaults(&output);

    jpeg_start_compress(&output, TRUE);

    while (output.next_scanline < output.image_height)
    {
        JSAMPROW row_pointer[1];

        row_pointer[0] =
            buffer + output.next_scanline * row_size;

        jpeg_write_scanlines(&output, row_pointer, 1);
    }

    jpeg_finish_compress(&output);

    fclose(output_file);
    jpeg_destroy_compress(&output);

    free(buffer);
    jpeg_destroy_decompress(&image);
    fclose(file);

    printf("Image closed successfully.\n");

    return 0;
}