#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// Idea: Stars are flying towards each other, form the message, and scatter.
// So the total area of the stars bounding box starts of big, shrinks, and at
// some point starts increasing again.
// We need to find this point and print it, because it likely contains the message

#define die(msg) do{printf(msg "\n");exit(1);}while(0)
#define check_vital(cond) do{if(!(cond))die(#cond " failed!");}while(0)

// no decimals in input, so we can use integral types
typedef struct Star {
  int x, y, dx, dy;
} Star;

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

void stars_step(Star *stars, int numstars, int dir) {
  for (int i = 0; i < numstars; ++i) {
    stars[i].x += dir * stars[i].dx;
    stars[i].y += dir * stars[i].dy;
  }
}

int64_t stars_area(Star *stars, int numstars) {
  int minx, maxx, miny, maxy;
  check_vital(numstars > 0);
  minx = maxx = stars[0].x;
  miny = maxy = stars[0].y;
  for (int i = 1; i < numstars; ++i) {
    minx = min(minx, stars[i].x);
    maxx = max(maxx, stars[i].x);
    miny = min(miny, stars[i].y);
    maxy = max(maxy, stars[i].y);
  }
  int width = maxx - minx;
  int height = maxy - miny;
  int64_t area = width * width + height * height;
  //printf("area: %" PRId64 "\n", area);
  return area;
}

void stars_print(Star *stars, int numstars) {
  int minx, maxx, miny, maxy;
  check_vital(numstars > 0);
  minx = maxx = stars[0].x;
  miny = maxy = stars[0].y;
  for (int i = 1; i < numstars; ++i) {
    minx = min(minx, stars[i].x);
    maxx = max(maxx, stars[i].x);
    miny = min(miny, stars[i].y);
    maxy = max(maxy, stars[i].y);
  }
  int width = maxx - minx + 1;
  int height = maxy - miny + 1;
  char *buf = malloc(sizeof(char[height][width]));
  memset(buf, '.', sizeof(char[height][width]));
  for (int i = 0; i < numstars; ++i) {
    int x = stars[i].x - minx;
    int y = stars[i].y - miny;
    buf[y * width + x] = '#';
  }
  for (int i = 0; i < height; ++i) {
    printf("%.*s\n", width, &buf[i*width]);
  }
}

int main(int argc, char** argv) {
  // parse command line
  FILE *input = NULL;
  if (argc == 1) {
    input = stdin;
  } else if (argc == 2) {
    input = fopen(argv[1], "r");
    check_vital((input != NULL) && "unable to open file for reading");
  } else {
    die("usage: day10-code [filename]");
  }

  // prepare stars buffer
  const int maxstars = 1024;
  Star *stars = malloc(sizeof(Star[maxstars]));
  check_vital(stars != NULL);
  size_t stars_num = 0;

  // read input
  char* line = NULL;
  size_t line_cap;
  while (getline(&line, &line_cap, input) > 0) {
    const char* s = line;
    while (*s == ' ' || *s == '\t' || *s == '\n') ++s;
    if (strlen(s) == 0 || s[0] == '#') continue;
    check_vital(stars_num < maxstars-1);
    int x, y, dx, dy, ncon;
    if ( sscanf(s, " position=< %d , %d > velocity=< %d ,  %d > ",  &x, &y, &dx, &dy) != 4) {
      printf("unable to parse line: '%s'\n", line);
      exit(1);
    }
    stars[stars_num] = (Star){x, y, dx, dy};
    stars_num += 1;
  }
  check_vital(feof(input) && "reading aborted before end-of-file");

  int64_t area = stars_area(stars, stars_num);
  int step = 0;
  // simulate until total area of stars starts to increase again
  while (1) {
    stars_step(stars, stars_num, 1);
    step += 1;
    int darea = stars_area(stars, stars_num) - area;
    area += darea;
    if (darea > 0) {
      printf("smallest bounding area at step %d\n", step-1);
      break;
    }
  }
  // rewind one step and print
  stars_step(stars, stars_num, -1);
  stars_print(stars, stars_num);
  
  fclose(input);
  free(stars);
}
