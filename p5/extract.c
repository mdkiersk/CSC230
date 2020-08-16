/**
 * Serves as the starting point of the extract program. It is responsible for handling the command
 * line arguments and extracting the message from the input image, outputting it to the given text
 * file.
 *
 * @file extract.c
 * @author Matthew Kierski
 */

#include "bits.h"
#include "image.h"
#include <stdio.h>
#include <stdlib.h>

/** Number of bits in a byte */
#define BYTE 8
/** Denotes usage of argv[1] */
#define INPUT_IMAGE argv[1]
/** Denotes usage of argv[2] */
#define OUTPUT_MESSAGE argv[2]
/** Denotes usage of argv[3] */
#define NUM_BITS argv[3]
/** Valid number of command-line arguments */
#define VALID_ARG_NUM 4

/**
 * Starting point of our extract program. This function will deal with command-lne arguments,
 * printing error messages and exiting if any input is invalid. If valid, it will extract the
 * message from the given image file, and output the text message to the given text file.
 *
 * @param argc number of command line arguments
 * @param *argv[] pointer to array of command line arguments
 * @return exit success
 */
int main(int argc, char *argv[])
{
    if (argc != VALID_ARG_NUM) {
        fprintf(stderr, "%s\n", "usage: extract <input-image> <output-message> <bits>");
        exit(EXIT_FAILURE);
    }
    int bits = atoi(NUM_BITS);
    if (bits < 1 || bits > BYTE) {
        fprintf(stderr, "%s\n", "Invalid number of bits");
        exit(EXIT_FAILURE);
    }

    Image *image = readImage(INPUT_IMAGE);
    int size = bits * image->cols * image->rows * PIXEL_WIDTH;
    char *message = (char *)malloc((size + 1) * sizeof(char));
    //Number of chars processed
    static int charsProcessed = 0;
    //Bits processed in each pixel so far
    static int bitsProcessed = 0;
    //Total number of pixels processed so far
    static int pixelsProcessed = 0;
    //While there are pixels to process ...
    while (pixelsProcessed < image->cols * image->rows * PIXEL_WIDTH) {
        for (int i = 0; i < BYTE; i++) {
            message[charsProcessed] = putBit(message[charsProcessed], i,
                getBit(image->color[pixelsProcessed], bitsProcessed));
            bitsProcessed++;
            if (bitsProcessed == bits) {
                pixelsProcessed++;
                bitsProcessed = 0;
            }
        }
        charsProcessed++;
    }
    message[charsProcessed] = '\0';
    //Reading complete; print message to output file
    FILE *fp = fopen(OUTPUT_MESSAGE, "w");
    fprintf(fp, "%s", message);

    //Free memory
    freeImage(image);
    free(message);
    fclose(fp);
}
