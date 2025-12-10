#ifndef BITMAP_DEFINES 
#define BITMAP_DEFINES

#include <stdint.h>
#include "logic_layer.h"

const char nor_arial_a[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0},
  {0, 1, 1, 1, 1}
};

const char nor_arial_b[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 1, 0, 0, 0},
  {1, 0, 1, 0, 0},
  {1, 1, 0, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 1, 1, 0, 0}
};

const char nor_arial_c[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0}
};

const char nor_arial_d[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {0, 0, 1, 1, 0},
  {0, 1, 0, 0, 1},
  {0, 1, 0, 0, 1},
  {0, 1, 0, 0, 1},
  {0, 0, 1, 1, 0}
};

const char nor_arial_e[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 0}
};

const char nor_arial_f[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0}
};

const char nor_arial_g[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
  {0, 0, 0, 1, 1}
};

const char nor_arial_h[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 0},
  {1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0}
};

const char nor_arial_i[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {0, 1, 1, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 1, 1, 1, 0}
};

const char nor_arial_j[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {0, 0, 0, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 0, 0, 1, 0},
  {1, 0, 0, 1, 0},
  {0, 1, 1, 0, 0}
};

const char nor_arial_k[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 0, 0, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 1, 1, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 0, 0, 0, 1}
};

const char nor_arial_l[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1}
};

const char nor_arial_m[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 0, 0, 0, 1},
  {1, 1, 0, 1, 1},
  {1, 0, 1, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1}
};

const char nor_arial_n[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 0, 0, 0, 0},
  {1, 1, 0, 0, 0},
  {1, 0, 1, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 0, 0, 0, 1}
};

const char nor_arial_o[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0}
};

const char nor_arial_p[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 1, 1, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 1, 1, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0}
};

const char nor_arial_q[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
  {0, 0, 0, 0, 1}
};

const char nor_arial_r[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 1, 1, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 1, 0, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 0, 0, 0, 1}
};

const char nor_arial_s[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {0, 1, 1, 1, 1},
  {1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0},
  {0, 0, 0, 0, 1},
  {1, 1, 1, 1, 0}
};

const char nor_arial_t[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 1, 1, 1, 1},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0}
};

const char nor_arial_u[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0}
};

const char nor_arial_v[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 1, 0, 1, 0},
  {0, 0, 1, 0, 0}
};

const char nor_arial_w[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 1, 0, 1},
  {1, 1, 0, 1, 1},
  {1, 0, 0, 0, 1}
};

const char nor_arial_x[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 1, 0, 1, 0},
  {1, 0, 0, 0, 1}
};

const char nor_arial_y[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0}
};

const char nor_arial_z[SMALL_LETTER_SIZE][SMALL_LETTER_SIZE] = {
  {1, 1, 1, 1, 1},
  {0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0},
  {1, 1, 1, 1, 1}
};

#endif
