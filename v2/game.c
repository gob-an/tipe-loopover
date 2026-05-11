#include "game.h"

// nom générique: rangée

void print (int* b) {
  printf(" +");
  for (int i = 0; i<N; i++) {printf(" -- ");}
  printf("+\n");
  for (int i = 0; i < N; i++) {
    printf(" |");
    for (int j = 0; j < N; j++) {
      printf(" %2d ", b[IDX(i, j)]);
    } printf("|\n");
  }
  printf(" +");
  for (int i = 0; i<N; i++) {printf(" -- ");}
  printf("+\n\n\n");
}


void R (int* b, int ln) {
    int tmp = b[IDX(ln, N-1)];
    for (int j = N-1; j > 0; j--) {
        b[IDX(ln, j)] = b[IDX(ln, j-1)];
    }
    b[IDX(ln, 0)] = tmp;
}
void L (int* b, int ln) {
    int tmp = b[IDX(ln, 0)];
    for (int j = 0; j < N-1; j++) {
        b[IDX(ln, j)] = b[IDX(ln, j+1)];
    }
    b[IDX(ln, N-1)] = tmp;
}
void D (int* b, int col) {
    int tmp = b[IDX(N-1, col)];
    for (int i = N-1; i > 0; i--) {
        b[IDX(i, col)] = b[IDX(i-1, col)];
    }
    b[IDX(0, col)] = tmp;
}
void U (int* b, int col) {
    int tmp = b[IDX(0, col)];
    for (int i = 0; i < N-1; i++) {
        b[IDX(i, col)] = b[IDX(i+1, col)];
    }
    b[IDX(N-1, col)] = tmp;
}

bool solved (int* b) {
  for (int i=0; i<N*N; i++) {
    if (b[i]!=i+1 && b[i]!=0) return false;
  }
  return true;
}

void scram (int* b) {
  int s = 1;

  for (int i = 0; i<N*N; i++) {
    int rd = rand() % (N*N);
    int m=b[i];
    b[i]=b[rd];
    b[rd]=m;
    if (rd!=i) {s*=-1;}
  }
  if (s==-1 && N%2==1) {
    int m = b[0];
    b[0] = b[1];
    b[1]=m;
  }
}

void play (int* b) {
  char cmd;
  int i;

  while (true) {
    print(b);
    printf("command (L/R/U/D + coord, q to quit):");

    scanf(" %c", &cmd);
    if (cmd == 'q') break;
    scanf("%d", &i);

    if (i < 1 || i > N) {
      printf("invalid index\n");
      continue;
    }

    switch (cmd) {
      case 'L': L(b, i-1); break;
      case 'R': R(b, i-1); break;
      case 'U': U(b, i-1); break;
      case 'D': D(b, i-1); break;
      case 'h': printf("L/R/U/D + coord \n q to quit \n h for help \n s to scram"); break;
      case 's': scram(b); break;
      default: printf("Commande inconnue\n");
    }

    if (solved(b)) printf("yay!\n");
  }
}
