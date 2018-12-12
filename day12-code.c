#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define die(...) do{printf(__VA_ARGS__);puts("\n");exit(1);}while(0)

int empty_or_comment(const char* s) {
  while (*s == ' ' || *s == '\t') ++s;
  return *s == '\n' || (*s == '/' && *(s+1) == '/');
}

#define RADIUS 2

typedef struct Rule {
  char* in;
  char out;
} Rule;

typedef struct Row {
  char* row;
  int len;
  int left;
} Row;

int apply_rule(Row rowin, Row rowout, Rule rule) {
  int num = 0;
  for (int i = 0; i < rowin.len; ++i) {
    int match = 1;
    for (int j = 0; j < 2 * RADIUS + 1; ++j) {
      char plant = '.';
      int idx = i - RADIUS + j;
      if (idx >= 0 && idx < rowin.len) plant = rowin.row[idx];
      match &= (plant == rule.in[j]);
    }
    if (!match)
      continue;
    num += 1;
    rowout.row[RADIUS+i] = rule.out;
  }
  return num;
}

int next_row(Row rowin, Row *rowout) {
  rowout->row = realloc(rowout->row, rowin.len + 2 * RADIUS + 1);
  if (rowout->row == NULL) die("out of memory");
  rowout->len = rowin.len + 2 * RADIUS;
  rowout->left = rowin.left - RADIUS;
  for (int i = 0; i < rowout->len; ++i) {
    rowout->row[i] = '.';
  }
  rowout->row[rowout->len] = '\0';
  return 0;
}

int compare_rules(const void* left,const void* right) {
  Rule *a = (Rule*) left;
  Rule *b = (Rule*) right;
  return strcmp(a->in, b->in);
}

int main(int argc, char** argv) {
  FILE *input = stdin;
  if (argc == 2) {
    input = fopen(argv[1], "r");
    if (input == NULL) die("unable to open file '%s'", argv[1]);
  }

  char* line = NULL;
  size_t len = 0;
  
  Row row_back = {NULL, 0, 0};
  Row row_front = {NULL, 0, 0};
  Rule rules[32];
  int nrules = 0;

  /////////////////////////////////////////////////////////
  // read input
  while (getline(&line, &len, input) > 0) {
    if (empty_or_comment(line)) continue;
    char *pattern = NULL;
    if (sscanf(line, " initial state: %m[.#] ", &row_front.row) == 1) {
      row_front.len = strlen(row_front.row);
    } else if (sscanf(line, " %m[.#] => %c ",
          &rules[nrules].in, &rules[nrules].out) == 2) {
      nrules += 1;
    }
  }
  // sort rules for debugging
  qsort(rules, 32, sizeof(Rule), compare_rules);
  printf("nrules: %d\n", nrules);
  printf("initial: %s\n", row_front.row);
  for (int i = 0; i < nrules; ++i) {
    printf("rule: %s --> %c\n", rules[i].in, rules[i].out);
  }

  /////////////////////////////////////////////////////////
  // simulate
  //printf("    % *s          1         2         3\n", ngens*RADIUS, "");
  //printf("    % *s0         0         0         0\n", ngens*RADIUS, "");

  int ngens = 20;
  for (int i = 0; i < ngens; ++i) {
    printf("%2d: % *s%s\n", i, (i-ngens)*RADIUS, "", row_front.row);
    next_row(row_front, &row_back);
    for (int j = 0; j < nrules; ++j) {
      apply_rule(row_front, row_back, rules[j]);
    }
    Row tmp = row_back;
    row_back = row_front;
    row_front = tmp;
  }
  printf("%2d: % *s%s\n", ngens, (ngens-ngens)*RADIUS, "", row_front.row);

  int sum = 0;
  for (int i = 0; i < row_front.len; ++i) {
    if (row_front.row[i] == '#') {
      sum += i + row_front.left;
      printf("%d ", i + row_front.left);
    }
  }
  printf("\n");
  printf("sum: %d\n", sum);

  free(row_back.row);
  free(row_front.row);

  fclose(input);
}
