#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define die(...) do{printf(__VA_ARGS__);puts("\n");abort();}while(0)

typedef union {
  struct i6n {
    int a, b, c, d, e, f;
  } named;
  int x[6];
} i6;

typedef struct {
  int opcode;
  int a, b, c;
} Instruction;

typedef void(*Op)(i6 *, int, int, int);

void addr(i6 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] + regs->x[b]; }
void addi(i6 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] + b; }
void mulr(i6 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] * regs->x[b]; }
void muli(i6 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] * b; }
void banr(i6 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] & regs->x[b]; }
void bani(i6 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] & b; }
void borr(i6 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] | regs->x[b]; }
void bori(i6 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] | b; }
void setr(i6 *regs, int a, int b, int c) { regs->x[c] = regs->x[a]; }
void seti(i6 *regs, int a, int b, int c) { regs->x[c] = a; }
void gtir(i6 *regs, int a, int b, int c) { regs->x[c] = a > regs->x[b]; }
void gtri(i6 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] > b; }
void gtrr(i6 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] > regs->x[b]; }
void eqir(i6 *regs, int a, int b, int c) { regs->x[c] = a == regs->x[b]; }
void eqri(i6 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] == b; }
void eqrr(i6 *regs, int a, int b, int c) { regs->x[c] = regs->x[a] == regs->x[b]; }

Op ops[16] = {
  addr, addi, mulr, muli, banr, bani, borr, bori,
  setr, seti, gtir, gtri, gtrr, eqir, eqri, eqrr
};

const char* opnames[16] = {
  "addr", "addi", "mulr", "muli", "banr", "bani", "borr", "bori",
  "setr", "seti", "gtir", "gtri", "gtrr", "eqir", "eqri", "eqrr"
};

int strtoop(const char* name) {
  if (strcmp(name, "addr") == 0) return 0;
  if (strcmp(name, "addi") == 0) return 1;
  if (strcmp(name, "mulr") == 0) return 2;
  if (strcmp(name, "muli") == 0) return 3;
  if (strcmp(name, "banr") == 0) return 4;
  if (strcmp(name, "bani") == 0) return 5;
  if (strcmp(name, "borr") == 0) return 6;
  if (strcmp(name, "bori") == 0) return 7;
  if (strcmp(name, "setr") == 0) return 8;
  if (strcmp(name, "seti") == 0) return 9;
  if (strcmp(name, "gtir") == 0) return 10;
  if (strcmp(name, "gtri") == 0) return 11;
  if (strcmp(name, "gtrr") == 0) return 12;
  if (strcmp(name, "eqir") == 0) return 13;
  if (strcmp(name, "eqri") == 0) return 14;
  if (strcmp(name, "eqrr") == 0) return 15;
  return -1;
}

#define MAXCODE 100

int main(int argc, char** argv) {
  if (argc != 2) die("usage: day19 <input>");

  FILE *input = fopen(argv[1], "r");
  if (input == NULL) die("unable to open file %s", argv[1]);

  Instruction code[MAXCODE];
  int code_len = 0;
  int ip = 0;
  int ipreg = 0;

  //i6 regs = {0, 0, 0, 0, 0, 0}; // PART 1
  i6 regs = {1, 0, 0, 0, 0, 0}; // PART 2

  char *line = NULL;
  size_t line_len;
  while (getline(&line, &line_len, input) > 0) {
    char name[5];
    int a, b, c;
    if (sscanf(line, "#ip %d \n", &ipreg) == 1) {
    } else if (sscanf(line, "%4s %d %d %d \n", name, &a, &b, &c) == 4) {
      int op = strtoop(name);
      if (op < 0) die("invalid instruction: %s", name);
      if (code_len == MAXCODE) die("program too long");
      code[code_len] = (Instruction){op, a, b, c};
      code_len += 1;
    } else if (line[0] == '/' && line[1] == '/') {
    } else
      die("invalid input");
  }
  free(line);
  fclose(input);

  printf("IP in register #%d\n", ipreg);
  printf("Program length: %d\n", code_len);

  //printf("Program:\n");
  //for (int i = 0; i < code_len; ++i) {
  //  Instruction *in = &code[i];
  //  printf("%d: %s %d %d %d\n", i, opnames[in->opcode], in->a, in->b, in->c);
  //}
  //printf("Program:");

  while (ip >= 0 && ip < code_len) {
    regs.x[ipreg] = ip;
    Instruction *i = &code[ip];
    printf("ip=%d [%d, %d, %d, %d, %d, %d] %s %d %d %d\n",
        ip, regs.x[0], regs.x[1], regs.x[2], regs.x[3], regs.x[4], regs.x[5],
        opnames[i->opcode], i->a, i->b, i->c);
    Op op = ops[i->opcode];
    op(&regs, i->a, i->b, i->c);
    ip = regs.x[ipreg];
    ip += 1;
    getchar();
  }

  printf("reg after halt: [%d, %d, %d, %d, %d, %d]\n",
        regs.x[0], regs.x[1], regs.x[2], regs.x[3], regs.x[4], regs.x[5]);
}
