/**
This component is responsible for implementing the functions declared in image.h.
This includes reading an image from a given PPM file, as well as writing an image to
a given PPM file, and freeing any memory associated with an Image.

@file image.c
@author Matthew Kierski
*/

#include "image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** Length of valid format of PPM file */
#define FORMAT_LENGTH 3

extern Image *readImage(char const *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "%s: No such file or directory\n", filename);
        exit(EXIT_FAILURE);
    }
    char format[FORMAT_LENGTH];
    fscanf(fp, "%3s", format);
    int cols = 0;
    fscanf(fp, "%d", &cols);
    int rows = 0;
    fscanf(fp, "%d", &rows);
    int maxColor = 0;
    fscanf(fp, "%d", &maxColor);
    //Skip newline character before color data
    fgetc(fp);
    
    if (strcmp(format, FORMAT) != 0) {
        fprintf(stderr, "%s", "Invalid image file\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    if (maxColor != MAX_COLOR) {
        fprintf(stderr, "%s", "Invalid image file\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    //Allocate image
    Image *image = (Image *)malloc(sizeof(Image));
    image->rows = rows;
    image->cols = cols;
    image->color = (unsigned char *)malloc(rows * cols * PIXEL_WIDTH * sizeof(unsigned char));
    int bytes = fread(image->color, sizeof(unsigned char), rows * cols * PIXEL_WIDTH, fp);
    if (bytes < rows * cols * PIXEL_WIDTH) {
        fprintf(stderr, "%s", "Invalid image file\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    return image;
}

extern void writeImage(Image *image, char const *filename)
{
    FILE *fp = fopen(filename, "wb");
    fprintf(fp, "%s\n", FORMAT);
    fprintf(fp, "%d %d\n", image->cols, image->rows);
    fprintf(fp, "%d\n", MAX_COLOR);
    fwrite(image->color, sizeof(unsigned char), image->cols * image->rows * PIXEL_WIDTH, fp);

    fclose(fp);
}

extern void freeImage(Image *image)
{
    free(image->color);
    free(image);
}
