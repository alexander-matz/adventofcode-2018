#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define die(...) do{printf(__VA_ARGS__);puts("\n");exit(1);}while(0)

// recipes have 1 digit max, so char works fine
char *recipes = NULL;
int recipes_len = 0;
int recipes_cap = 4096;
int curr_a = 0;
int curr_b = 1;

void recipes_reserve(int cap) {
  if (recipes != NULL && recipes_cap > cap) return;
  while (recipes_cap < cap) recipes_cap *= 1.5;
  recipes = realloc(recipes, recipes_cap);
  if (recipes == NULL) die("out of memory");
}

void scribble() {
  for (int i = 0; i < recipes_len; ++i) {
    if (curr_a == i)
      printf("(%d)", (int)recipes[i]);
    else if(curr_b == i)
      printf("[%d]", (int)recipes[i]);
    else
      printf(" %d ", (int)recipes[i]);
  }
  printf("\n");
}

void scribble_last_from(int n, int from) {
  int top = from < recipes_len ? from : recipes_len;
  int bottom = top-n >= 0 ? top-n : 0; 
  for (int i = bottom; i < top; ++i) {
    printf("%d", (int)recipes[i]);
  }
  printf("\n");
}

void brew() {
  char new_recipe = recipes[curr_a] + recipes[curr_b];
  if (new_recipe >= 10) {
    recipes_reserve(recipes_len+2);
    recipes[recipes_len] = new_recipe / 10;
    recipes[recipes_len+1] = new_recipe % 10;
    recipes_len += 2;
  } else {
    recipes_reserve(recipes_len+1);
    recipes[recipes_len] = new_recipe;
    recipes_len += 1;
  }
  curr_a = (curr_a + recipes[curr_a] + 1) % recipes_len;
  curr_b = (curr_b + recipes[curr_b] + 1) % recipes_len;
}

void brew_num(int n) {
  while (recipes_len < n) {
    brew();
  }
}

int search(const char *big, int bigsize, const char* little, int littlesize) {
  for (int i = 0; i < bigsize; ++i) {
    if (big[i] == little[0]) {
      int j = 1;
      while (j < littlesize && big[i+j] == little[j]) {
        ++j;
      }
      if (j == littlesize) {
        return i;
      }
    }
  }
  return -1;
}

void part1(int argc, char** argv) {
  if (argc != 2) die("usage: day14-code <recipes>");
  char *end;
  int n = strtol(argv[1], &end, 10);
  if (end == argv[1]) die("<recipes> must be a number");
  
  recipes_reserve(2);
  recipes[0] = 3;
  recipes[1] = 7;
  recipes_len = 2;
  curr_a = 0;
  curr_b = 1;
  
  //scribble();
  while (recipes_len < n+10) {
    brew();
    //scribble();
  }
  scribble_last_from(10, n+10);
  free(recipes);
}

void part2(int argc, char** argv) {
  if (argc != 2) die("usage: day14-code <sequence>");

  char* p = argv[1];
  while (*p != '\0') {
    if (*p < '0' || *p > '9') die("sequence can only contain numbers 0-9");
    *p -= '0';
    ++p;
  }

  int sequence_len = p-argv[1];
  char *sequence = argv[1];
 
  recipes_reserve(2);
  recipes[0] = 3;
  recipes[1] = 7;
  recipes_len = 2;
  curr_a = 0;
  curr_b = 1;

  int offset = 0;
  while (1) {
    brew_num(recipes_len + 1024);
    int idx = search(&recipes[offset], recipes_len-offset, sequence, sequence_len);
    if (idx >= 0) {
      printf("sequence found after %d recipes\n", offset + idx);
      break;
    }
    offset = recipes_len;
  }
  free(recipes);
}

int main(int argc, char **argv) {
  // part1(argc, argv);
  part2(argc, argv);
}
