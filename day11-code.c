#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define die(msg) do{printf(msg "\n");exit(1);}while(0)

#define WIDTH (300)
#define HEIGHT (300)
#define GRID (9798)
//#define GRID (18)

int power_level(int x, int y, int serial) {
  int64_t power = (x + 10) * y + serial;
  power = power * (x + 10);
  power = (power / 100)%10;
  power = power - 5;
  return (int)power;
}

void integral_image(int *buf, int w, int h) {
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      int sum = buf[i*w+j];
      if (i > 0) sum += buf[(i-1)*w+j];
      if (j > 0) sum += buf[i*w+(j-1)];
      if (i > 0 && j > 0) sum -= buf[(i-1)*w+(j-1)];
      buf[i*w+j] = sum;
    }
  }
}

int power_square(int *buf, int x, int y, int w, int h, int sq) {
  return buf[y*w + x]
    + buf[(y+sq)*w + x+sq]
    - buf[y*w + x+sq]
    - buf[(y+sq)*w + x];
}

int main() {
  int *buf = malloc(sizeof(int[HEIGHT][WIDTH]));
  if (buf == NULL) {
    die("out of memory");
  }

  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      buf[i*WIDTH+j] = power_level(j+1, i+1, GRID);
    }
  }

  integral_image(buf, WIDTH, HEIGHT);
  int maxpower = 0;
  int bestx, besty;
  for (int i = 0; i < HEIGHT-3; ++i) {
    for (int j = 0; j < WIDTH-3; ++j) {
      int sum = power_square(buf, j, i, WIDTH, HEIGHT, 3);
      if (sum > maxpower) {
        maxpower = sum;
        bestx = j+2;
        besty = i+2;
      }
    }
  }
  printf("best 3x3 square @ %d, %d with %d power\n", bestx, besty, maxpower);

  maxpower = 0;
  int bestsize;
  for (int sq = 1; sq < WIDTH; ++sq) {
    for (int i = 0; i < HEIGHT-sq; ++i) {
      for (int j = 0; j < WIDTH-sq; ++j) {
        int sum = power_square(buf, j, i, WIDTH, HEIGHT, sq);
        if (sum > maxpower) {
          maxpower = sum;
          bestsize = sq;
          bestx = j+2;
          besty = i+2;
        }
      }
    }
  }
  printf("best square @ %d,%d,%d with %d power\n", bestx, besty, bestsize, maxpower);

  free(buf);
}
