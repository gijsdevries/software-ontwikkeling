#ifndef GLOBAL_H 
#define GLOBAL_H

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

//TODO: give appropiate color ranges.
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

#endif
