/*#include "main.h"
#include "stm32_ub_vga_screen.h"
#include <math.h>
#include "front_layer.h"
#include "io_layer.h"
#include "logic_layer.h"
#include "global.h"
*/

#include "stdio.h"
#include "logic_layer.h"
#include "stm32_ub_vga_screen.h"
#include "global.h"
#include "string.h"

//TODO
void getBitmap(text_struct textStruct) {

  if (!strcmp(textStruct.fontname, "arial"))
      printf("fontname arial\n");
  else if (!strcmp(textStruct.fontname, "consolas"))
    printf("fontname consolas\n");
  else
    printf("unknown cmd\n");

/*
  switch (textStruct.fontname) {
    case "arial":
      printf("arials");
      break;
    default:
      printf("no arial");
      break;
  }
 */ 
}

void textToVGA(text_struct textStruct)
{
  char sizeOfText = 0;

  for(sizeOfText = 0; sizeOfText < 255; sizeOfText++) {
    if (textStruct.text[sizeOfText] == 0)
      break;
  }

  printf("size of string: %d\n", sizeOfText);
  getBitmap(textStruct);
}

int main(void)
{
  text_struct textStruct = {
    .x_lup = 10,
    .y_lup = 20,
    .color = VGA_COL_WHITE,
    .text = "hallo daar",
    .fontname = "consolas",
    .fontsize = 12,
    .fontstyle = 2
  };

  textToVGA(textStruct);
}
