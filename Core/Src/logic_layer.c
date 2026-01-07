/** \file
 * logiclayerc
 * @brief Doet berekeningen om de gewenste commands op het VGA scherm te krijgen
*/

#include "logic_layer.h"
#include "bitmap_defines.h"
#include "stm32_ub_vga_screen.h"
#include "front_layer.h"
#include <stdio.h>

/**
 * @brief Converteert tekstinformatie van strings naar integers.
 * @param fontname De naam van het lettertype als string.
 * @param fontstyle De stijl van het lettertype als string.
 * @param fontsize De grootte van het lettertype.
 * @return Een textInfo struct met de geconverteerde waarden.
 */
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

/**
 * @brief Haalt de onbewerkte bitmapdata voor een letter op.
 * @param letter Het karakter waarvoor de data wordt opgehaald.
 * @param style Het lettertype (ARIAL of CONSOLAS).
 * @return Een pointer naar de bitmapdata van de letter.
 */
const char *getRawLetter(char letter, int style) {

  char index = 0;

  if (style == ARIAL)
    index = 0;
  else if (style == CONSOLAS)
    index = 1;

  switch (letter) {
    case '!': return exclamation[index];
    case '@': return at[index];
    case '#': return hash[index];
    case '$': return dollar[index];
    case '%': return percent[index];
    case '&': return ampersand[index];
    case '^': return caret[index];
    case '(': return left_paren[index];
    case ')': return right_paren[index];
    case '*': return asterisk[index];
    case ' ': return space[index];

    case '0': return zero[index];
    case '1': return one[index];
    case '2': return two[index];
    case '3': return three[index];
    case '4': return four[index];
    case '5': return five[index];
    case '6': return six[index];
    case '7': return seven[index];
    case '8': return eight[index];
    case '9': return nine[index];

    case 'a': return lc_a[index];
    case 'b': return lc_b[index];
    case 'c': return lc_c[index];
    case 'd': return lc_d[index];
    case 'e': return lc_e[index];
    case 'f': return lc_f[index];
    case 'g': return lc_g[index];
    case 'h': return lc_h[index];
    case 'i': return lc_i[index];
    case 'j': return lc_j[index];
    case 'k': return lc_k[index];
    case 'l': return lc_l[index];
    case 'm': return lc_m[index];
    case 'n': return lc_n[index];
    case 'o': return lc_o[index];
    case 'p': return lc_p[index];
    case 'q': return lc_q[index];
    case 'r': return lc_r[index];
    case 's': return lc_s[index];
    case 't': return lc_t[index];
    case 'u': return lc_u[index];
    case 'v': return lc_v[index];
    case 'w': return lc_w[index];
    case 'x': return lc_x[index];
    case 'y': return lc_y[index];
    case 'z': return lc_z[index];

    case 'A': return uc_a[index];
    case 'B': return uc_b[index];
    case 'C': return uc_c[index];
    case 'D': return uc_d[index];
    case 'E': return uc_e[index];
    case 'F': return uc_f[index];
    case 'G': return uc_g[index];
    case 'H': return uc_h[index];
    case 'I': return uc_i[index];
    case 'J': return uc_j[index];
    case 'K': return uc_k[index];
    case 'L': return uc_l[index];
    case 'M': return uc_m[index];
    case 'N': return uc_n[index];
    case 'O': return uc_o[index];
    case 'P': return uc_p[index];
    case 'Q': return uc_q[index];
    case 'R': return uc_r[index];
    case 'S': return uc_s[index];
    case 'T': return uc_t[index];
    case 'U': return uc_u[index];
    case 'V': return uc_v[index];
    case 'W': return uc_w[index];
    case 'X': return uc_x[index];
    case 'Y': return uc_y[index];
    case 'Z': return uc_z[index];
    default: return NULL;
  }
  return NULL;
}

/**
 * @brief Tekent een letter op het VGA-scherm.
 * @param x_lup X-coördinaat van de linkerbovenhoek.
 * @param y_lup Y-coördinaat van de linkerbovenhoek.
 * @param color De kleur van de letter.
 * @param letter Pointer naar de bitmapdata van de letter.
 * @param x_max De breedte van de letter.
 * @param y_max De hoogte van de letter.
 * @param size De grootte van de letter (KLEIN of GROOT).
 */
