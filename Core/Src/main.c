#include "stm32_ub_vga_screen.h"
#include <math.h>
#include "front_layer.h"
#include "io_layer.h"
#include "global.h"
#include "stdio.h"
#include "logic_layer.h"
#include "global.h"
#include "string.h"
#include "bitmap_defines.h"

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

typedef struct {
  int FONTNAAM;
  int FONTSTIJL;
  int FONTGROOTTE;
} textInfo;

textInfo textStructToInt(char *fontname, char *fontstyle, char fontsize) {

  textInfo textInfo;

  if (!strcmp(fontname, "arial"))
    textInfo.FONTNAAM = ARIAL; 
  else if (!strcmp(fontname, "consolas"))
    textInfo.FONTNAAM = CONSOLAS;
  else //struct is already error handled, so it should never be -1
    textInfo.FONTNAAM = -1; 

  if (!strcmp(fontstyle, "normaal"))
    textInfo.FONTSTIJL = NORMAAL; 
  else if (!strcmp(fontstyle, "vet"))
    textInfo.FONTSTIJL = VET;
  else if (!strcmp(fontstyle, "cursief"))
    textInfo.FONTSTIJL = CURSIEF;
  else //struct is already error handled, so it should never be -1
    textInfo.FONTSTIJL= -1;

  if (fontsize == 1)
    textInfo.FONTGROOTTE = KLEIN;
  else if (fontsize == 2)
    textInfo.FONTGROOTTE = GROOT;
  else
    textInfo.FONTGROOTTE = -1;

  return textInfo;
}

//fontnaam arial, fontstijl normaal
const char (*getSmallBitmapLetter(char letter))[SMALL_LETTER_SIZE] {
  switch (letter) {
    case 'a': return nor_arial_a;
    case 'b': return nor_arial_b;
    case 'c': return nor_arial_c;
    case 'd': return nor_arial_d;
    case 'e': return nor_arial_e;
    case 'f': return nor_arial_f;
    case 'g': return nor_arial_g;
    case 'h': return nor_arial_h;
    case 'i': return nor_arial_i;
    case 'j': return nor_arial_j;
    case 'k': return nor_arial_k;
    case 'l': return nor_arial_l;
    case 'm': return nor_arial_m;
    case 'n': return nor_arial_n;
    case 'o': return nor_arial_o;
    case 'p': return nor_arial_p;
    case 'q': return nor_arial_q;
    case 'r': return nor_arial_r;
    case 's': return nor_arial_s;
    case 't': return nor_arial_t;
    case 'u': return nor_arial_u;
    case 'v': return nor_arial_v;
    case 'w': return nor_arial_w;
    case 'x': return nor_arial_x;
    case 'y': return nor_arial_y;
    case 'z': return nor_arial_z;
  }
}

const char *bitmapLinus(char letter) {

  switch (letter) {
    case 'a': return letter_a;
    default: return NULL;
  }
}

const char (*getSmallBitmap(textInfo textInfo, char letter))[SMALL_LETTER_SIZE] {

  switch (textInfo.FONTSTIJL) {
    case NORMAAL:
      switch (textInfo.FONTNAAM) {
        case ARIAL:
          return getSmallBitmapLetter(letter);
          break;
        case CONSOLAS:
        default:
          break;
      }
      break;
    case VET:
      switch (textInfo.FONTNAAM) {
        case ARIAL:
        case CONSOLAS:
        default:
          break;
      }
      break;
    case CURSIEF:
      switch (textInfo.FONTNAAM) {
        case ARIAL:
        case CONSOLAS:
        default:
          break;
      }
      break;
    default:
      break;
  }
  return NULL;
}

void ttextToVGA(text_struct textStruct)
{
  size_t sizeOfText = strlen(textStruct.text);
  textInfo textInfo = textStructToInt(textStruct.fontname, textStruct.fontstyle, textStruct.fontsize);

  printf("size of string: %zu\n", sizeOfText);

  int x = textStruct.x_lup;
  int y = textStruct.y_lup;

  if (textInfo.FONTGROOTTE == GROOT) {

    char *letter_buf = bitmapLinus(textStruct.text[0]);
    letterToVGANEW(x, y, textStruct.color, letter_buf, GROOT);
    for (char i = 0; i < sizeOfText; i++) {
      if (letter_buf != NULL) {

        //letterToVGANEW(x, y, textStruct.color, letter_buf, GROOT);
        if (x > VGA_DISPLAY_X - SMALL_LETTER_SIZE) { //go to a new line
          x = 0;
          y = y + SMALL_LETTER_SIZE;
        }
        x = x + SMALL_LETTER_SIZE;
      }
    }
  }
  else {
    //TODO
  }
}

int letterToVGANEW(int x_lup, int y_lup, int color, const uint8_t *letter, int size)
{
  char y_max, x_max = 0;
  if (size == GROOT) {
    y_max = SIZE_BIG_LETTER_Y;
    x_max = SIZE_BIG_LETTER_X;
  }
  for (int y = 0; y < y_max; y++) // Loop door alle y coördinaten heen
  {
    for (int x = 0; x < x_max; x++) // Loop door alle x coördinaten heen
    {
      if ((letter[y] << x) & 0x80) // Als een pixel hoog is
      {
        UB_VGA_SetPixel(x_lup + x, y_lup + y, color); // Pas de pixel aan naar de gegeven kleur
      }
    }
  }
  return 0;
}


int main(void)
{
  SystemCoreClockUpdate();
  SystemInit(); // System speed to 168MHz
  UB_VGA_Screen_Init(); // Init VGA-Screen
  UB_VGA_FillScreen(VGA_COL_RED);
  USART2_Init();

  text_struct textStruct = {
    .x_lup = 100,
    .y_lup = 200,
    .color = VGA_COL_BLACK,
    .text = "a",
    .fontname = "arial",
    .fontsize = 2,
    .fontstyle = "normaal"
  };

  //ttextToVGA(textStruct);


  char* letter_buf = bitmapLinus('a');
  letterToVGANEW(200, 100, textStruct.color, letter_buf, GROOT);

  while(1)
  {
    USART2_BUFFER();
  }
}
