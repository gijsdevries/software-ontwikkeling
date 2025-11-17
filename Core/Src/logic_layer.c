#include "logic_layer.h"
#include "global.h"

unsigned char* clearScreenToBitmap(clearscreen_struct CS_struct) {
  unsigned char bitmap[320][240][3];
  unsigned char *pbitmap = bitmap;
  unsigned char drawColors[3];
  RGB c;

  //TODO: add switch case for every color
  switch (CS_struct.color)
  {
    case BLACK:
      c = BLACK_RGB;
      drawColors[2] = c.b; drawColors[1] = c.g; drawColors[0] = c.r;
      break; 
    case BLUE:
      c = BLUE_RGB;
      drawColors[2] = c.b; drawColors[1] = c.g; drawColors[0] = c.r;
      break;
    case RED:
      c = RED_RGB;
      drawColors[2] = c.b; drawColors[1] = c.g; drawColors[0] = c.r;
      break;
    case GREEN:
      c = GREEN_RGB;
      drawColors[2] = c.b; drawColors[1] = c.g; drawColors[0] = c.r;
      break;
    case WHITE:
      c = WHITE_RGB;
      drawColors[2] = c.b; drawColors[1] = c.g; drawColors[0] = c.r;
      break;
   default:
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
