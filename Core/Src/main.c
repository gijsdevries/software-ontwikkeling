#include "stm32_ub_vga_screen.h"
#include <math.h>
#include "front_layer.h"
#include "io_layer.h"
#include "logic_layer.h"
#include "global.h"

#include "stdio.h"
#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
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

const char (*gitSmallBitmap(textInfo textInfo, char letter))[5] {

   switch (textInfo.FONTSTIJL) {
    case NORMAAL:
      switch (textInfo.FONTNAAM) {
        case ARIAL:
          if (letter == 'a')
            return lc_a;
          if (letter == 'b')
            return lc_b;
          if (letter == 'c')
            return lc_c;
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

  if (textInfo.FONTGROOTTE == KLEIN) {
    int x = 5;
    int y = 1;
    for (char i = 0; i < sizeOfText; i++) {
      const char (*psmall_bitmap)[5] = gitSmallBitmap(textInfo, textStruct.text[i]);
      if (psmall_bitmap != NULL) {
        printf("drawing bitmap now....\n");
        letterToVGA(x, y, textStruct.color, (uint8_t(*)[5])psmall_bitmap, 5);
        if (x > 315) {
          x = 0;
          y = y + 5;
        }

        x = x + 5;
      }
    }
  }
  else {
    //TODO
  }
}

int main(void)
{
  SystemCoreClockUpdate();
  SystemInit(); // System speed to 168MHz
  UB_VGA_Screen_Init(); // Init VGA-Screen
  UB_VGA_FillScreen(VGA_COL_RED);
  USART2_Init();
  
  text_struct textStruct = {
    .x_lup = 10,
    .y_lup = 20,
    .color = VGA_COL_WHITE,
    .text = "abcabcabcabacabacabcabca",
    .fontname = "arial",
    .fontsize = 1,
    .fontstyle = "normaal"
  };

  ttextToVGA(textStruct);

  while(1)
  {
    USART2_BUFFER();
  }
}
