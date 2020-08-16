/**
    @file image.h
    @author Matthew Kierski

    Header for the image component, with a representation for an Image
    and functions for reading and writing images in PPM format.
*/

#ifndef _IMAGE_H_
#define _IMAGE_H_

/** PPM file format string */
#define FORMAT "P6"

/** Maximum color value */
#define MAX_COLOR 255

/** Number of intensity values per pixel. */
#define PIXEL_WIDTH 3

/** Representation for image file data */
typedef struct {
  /** number of rows */
  int rows;
  
  /** pixels per row */
  int cols;
  
  /** Dynamically allocated pixel data, rows * cols pixels, each with
      3 intensities (RGB), stored in row-major order. */
  unsigned char *color;
} Image;

/**
 * Dynamically allocates an instance of an image and populates it based on the given
 * PPM file. If a problem occurs, it prints an appropraite error message and terminates
 * the program.
 * @param filename name of PPM file containing image
 * @return an instance of the new image
 */
Image *readImage(char const *filename);

/**
 * Writes out the given image in raw PPM format to a given file. It will
 * print an error message and terminate if the given file cannot be opened.
 * @param image image to write out in binary
 * @param filename name of file to write to
 */
void writeImage(Image *image, char const *filename);

/**
 * Frees the memory associated with the given Image.
 * @param image image to free
 */
void freeImage(Image *image);

#endif
