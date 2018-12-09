#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct Marble {
  int number;
  struct Marble* prev;
  struct Marble* next;
} Marble;

#define check_vital(cond) do{if(!(cond)){printf(#cond " failed!\n");exit(1);}}while(0)

Marble* insert_right(Marble *root, int idx, int num) {
  for (int i = 0; i < idx; ++i) {
    root = root->next;
  }
  Marble *next = root->next;
  Marble *new_marble = malloc(sizeof(Marble));

  check_vital(new_marble != NULL);
  new_marble->number = num;
  new_marble->next = next;
  new_marble->prev = root;
  next->prev = new_marble;
  root->next = new_marble;
  return new_marble;
}

Marble* pop_left(Marble* root, int idx) {
  for (int i = 0; i < idx; ++i) {
    root = root->prev;
  }
  Marble *next = root->next;
  Marble *prev = root->prev;
  prev->next = next;
  next->prev = prev;
  return root;
}

void print_marbles(Marble* root, Marble *current) {
  Marble* it = root;
  do {
    if (it != root) printf(" ");
    if (it == current) printf("(%d)", it->number);
    else printf("%d", it->number);
    it = it->next;
  } while(it != root);
  printf("\n");
}

int main(int argc, char** argv) {

  if (argc != 3){
    printf("usage: %s players marbles\n", argv[0]);
    exit(1);
  }

  char* end;
  int players = strtol(argv[1], &end, 10);
  if (end == argv[1]) {
    printf("%s not a number\n", argv[1]);
  }
  int maxmarbles = strtol(argv[2], &end, 10);
  if (end == argv[2]) {
    printf("%s not a number\n", argv[2]);
  }

  Marble *root = malloc(sizeof(Marble));
  check_vital(root != NULL);
  root->number = 0;
  root->prev = root;
  root->next = root;

  int64_t *scores = malloc(sizeof(int64_t[players]));
  for (int i = 0; i < players; ++i) {
    scores[i] = 0;
  }

  Marble *current = root;

  //print_marbles(root, current);
  for (int i = 1; i <= maxmarbles; ++i) {
    if (i % 23 != 0) {
      current = insert_right(current, 1, i);
    } else {
      Marble *picked = pop_left(current, 7);
      int64_t points = i + picked->number;
      current = picked->next;
      free(picked);
      scores[(i-1)%players] += points;
    }
    //print_marbles(root, current);
  }

  int64_t maxpoints = 0;
  for (int i = 0; i < players; ++i) {
    maxpoints = scores[i] > maxpoints? scores[i] : maxpoints;
  }

  printf("max points: %" PRId64 "\n", maxpoints);

  current = root;
  do {
    Marble* tmp = current;
    current = current->next;
    free(tmp);
  } while (current != root);
  free(scores);

}
