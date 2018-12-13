#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define die(...) do{printf(__VA_ARGS__);puts("\n");exit(1);}while(0)

typedef struct Crash {
  int x, y;
} Crash;

typedef struct Cart {
  int x, y;
  char dir;
  int turn;
  int alive;
} Cart;

#define MAXCARTS 256
typedef struct Carts {
  int num;
  Cart carts[MAXCARTS];
} Carts;

#define MAXROWS 256
typedef struct Tracks {
  int num;
  char* rows[MAXROWS];
} Tracks;

char check_cart(char ch) {
  switch (ch) {
  case '^':
  case '>':
  case 'v':
  case '<':
    return ch;
  default: die("invalid cart: '%c'", ch);
  }
}

void add_cart(char ch, int x, int y, Carts *carts) {
  int dir = 0;
  if (carts->num == MAXCARTS) die("too many carts");
  carts->carts[carts->num] = (Cart){x, y, check_cart(ch), /*turn*/0, /*alive*/1};
  carts->num += 1;
}

void add_tracks_row(char* row, Tracks *tracks) {
  if (tracks->num == MAXROWS) die("two many rows in tracks");
  tracks->rows[tracks->num] = row;
  tracks->num += 1;
}

/*
int ch_to_dir(char ch) {
  switch (ch) {
  case '^': return 0;
  case '>': return 1;
  case 'v': return 3;
  case '<': return 4;
  default: return -1;
  }
}

char dir_to_ch(int dir) {
  switch (dir) {
  case 0: return '^';
  case 1: return '>';
  case 2: return 'v';
  case 3: return '<';
  default: return '#';
  }
}
*/

int compare_carts(const void* a, const void* b) {
  Cart *ca = (Cart*)a;
  Cart *cb = (Cart*)b;
  if (ca->alive && !cb->alive) return -1;
  else if (!ca->alive && cb->alive) return 1;
  else if (ca->y < cb->y) return -1;
  else if (ca->y > cb->y) return 1;
  else if (ca->x < cb->x) return -1;
  else if (ca->x > cb->x) return 1;
  else return 0;
}

char turn(char ch, int dir) {
  if (dir == 1) {
    return ch;
  }
  if (dir == 0) {
    switch (ch) {
      case '^': return '<';
      case '>': return '^';
      case 'v': return '>';
      case '<': return 'v';
      default: return '#';
    }
  }
  if (dir == 2) {
    switch (ch) {
      case '^': return '>';
      case '>': return 'v';
      case 'v': return '<';
      case '<': return '^';
      default: return '#';
    }
  }
  return '#';
}

void cart_follow_track(Cart *cart, char track) {
  char ch = cart->dir;
  if (track == '+') {
    cart->dir = turn(ch, cart->turn);
    cart->turn = (cart->turn+1)%3;
  } else if (track == '/' && (ch == '<' || ch == '>')) {
    cart->dir = turn(ch, 0);
  } else if (track == '/' && (ch == '^' || ch == 'v')) {
    cart->dir = turn(ch, 2);
  } else if (track == '\\' && (ch == '<' || ch == '>')) {
    cart->dir = turn(ch, 2);
  } else if (track == '\\' && (ch == '^' || ch == 'v')) {
    cart->dir = turn(ch, 0);
  } else if (track == '-' && (ch == '<' || ch == '>')) {
    // nothing
  } else if (track == '|' && (ch == '^' || ch == 'v')) {
    // nothing
  } else {
    die("cart in impossible situation");
  }
}

void cart_move(Cart *cart) {
  switch (cart->dir) {
    case '^': cart->y -= 1; break;
    case '>': cart->x += 1; break;
    case 'v': cart->y += 1; break;
    case '<': cart->x -= 1; break;
    default: die("invalid card: '%c'", cart->dir);
  }
}

// requires carts to be sorted by coordinates
// returns number of crashes
int carts_step(Tracks *tracks, Carts *carts, Crash *crash) {
  int crashes = 0;
  for (int i = 0; i < carts->num; ++i) {
    Cart *cart = &carts->carts[i];
    if (!cart->alive) continue;

    cart_move(cart);
    char *row = tracks->rows[cart->y];
    cart_follow_track(cart, row[cart->x]);
    for (int j = 0; j < carts->num; ++j) {
      if (i == j) continue;
      Cart *other = &carts->carts[j];
      if (compare_carts(cart, other) == 0) {
        crash->x = cart->x;
        crash->y = cart->y;
        cart->alive = 0;
        other->alive = 0;
        crashes += 1;
      }
    }
  }
  return crashes;
}

int carts_alive(Carts *carts) {
  int alive = 0;
  for (int i = 0; i < carts->num; ++i) {
    if (carts->carts[i].alive) alive += 1;
  }
  return alive;
}

void print_tracks(Tracks *tracks, Carts *carts, Crash *crash) {
  for (int y = 0; y < tracks->num; ++y) {
    char *row = tracks->rows[y];
    int x = 0;
    while (row[x] != '\0') {
      char ch = row[x];

      for (int j = 0; j < carts->num; ++j) {
        Cart *cart = &carts->carts[j];
        if (cart->x == x && cart->y == y && cart->alive) {
          ch = cart->dir;
        }
      }

      // crash has priority
      if (crash->x == x && crash->y == y) {
        ch = 'X';
      }
      putchar(ch);
      x += 1;
    }
    putchar('\n');
  }
}

int main(int argc, char** argv) {
  FILE *input = stdin;
  if (argc == 2) {
    input = fopen(argv[1], "r");
    if (input == NULL) die("unable to open file '%s'", argv[1]);
  }

  Tracks tracks;
  tracks.num = 0;

  Carts carts;
  carts.num = 0;

  Crash crash = {-1, -1};

  char* line = NULL;
  size_t len = 0;

  // read input
  while (getline(&line, &len, input) > 0) {

    int y = tracks.num;
    char* p = line;
    while (*p != '\0') {
      int x = (int) (p - line);
      switch (*p) {
      case '^':
      case 'v':
        add_cart(*p, x, y, &carts);
        *p = '|';
        break;
      case '>':
      case '<':
        add_cart(*p, x, y, &carts);
        *p = '-';
        break;
      case '\n':
        *p = '\0';
        break;
      }
      ++p;
    }
    add_tracks_row(line, &tracks);

    line = NULL;
  }
  fclose(input);

  int crashes = 0;
  int alive = carts_alive(&carts);
  //print_tracks(&tracks, &carts, &crash);
  //printf("carts alive: %d\n", alive);

  for (int i = 0; alive > 1; ++i) {
    //getchar();
    qsort(&carts.carts, carts.num, sizeof(Cart), compare_carts);
    crashes += carts_step(&tracks, &carts, &crash);
    alive = carts_alive(&carts);
    //print_tracks(&tracks, &carts, &crash);
    //printf("carts alive: %d\n", alive);
  }

  printf("location of crash: %d,%d\n", crash.x, crash.y);
  for (int i = 0; i < carts.num; ++i) {
    if (carts.carts[i].alive) {
      printf("first alive cart @ %d,%d\n", carts.carts[i].x, carts.carts[i].y);
      break;
    }
  }
}
