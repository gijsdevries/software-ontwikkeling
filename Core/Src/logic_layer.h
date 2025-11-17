#ifndef LOGIC_LAYER_H 
#define LOGIC_LAYER_H

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

struct {
  int x;
  int y;
  int width;
  int height;
  int color;
  int filled;
} rectangle_struct;

struct {
  int x_lup;
  int y_lup;
  int bm_nr;
} bitmap_struct;

typedef struct {
  int color;
} clearscreen_struct;

//function prototypes
void clearScreenToBitmap(clearscreen_struct clearScreenStruct, unsigned char bitmap[320][240][3]);

#endif
