/**
 * Serves as the starting point of the conceal program. It is responsible for handling command
 * line arguments, reading the image and message files, hiding bits of the message in the image
 * and writing out the resulting image file (using functions in bit.c and image.c).
 *
 * @file conceal.c
 * @author Matthew Kierski
 */

#include "bits.h"
#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Denotes valid number of command line arguments */
#define VALID_ARG_NUM 5
/** Denotes number of bits in a byte */
#define BYTE 8
/** Denotes usage of argv[1] */
#define MESSAGE argv[1]
/** Denotes usage of argv[2] */
#define INPUT_IMAGE argv[2]
/** Denotes usage of argv[3] */
#define OUTPUT_IMAGE argv[3]
/** Denotes usage of argv[4] */
#define NUM_BITS argv[4]

/**
 * Starting point of the conceal program. This function will deal with command-line arguments,
 * printing error messages and exiting if any input is invalid. If valid, it will conceal the
 * given message into the given image file, outputting the result to a new image file.
 *
 * @param argc number of command line arguments
 * @param *argv[] pointer to array of command line arguments
 * @return exit success
 */
int main(int argc, char *argv[])
{
    //Determine if command line arguments are valid
    if (argc != VALID_ARG_NUM) {
        fprintf(stderr, "%s\n",
            "usage: conceal <input-message> <input-image> <output-image> <bits>");
        exit(EXIT_FAILURE);
    }
    int bits = atoi(NUM_BITS);
    if (bits < 1 || bits > BYTE) {
        fprintf(stderr, "%s\n", "Invalid number of bits");
        exit(EXIT_FAILURE);
    }
    //Open message file and save it to array
    FILE *fp = fopen(MESSAGE, "r");
    if (!fp) {
        fprintf(stderr, "%s: No such file or directory\n", MESSAGE);
        exit(EXIT_FAILURE);
    }
    //Find the buffer size needed, allocating appropriate space and reading message into array
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    rewind(fp);
    char *message = (char *)malloc((size + 1) * sizeof(char));
    fread(message, size, 1, fp);
    message[size] = '\0';
    fclose(fp);

    //Read image and determine if we have enough bits to conceal message
    Image *image = readImage(INPUT_IMAGE);
    int length = strlen(message);
    int bitsNeeded = length * BYTE;
    int bitsAvail = image->cols * image->rows * bits * PIXEL_WIDTH;
    if (bitsAvail < bitsNeeded) {
        fclose(fp);
        fprintf(stderr, "%s", "Too few pixels to conceal image");
        exit(EXIT_FAILURE);
    }
    
    //Arguments are valid; proceed to concealing message
    static int charsProcessed = 0;
    //Bits processed in each pixel so far
    static int bitsProcessed = 0;
    //Total number of pixels processed so far
    static int pixelsProcessed = 0;
    //While there are characters to process ...
    while (charsProcessed < length) {
        for (int i = 0; i < BYTE; i++) {
            image->color[pixelsProcessed] = putBit(image->color[pixelsProcessed],
                bitsProcessed, getBit(message[charsProcessed], i));
            bitsProcessed++;
            if (bitsProcessed == bits) {
                pixelsProcessed++;
                bitsProcessed = 0;
            }
        }
        charsProcessed++;
    }
    
    //Pad remaining lower order bits with 0's
    for (int i = pixelsProcessed; i < image->cols * image->rows * PIXEL_WIDTH; i++) {
        for (int n = 0; n < bits; n++) {
            image->color[i] = putBit(image->color[i], n, 0);
        }
    }
    //Message concealed, write new image
    writeImage(image, OUTPUT_IMAGE);

    //Free allocated space and exit
    free(message);
    freeImage(image);
    exit(EXIT_SUCCESS);
}
