#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define die(...) do{printf(__VA_ARGS__);puts("\n");abort();}while(0)

#define MAXMAP (256*256)
#define MAXNPC (50)

typedef struct NPC {
  char type;
  int col;
  int row;
} NPC;

void print_game(char *map, int cols, int rows, NPC *npcs, int numnpcs) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      char ch = 0;
      for (int n = 0; n < numnpcs; ++n) {
        if (npcs[n].col == j && npcs[n].row == i) {
          ch = npcs[n].type;
        }
      }
      if (ch == 0) {
        ch = map[i * cols + j];
      }
      putchar(ch);
    }
    putchar('\n');
  }
}

char _buf[MAXMAP];
int turn(char *map, int cols, int rows, NPC *npcs, int numnpcs, int which) {
}

int main() {
  char map[MAXMAP];
  int rows = 0;
  int cols = 0;

  NPC npcs[MAXNPC];
  int numnpcs = 0;

  int row = 0;
  int col = 0;
  while (!feof(stdin)) {
    int ch = fgetc(stdin);
    switch (ch) {
    case '#':
    case '.':
      map[row * cols + col] = ch;
      col += 1;
      break;
    case 'E':
    case 'G':
      map[row * cols + col] = '.';
      npcs[numnpcs] = (NPC){ch, row, col};
      numnpcs += 1;
      col += 1;
      break;
    case '\n': 
      if (cols == 0) cols = col;
      else if (cols != col) die ("irregular map");
      col = 0;
      row += 1;
      break;
    default:
      break;
    }
  }
  rows = row;

  printf("cols: %d, rows: %d\n", cols, rows);

  print_game(map, cols, rows, npcs, numnpcs);
}
