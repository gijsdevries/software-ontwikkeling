/** \file
 * Logic layer of application. 
 * @brief Dit bestand bevat de declaraties voor de logic laag.
 */

#ifndef LOGIC_LAYER_H 
#define LOGIC_LAYER_H

#include <global.h>
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
  char *fontstyle;
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
  int FONTNAAM;
  int FONTSTIJL;
  int FONTGROOTTE;
} textInfo;

typedef struct {
  int color;
} clearscreen_struct;

enum FONTNAMES {
  ARIAL,
  CONSOLAS
};

enum FONTSTYLE {
  NORMAAL,
  VET,
  CURSIEF
};


#define GROOT 2
#define KLEIN 1

#define LETTER_MARGE_SMALL 8

void clearScreenToVGA(clearscreen_struct CS_struct);

void rectangleToVGA(rectangle_struct rectangleStruct);

void lineToVGA(line_struct lineStruct);

void textToVGA(text_struct textStruct);

void bitmapToVGA (bitmap_struct bitmapStruct);

#endif
