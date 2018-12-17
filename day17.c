#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define die(...) do{printf(__VA_ARGS__);puts("\n");abort();}while(0)
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

typedef struct Vein {
  int xstart, xend;
  int ystart, yend;
  struct Vein *next;
} Vein;

void init_map(char* map, int xoff, int yoff, int width, int height, Vein *vein) {
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      map[ i * width + j] = '.';
    }
  }
  while (vein != NULL) {
    for (int i = vein->ystart; i < vein->yend; ++i) {
      for (int j = vein->xstart; j < vein->xend; ++j) {
        map[ (i-yoff) * width + (j-xoff) ] = '#';
      }
    }
    vein = vein->next;
  }
}

void print_map(char* map, int xoff, int yoff, int width, int height) {
  for (int j = 0; j < width; ++j) {
    if (j + xoff == 500)
      putchar('+');
    else
      putchar('.');
  }
  putchar('\n');
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      putchar(map[i*width + j]);
    }
    putchar('\n');
  }
}

void step_map(char* map, int xoff, int yoff, int width, int height) {
  for (int j = 0; j < width; ++j) {
    if (j + xoff == 500) map[j] = '-';
  }

  for (int j = 0; j < width; ++j) {
    if (map[(height-1)*width + j] == '-') {
      map[(height-1)*width + j] = '|';
    }
  }

  for (int i = height-2; i >= 0; --i) {
    for (int j = 0; j < width; ++j) {
      if (map[i * width +j] == '-') {
        char b = map[(i+1)*width+j];
        char l = j > 0 ? map[i*width+j-1] : '#';
        char r = j < width-1 ? map[i*width+j+1] : '#';
        if (b == '|' || b == '.') {
          map[(i+1)*width+j] = '-';
          map[i*width+j] = '|';
        } else if (l == '|' || l == '.') {
          map[i*width+j-1] = '-';
          map[i*width+j] = '|';
        } else if (r == '|' || r == '.') {
          map[i*width+j+1] = '-';
          map[i*width+j] = '|';
        } else {
          map[i*width+j] = '~';
        }
      }
    }
  }
}

int main(int argc, char** argv) {

  if (argc != 2) die("usage: day17 <input>");

  FILE *input = fopen(argv[1], "r");
  if (input == NULL) die("unable to open file %s", argv[1]);

  Vein *veins = NULL;
  char c1, c2;
  int fix, start, stop;
  int first = 1;
  int xmin, xmax, ymin, ymax;
  while (fscanf(input, "%c=%d, %c=%d..%d\n", &c1, &fix, &c2, &start, &stop) == 5) {
    Vein * vein = malloc(sizeof(Vein));
    if (vein == NULL) die("out of memory");
    if (c1 == 'x') {
      *vein = (Vein){fix, fix+1, start, stop+1, veins};
    } else {
      *vein = (Vein){start, stop+1, fix, fix+1, veins};
    }
    if (first) {
      xmin = vein->xstart;
      xmax = vein->xend;
      ymin = vein->ystart;
      ymax = vein->yend;
      first = 0;
    } else {
      xmin = min(xmin, vein->xstart);
      xmax = max(xmax, vein->xend);
      ymin = min(ymin, vein->ystart);
      ymax = max(ymax, vein->yend);
    }
    veins = vein;
  }
  fclose(input);

  int xoff = xmin - 1;
  int yoff = ymin;
  int width = xmax - xmin + 2;
  int height = ymax - ymin;

  char *map_front = malloc(width * height);
  if (map_front == NULL) die("out of memory");
  char *map_back = malloc(width * height);
  if (map_back == NULL) die("out of memory");
  init_map(map_front, xoff, yoff, width, height, veins);

  while (veins != NULL) {
    Vein *tmp = veins->next;
    free(veins);
    veins = tmp;
  }

  print_map(map_front, xoff, yoff, width, height);
  while (1) {
    getchar();
    //printf("\n");
    step_map(map_front, xoff, yoff, width, height);
    print_map(map_front, xoff, yoff, width, height);
  }
}