void letterToVGA(int x_lup, int y_lup, int color, const uint8_t *letter, char x_max, char y_max, int size)
{
  if (size == KLEIN)
  {
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
  }
  else { // GROOT
    const char (*doubled_bitmap)[2] = (const char (*)[2])letter; // Cast to correct type
    for (int y = 0; y < y_max; y++) { // Loop through 16 rows, was y_max + 2
      uint8_t high_byte = doubled_bitmap[y][0];
      uint8_t low_byte  = doubled_bitmap[y][1];

      // Process high byte (left 8 pixels)
      for (int x = 0; x < 8; x++) {
        if ((high_byte << x) & 0x80) {
          UB_VGA_SetPixel(x_lup + x, y_lup + y, color);
        }
      }
      // Process low byte (right 8 pixels)
      for (int x = 0; x < 8; x++) {
        if ((low_byte << x) & 0x80) {
          UB_VGA_SetPixel(x_lup + 8 + x, y_lup + y, color); // Offset by 8 for the right half
        }
      }
    }
  }
}

/**
 * @brief Vult het scherm met een bepaalde kleur.
 * @param CS_struct Struct met de kleur.
 */
void clearScreenToVGA(clearscreen_struct CS_struct) {
  UB_VGA_FillScreen(CS_struct.color);
}

/**
 * @brief Tekent een rechthoek op het VGA-scherm.
 * @param rectangleStruct Struct met de eigenschappen van de rechthoek.
 */
void rectangleToVGA(rectangle_struct rectangleStruct) {
  int x = rectangleStruct.x; int w = rectangleStruct.width;
  int y = rectangleStruct.y; int h = rectangleStruct.height;

  if (rectangleStruct.filled) {
    for (int yy = y; yy < y + h; yy++) {
      for (int xx = x; xx < x + w; xx++) {
        UB_VGA_SetPixel(xx, yy, rectangleStruct.color);
      }
    }
  }
  else {
    // Top edge
    for (int xx = x; xx < x + w; xx++)
      UB_VGA_SetPixel(xx, y, rectangleStruct.color);

    // Bottom edge
    for (int xx = x; xx < x + w; xx++)
      UB_VGA_SetPixel(xx, y + h - 1, rectangleStruct.color);

    // Left edge
    for (int yy = y; yy < y + h; yy++)
      UB_VGA_SetPixel(x, yy, rectangleStruct.color);

    // Right edge
    for (int yy = y; yy < y + h; yy++)
      UB_VGA_SetPixel(x + w - 1, yy, rectangleStruct.color);
  }
}

/**
 * @brief Tekent een lijn op het VGA-scherm met een bepaalde dikte.
 * @param lineStruct Struct met de eigenschappen van de lijn.
 */
