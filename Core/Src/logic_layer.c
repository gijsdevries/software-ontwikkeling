#include <stdio.h>
//#include "logic_layer.h"

typedef struct {
  int color;
} clearscreen_struct;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;

enum colors {
  BLACK,
  BLUE,
  LIGHT_BLUE,
  GREEN,
  LIGHT_GREEN,
  CYAN,
  LIGHT_CYAN,
  RED,
  LIGHT_RED,
  MAGENTA,
  LIGHT_MAGENTA,
  BROWN,
  YELLOW,
  GREY,
  WHITE
};

#define BLACK_RGB         (RGB){0, 0, 0}
#define BLUE_RGB          (RGB){0, 0, 255}
#define LIGHT_BLUE_RGB    (RGB){0, 0, 0}
#define GREEN_RGB         (RGB){0, 255, 0}
#define LIGHT_GREEN_RGB   (RGB){0, 0, 0}
#define CYAN_RGB          (RGB){0, 0, 0}
#define LIGHT_CYAN_RGB    (RGB){0, 0, 0}
#define RED_RGB           (RGB){255, 0, 0}
#define LIGHT_RED_RGB     (RGB){0, 0, 0}
#define MAGENTA_RGB       (RGB){0, 0, 0}
#define LIGHT_MAGENTA_RGB (RGB){0, 0, 0}
#define BROWN_RGB         (RGB){0, 0, 0}
#define YELLOW_RGB        (RGB){0, 0, 0}
#define GREY_RGB          (RGB){0, 0, 0}
#define WHITE_RGB         (RGB){255, 255, 255}


unsigned char* clearScreenToBitmap(clearscreen_struct CS_struct);

int main() {
  clearscreen_struct clearScreenStruct;
  clearScreenStruct.color = WHITE;

  char (*bmpPtr)[240][3] = clearScreenToBitmap(clearScreenStruct);

  //debug
  for (int y = 0; y < 240; y++) {
    for (int x = 0; x < 320; x++) {
      unsigned char r = bmpPtr[x][y][0];
      unsigned char g = bmpPtr[x][y][1];
      unsigned char b = bmpPtr[x][y][2];

      printf("(%3d,%3d,%3d) ", r, g, b);
    }
   printf("\n");
  }
  return 0;
}

unsigned char* clearScreenToBitmap(clearscreen_struct CS_struct) {
  unsigned char bitmap[320][240][3];
  unsigned char *pbitmap = bitmap;
  unsigned char drawColors[3];
  RGB c;

  switch (CS_struct.color)
  {
    case BLACK:
      printf("recieved black\n");
      c = BLACK_RGB;
      drawColors[2] = c.b; drawColors[1] = c.g; drawColors[0] = c.r;
      break; 
    case BLUE:
      printf("recieved blue\n");
      c = BLUE_RGB;
      drawColors[2] = c.b; drawColors[1] = c.g; drawColors[0] = c.r;
      break;
    case RED:
      printf("recieved black\n");
      c = RED_RGB;
      drawColors[2] = c.b; drawColors[1] = c.g; drawColors[0] = c.r;
      break;
    case GREEN:
      printf("recieved black\n");
      c = GREEN_RGB;
      drawColors[2] = c.b; drawColors[1] = c.g; drawColors[0] = c.r;
      break;
    case WHITE:
      printf("recieved black\n");
      c = WHITE_RGB;
      drawColors[2] = c.b; drawColors[1] = c.g; drawColors[0] = c.r;
      break;
   default:
      printf("recieved invalid value\n");
      break;
   }

  for (int y = 0; y < 240; y++) {
    for (int x = 0; x < 320; x++) {
      bitmap[x][y][0] = drawColors[0];
      bitmap[x][y][1] = drawColors[1];
      bitmap[x][y][2] = drawColors[2];
    }
  }
  return pbitmap;
}
