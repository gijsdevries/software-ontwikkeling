/** \file
 * Logic layer of application. 
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

enum FONTGROOTTE {
  GROOT,
  KLEIN
};


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

void textToVGA(text_struct textStruct);

#endif