void lineToVGA(line_struct lineStruct) {
  int x1 = lineStruct.x_1; int x2 = lineStruct.x_2;
  int y1 = lineStruct.y_1; int y2 = lineStruct.y_2;

  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;

  while (1) {
    for (int i = 0; i < lineStruct.weight; i++)
      if (dx > dy)
        UB_VGA_SetPixel(x1, y1 + i, lineStruct.color);
      else
        UB_VGA_SetPixel(x1 + i, y1, lineStruct.color);

    if (x1 == x2 && y1 == y2)
      break;

    int e2 = err * 2;

    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
}

/**
 * @brief Maakt een byte vet door een bit toe te voegen.
 * @param byte De input byte.
 * @return De "vette" byte.
 */
char addVet(char byte) {
  if (byte & 0x01)
    return byte;

  for (int i = 1; i < 8; i++) {
    if (byte & (1 << i)) {
      return byte | (1 << (i - 1));
    }
  }
  return byte;
}

/**
 * @brief Maakt een byte cursief door te shiften.
 * @param byte De input byte.
 * @param row De huidige rij van de letter.
 * @param style Het lettertype.
 * @return De "cursieve" byte.
 */
char addCursive(char byte, int row, int style) {

  char cursiveFactor;

  if (style == ARIAL)
    cursiveFactor = 5;
  else
    cursiveFactor = 3;

  return byte >> (row / cursiveFactor);
}

/**
 * @brief Verdubbelt een 8x8 bitmap naar een 16x16 bitmap.
 * @param src De bron 8x8 bitmap.
 * @param dst De bestemming 16x16 bitmap.
 */
void double_bitmap(const char src[8], char dst[16][2]) {
  for (int y = 0; y < 8; y++) {
    uint8_t row = src[y];
    uint16_t doubled_row = 0;

    // Double each bit horizontally
    for (int x = 0; x < 8; x++) {
      int bit = (row >> (7 - x)) & 1;
      // Place two bits for doubling
      doubled_row |= (bit << (15 - 2*x));
      doubled_row |= (bit << (15 - 2*x - 1));
    }

    // Split 16-bit row into two bytes
    uint8_t high_byte = (doubled_row >> 8) & 0xFF;
    uint8_t low_byte  = doubled_row & 0xFF;

    // Double each row vertically
    dst[2*y][0]   = high_byte;
    dst[2*y][1]   = low_byte;
    dst[2*y+1][0] = high_byte;
    dst[2*y+1][1] = low_byte;
  }
}

/**
 * @brief Tekent een volledige tekst op het VGA-scherm.
 * @param textStruct Struct met de eigenschappen van de tekst.
 */
void textToVGA(text_struct textStruct) {
  size_t sizeOfText = strlen(textStruct.text);
  textInfo textInfo = textStructToInt(textStruct.fontname, textStruct.fontstyle, textStruct.fontsize);

  char dx, dy = 0;
  int x = textStruct.x_lup;
  int y = textStruct.y_lup;


  if (textInfo.FONTGROOTTE == KLEIN) {
    dx = 8;
    dy = 8;
  }
  else {
    dx = 16;
    dy = 16;
  }

  uint8_t buff[8]; // font data is always 8x8

  for (char i = 0; i < sizeOfText; i++) {
    const char *buf = getRawLetter(textStruct.text[i], textInfo.FONTNAAM);
    memcpy(buff, buf, 8);

    if (textInfo.FONTSTIJL == VET) {
      for (char j = 0; j < 8; j++) {
        buff[j] = addVet(buff[j]);
      }
    } else if (textInfo.FONTSTIJL == CURSIEF) {
      for (char j = 0; j < 8; j++) {
        buff[j] = addCursive(buff[j], j, textInfo.FONTNAAM);
      }
    }

    if (textInfo.FONTGROOTTE == GROOT) {
      uint8_t dest[16][2];

      // The memcpy from NULL was causing a crash. It's removed.
      // Zeroing the buffer is not necessary as double_bitmap fills it completely.
      double_bitmap((const char*)buff, (char (*)[2])dest);
      letterToVGA(x, y, textStruct.color, (const uint8_t*)dest, dx, dy, textInfo.FONTGROOTTE);
    }
    else {
      letterToVGA(x, y, textStruct.color, buff, dx, dy, textInfo.FONTGROOTTE);
    }

    x += dx;

    if (x > VGA_DISPLAY_X - dx) { //go to a new line
      x = 0;
      y += dy;
    }
  }
}

/**
 * @brief Tekent een bitmap op het VGA-scherm.
 * @param bitmapStruct Struct met de eigenschappen van de bitmap.
 */
void bitmapToVGA(bitmap_struct bitmapStruct) {
  uint8_t* bitmap = NULL;

  switch (bitmapStruct.bm_nr) { //Kies bitmap op basis van ingevoerd getal
    case 0:
      bitmap = SMILEY_BOOS_BITMAP;
      break;
    case 1:
      bitmap = SMILEY_BLIJ_BITMAP;
      break;
    case 2:
      bitmap = PIJL_OMHOOG_BITMAP;
      break;
    case 3:
      bitmap = PIJL_NAAR_BENEDEN_BITMAP;
      break;
    case 4:
      bitmap = PIJL_NAAR_RECHTS_BITMAP;
      break;
    case 5:
      bitmap = PIJL_NAAR_LINKS_BITMAP;
      break;
    case 6:
      bitmap = INKTVIS_BITMAP;
      break;
  }

  //Zet witte pixels van bitmap op het scherm, negeer de rest
  for (int yy = 0; yy < MAX_BITMAP_ARRAY; yy++) {
    for (int xx = 0; xx < MAX_BITMAP_ARRAY; xx++)
    {
      uint8_t pixelColor = bitmap[yy * MAX_BITMAP_ARRAY + xx];
      if (pixelColor == 255)
        UB_VGA_SetPixel(bitmapStruct.x_lup + xx, bitmapStruct.y_lup + yy, pixelColor);
    }
  }
}
