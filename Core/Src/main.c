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

const char *bitmapLinus(char letter) {

  switch (letter) {

    // Special characters
    case '!': return exclamation;
    case '@': return at;
    case '#': return hash;
    case '$': return dollar;
    case '%': return percent;
    case '&': return ampersand;
    case '^': return caret;
    case '(': return left_paren;
    case ')': return right_paren;
    case '*': return asterisk;
    case ' ': return space;

    case '0': return zero;
    case '1': return one;
    case '2': return two;
    case '3': return three;
    case '4': return four;
    case '5': return five;
    case '6': return six;
    case '7': return seven;
    case '8': return eight;
    case '9': return nine;

    case 'a': return lc_letter_a;
    case 'b': return lc_letter_b;
    case 'c': return lc_letter_c;
    case 'd': return lc_letter_d;
    case 'e': return lc_letter_e;
    case 'f': return lc_letter_f;
    case 'g': return lc_letter_g;
    case 'h': return lc_letter_h;
    case 'i': return lc_letter_i;
    case 'j': return lc_letter_j;
    case 'k': return lc_letter_k;
    case 'l': return lc_letter_l;
    case 'm': return lc_letter_m;
    case 'n': return lc_letter_n;
    case 'o': return lc_letter_o;
    case 'p': return lc_letter_p;
    case 'q': return lc_letter_q;
    case 'r': return lc_letter_r;
    case 's': return lc_letter_s;
    case 't': return lc_letter_t;
    case 'u': return lc_letter_u;
    case 'v': return lc_letter_v;
    case 'w': return lc_letter_w;
    case 'x': return lc_letter_x;
    case 'y': return lc_letter_y;
    case 'z': return lc_letter_z;

    case 'A': return uc_letter_a;
    case 'B': return uc_letter_b;
    case 'C': return uc_letter_c;
    case 'D': return uc_letter_d;
    case 'E': return uc_letter_e;
    case 'F': return uc_letter_f;
    case 'G': return uc_letter_g;
    case 'H': return uc_letter_h;
    case 'I': return uc_letter_i;
    case 'J': return uc_letter_j;
    case 'K': return uc_letter_k;
    case 'L': return uc_letter_l;
    case 'M': return uc_letter_m;
    case 'N': return uc_letter_n;
    case 'O': return uc_letter_o;
    case 'P': return uc_letter_p;
    case 'Q': return uc_letter_q;
    case 'R': return uc_letter_r;
    case 'S': return uc_letter_s;
    case 'T': return uc_letter_t;
    case 'U': return uc_letter_u;
    case 'V': return uc_letter_v;
    case 'W': return uc_letter_w;
    case 'X': return uc_letter_x;
    case 'Y': return uc_letter_y;
    case 'Z': return uc_letter_z;

    default: return NULL;
  }
}

void ttextToVGA(text_struct textStruct)
{
  size_t sizeOfText = strlen(textStruct.text);
  textInfo textInfo = textStructToInt(textStruct.fontname, textStruct.fontstyle, textStruct.fontsize);

  int x = textStruct.x_lup;
  int y = textStruct.y_lup;

  if (textInfo.FONTGROOTTE == GROOT) {

    for (char i = 0; i < sizeOfText; i++) {
      char *letter_buf = bitmapLinus(textStruct.text[i]);
      letterToVGA(x, y, textStruct.color, letter_buf, GROOT);

      x = x + SIZE_BIG_LETTER_X;

      if (x > VGA_DISPLAY_X - SIZE_BIG_LETTER_X) { //go to a new line
        x = 0;
        y = y + SIZE_BIG_LETTER_Y;
      }
    }
  }
  else {
    //TODO
  }
}

int letterToVGA(int x_lup, int y_lup, int color, const uint8_t *letter, int size)
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
    .x_lup = 1,
    .y_lup = 1,
    .color = VGA_COL_CYAN,
    .text = "!@#$%^&*() 123456789 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    .fontname = "arial",
    .fontsize = 2,
    .fontstyle = "normaal"
  };

  ttextToVGA(textStruct);


  //char* lc_letter_buf = bitmapLinus('a');
  //letterToVGA(200, 100, textStruct.color, lc_letter_buf, GROOT);

  while(1)
  {
    USART2_BUFFER();
  }
}
