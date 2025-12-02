#ifndef LOGIC_LAYER_H 
#define LOGIC_LAYER_H

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
  int color;
} clearscreen_struct;

//function prototypes
void clearScreenToVGA(clearscreen_struct CS_struct);
void rectangleToVGA(rectangle_struct rectangleStruct);
void lineToVGA(line_struct lineStruct);
int letterToVGA(int x_lup, int y_lup, int color, uint8_t bitmap[5][5], int font_size);

const bool lc_a[5][5] = {
  {0, 1, 1, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0},
  {0, 1, 1, 1, 1}
};

const bool lc_b[5][5] = {
  {1, 1, 0, 0, 0},
  {1, 0, 1, 0, 0},
  {1, 1, 0, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 1, 1, 0, 0}
};

const bool lc_c[5][5] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0}
};

const bool lc_d[5][5] = {
  {0, 0, 1, 1, 0},
  {0, 1, 0, 0, 1},
  {0, 1, 0, 0, 1},
  {0, 1, 0, 0, 1},
  {0, 0, 1, 1, 0}
};

const bool lc_e[5][5] = {
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 0}
};

const bool lc_f[5][5] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0}
};

const bool lc_g[5][5] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
  {0, 0, 0, 1, 1}
};

const bool lc_h[5][5] = {
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 0},
  {1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0}
};

const bool lc_i[5][5] = {
  {0, 1, 1, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 1, 1, 1, 0}
};

const bool lc_j[5][5] = {
  {0, 0, 0, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 0, 0, 1, 0},
  {1, 0, 0, 1, 0},
  {0, 1, 1, 0, 0}
};

const bool lc_k[5][5] = {
  {1, 0, 0, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 1, 1, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 0, 0, 0, 1}
};

const bool lc_l[5][5] = {
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1}
};

const bool lc_m[5][5] = {
  {1, 0, 0, 0, 1},
  {1, 1, 0, 1, 1},
  {1, 0, 1, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1}
};

const bool lc_n[5][5] = {
  {1, 0, 0, 0, 0},
  {1, 1, 0, 0, 0},
  {1, 0, 1, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 0, 0, 0, 1}
};

const bool lc_o[5][5] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0}
};

const bool lc_p[5][5] = {
  {1, 1, 1, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 1, 1, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0}
};

const bool lc_q[5][5] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
  {0, 0, 0, 0, 1}
};

const bool lc_r[5][5] = {
  {1, 1, 1, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 1, 0, 0, 0},
  {1, 0, 0, 1, 0},
  {1, 0, 0, 0, 1}
};

const bool lc_s[5][5] = {
  {0, 1, 1, 1, 1},
  {1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0},
  {0, 0, 0, 0, 1},
  {1, 1, 1, 1, 0}
};

const bool lc_t[5][5] = {
  {1, 1, 1, 1, 1},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0}
};

const bool lc_u[5][5] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0}
};

const bool lc_v[5][5] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 1, 0, 1, 0},
  {0, 0, 1, 0, 0}
};

const bool lc_w[5][5] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 1, 0, 1},
  {1, 1, 0, 1, 1},
  {1, 0, 0, 0, 1}
};

const bool lc_x[5][5] = {
  {1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 1, 0, 1, 0},
  {1, 0, 0, 0, 1}
};

const bool lc_y[5][5] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0}
};

const bool lc_z[5][5] = {
  {1, 1, 1, 1, 1},
  {0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0},
  {1, 1, 1, 1, 1}
};

#endif
