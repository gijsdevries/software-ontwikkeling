/** \file
 * Logic layer of application. 
 */ 

#ifndef LOGIC_LAYER_H 
#define LOGIC_LAYER_H

#include <stdbool.h>
#include <stdint.h>

//structs per msg type
typedef struct {
  int x_lup;
  int y_lup;
  int color;
  char *text;
  char *fontname;
  int fontsize;
  int fontstyle;
} text_struct;

typedef struct {
  int x_1;
  int y_1;
  int x_2;
  int y_2;
  int color;
  int weight;
} line_struct;

typedef struct {
  int x;
  int y;
  int width;
  int height;
  int color;
  int filled;
} rectangle_struct;

typedef struct {
  int x_lup;
  int y_lup;
  int bm_nr;
} bitmap_struct;

typedef struct {
  int color;
} clearscreen_struct;

/**
 * @brief writes a color to VGA screen. 
 *
 * This functions writes a predefined color to the entire VGA screen. 
 *
 * @param a clearscreen_struct.
 */

void clearScreenToVGA(clearscreen_struct CS_struct);

void rectangleToVGA(rectangle_struct rectangleStruct);

void lineToVGA(line_struct lineStruct);
int letterToVGA(int x_lup, int y_lup, int color, uint8_t bitmap[5][5], int font_size);
void textToVGA(text_struct textStruct);



#endif
