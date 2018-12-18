#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>

#define die(...) do{printf(__VA_ARGS__);puts("\n");abort();}while(0)
#define MAXLINES 50

int type(char ch) {
  switch (ch) {
    case '.': return 0;
    case '|': return 1;
    case '#': return 2;
    default: return 3;
  }
}

char getsafe(char* front, int width, int height, int x, int y) {
  if (x < 0 || y < 0 || x >= width || y >= height) {
    return ' ';
  } else {
    return front[y*width+x];
  }
}

int step(char* front, char* back, int width, int height) {
  int trees = 0;
  int yards = 0;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int n[4] = {0, 0, 0, 0};

      n[type(getsafe(front, width, height, j-1, i-1))] += 1;
      n[type(getsafe(front, width, height, j,   i-1))] += 1;
      n[type(getsafe(front, width, height, j+1, i-1))] += 1;

      n[type(getsafe(front, width, height, j-1, i))] += 1;
      n[type(getsafe(front, width, height, j+1, i))] += 1;

      n[type(getsafe(front, width, height, j-1, i+1))] += 1;
      n[type(getsafe(front, width, height, j,   i+1))] += 1;
      n[type(getsafe(front, width, height, j+1, i+1))] += 1;

      char ch = front[i*width + j];
      char next = 'x';

      if (ch == '.') {
        if (n[1] >= 3) next = '|';
        else next = '.';
      } else if (ch == '|') {
        if (n[2] >= 3) next = '#';
        else next = '|';
      } else if (ch == '#') {
        if (n[2] >= 1 && n[1] >= 1) next = '#';
        else next = '.';
      } else die("invalid value");
      back[i*width+j] = next;
      if (next == '|') trees += 1;
      if (next == '#') yards += 1;
    }
  }
  return trees*yards;
}

void display(char* front, int width, int height) {
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      putchar(front[i*width+j]);
    }
    putchar('\n');
  }
}

int main(int argc, char** argv) {

  if (argc != 2) die("usage: day18 <input>");

  FILE *input = fopen(argv[1], "r");

  char *lines[MAXLINES];
  int numlines = 0;
  size_t buflen;

  int width = 0;

  lines[0] = NULL;
  while (getline(&lines[numlines], &buflen, input) > 0) {
    if (numlines >= MAXLINES) die("too many lines");
    int len = strlen(lines[numlines]);
    if (lines[numlines][len-1] == '\n')
      len -= 1;

    if (width == 0)
      width = len;
    else if (width != len)
      die("non-uniform width\n");

    numlines += 1;
    lines[numlines] = NULL;
  }
  fclose(input);

  int height = numlines;
  char* front = malloc(width*height);
  if (front == NULL) die("out of memory");
  char* back = malloc(width*height);
  if (back == NULL) die("out of memory");

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      front[i*width + j] = lines[i][j];
    }
    free(lines[i]);
  }

  int n = 1000000000;

  // values are perdiodic, so we detect the period and extrapolate
  int start = 0;
  int period = 0;
  int last;
  int series[50];

  //display(front, width, height);
  for (int i = 0; i < n; ++i) {
    //getchar();
    last = step(front, back, width, height);

    if (i > 0 && i % 1000 == 0) {
      printf("step %d, last: %d\n", i, last);
      series[i/1000-1] = last;
      if (i > 1000 && last == series[0]) {
        period = i/1000-1;
        //break;
      }
    }
    char* tmp = back;
    back = front;
    front = tmp;
    //display(front, width, height);
  }
  //printf("period: %d\n", period);

  //printf("prediction for %d: %d\n", n, series[((n/1000)-1)%period]);

  for (int i = 9000; i < 20000; i += 1000) {
    int idx = ((i/1000)-1) % period;
    printf("step %d, last: %d\n", i, series[idx]);
  }

  //printf("resource value after %ds: %d\n", n, last);
}
