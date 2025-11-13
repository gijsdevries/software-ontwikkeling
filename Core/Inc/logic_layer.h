struct text_struct {
  int x_lup;
  int y_lup;
  int color;
  char *text;
  char *fontname;
  int fontsize;
  int fontstyle;
}

struct line_struct {
  int x_1;
  int y_1;
  int x_2;
  int y_2;
  int color;
  int weight;
}

struct rectangle_struct {
  int x;
  int y;
  int width;
  int height;
  int color;
  int filled;
}

struct bitmap_struct {
  int x_lup;
  int y_lup;
  int bm_nr;
}

struct clearscreen_struct {
  int color;
}

