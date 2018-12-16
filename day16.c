#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define die(...) do{printf(__VA_ARGS__);puts("\n");abort();}while(0)

typedef union {
  struct i4n {
    int a, b, c, d;
  } named;
  int x[4];
} i4;

typedef void(*Inst)(i4 *, int, int, int);

void addr(i4 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] + regs->x[b]; }
void addi(i4 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] + b; }
void mulr(i4 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] * regs->x[b]; }
void muli(i4 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] * b; }
void banr(i4 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] & regs->x[b]; }
void bani(i4 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] & b; }
void borr(i4 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] | regs->x[b]; }
void bori(i4 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] | b; }
void setr(i4 *regs, int a, int b, int c) { regs->x[c] = regs->x[a]; }
void seti(i4 *regs, int a, int b, int c) { regs->x[c] = a; }
void gtir(i4 *regs, int a, int b, int c) { regs->x[c] = a > regs->x[b]; }
void gtri(i4 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] > b; }
void gtrr(i4 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] > regs->x[b]; }
void eqir(i4 *regs, int a, int b, int c) { regs->x[c] = a == regs->x[b]; }
void eqri(i4 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] == b; }
void eqrr(i4 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] == regs->x[b]; }

Inst insts[16] = {
  addr, addi, mulr, muli, banr, bani, borr, bori,
  setr, seti, gtir, gtri, gtrr, eqir, eqri, eqrr
};

int match(i4 before, i4 inst, i4 after, Inst op) {
  op(&before, inst.x[1], inst.x[2], inst.x[3]);
  return before.x[0] == after.x[0] && before.x[1] == after.x[1] &&
      before.x[2] == after.x[2] && before.x[3] == after.x[3];
}

int num_matches(i4 before, i4 inst, i4 after) {
  int matches = 0;
  for (int i = 0; i < 16; ++i) {
    matches += match(before, inst, after, insts[i]);
  }
  return matches;
}

uint16_t maybes[16];
int opcodes[16];
void init_maybes() {
  for (int i = 0; i < 16; ++i) {
    maybes[i] = 0xffff;
    opcodes[i] = -1;
  }
}
void add_sample(i4 before, i4 inst, i4 after) {
  uint16_t matches = 0;
  int op = inst.x[0];
  for (int i = 0; i < 16; ++i) {
    matches = matches | (match(before, inst, after, insts[i]) << i);
  }
  maybes[op] = maybes[op] & matches;
}
void resolve_maybes() {
  while (1) {
    // look for an unambiguous instruction
    uint16_t opcode = 0;
    uint16_t mask = 0;
    for (int i = 0; i < 16; ++i) {
      // if instruction is unambiguous, set and remember opcode
      if (__builtin_popcount(maybes[i]) == 1) {
        opcode = __builtin_ctz(maybes[i]);
        mask = ~(maybes[i]);
        opcodes[i] = opcode;
        break;
      }
    }
    if (mask == 0) {
      break;
    } else {
      // delete opcode from all other candidates
      for (int i = 0; i < 16; ++i) {
        maybes[i] = maybes[i] & mask;
      }
    }
  }
}
void print_maybes() {
  for (int i = 0; i < 16; ++i) {
    printf("0x%04x\n", maybes[i]);
  }
}
void print_opcodes() {
  for (int i = 0; i < 16; ++i) {
    printf("0x%04x\n", opcodes[i]);
  }
}

int main() {
  i4 regs;
  i4 inst;

  int state = 0; // 0: before, 1: inst, 1: after
  char *line = NULL;
  size_t size;
  int three_or_more = 0;
  init_maybes();
  while (getline(&line, &size, stdin) > 0) {
    int a, b, c, d;

    if (state == 0 && sscanf(line, "Before: [%d, %d, %d, %d]", &a, &b, &c, &d) == 4) {
      regs = (i4){.named = {a, b, c, d}};
      state = 1;
      continue;
    }

    if (state == 1 && sscanf(line, "%d %d %d %d", &a, &b, &c, &d) == 4) {
      inst = (i4){.named = {a, b, c, d}};
      state = 2;
      continue;
    }

    if (state == 2 && sscanf(line, "After: [%d, %d, %d, %d]", &a, &b, &c, &d) == 4) {
      i4 after = (i4){.named = {a, b, c, d}};
      state = 0;
      three_or_more += num_matches(regs, inst, after) >= 3;
      add_sample(regs, inst, after);
      continue;
    }

    // if nothing matched, check if line was empty, otherwise this
    // is where the program starts
    char* p = line;
    while (*p == ' ' || *p == '\t' || *p == '\n') ++p;
    if (*p != '\0') break;
  }
  printf("three or more matches: %d\n", three_or_more);

  // solve maybe opcodes into definitive ones (one is always unambiguous)
  resolve_maybes();

  // interpret program
  regs = (i4){.named = {0, 0, 0, 0}};
  do {
    int a, b, c, d;
    if (sscanf(line, "%d %d %d %d", &a, &b, &c, &d)) {
      Inst op = insts[opcodes[a]];
      op(&regs, b, c, d);
    }
  } while (getline(&line, &size, stdin) > 0);
  printf("regs[0] = %d\n", regs.x[0]);
}
