#include "game.h"

char* color (int n, int k) { //works great for n=5
  if (k == 00 || k == 01 || k == 10 || k == 11) {return "\033[41m";}
  if (k == 03 || k == 04 || k == 13 || k == 14) {return "\033[44m";}
  if (k == 33 || k == 34 || k == 43 || k == 44) {return "\033[106m";}
  if (k == 30 || k == 31 || k == 40 || k == 41) {return "\033[103m";}
  if (k == 22) {return "\033[100m";}
  if (k == 02 || k == 12) {return "\033[45m";}
  if (k == 20 || k == 21) {return "\033[43m";}
  if (k == 23 || k == 24) {return "\033[104m";}
  if (k == 32 || k == 42) {return "\033[102m";}
  return "";
}



int* board (int n) {
  int* r = malloc(n*n*sizeof(int));
  for (int i=0; i<n; i++) {
    for (int j = 0; j<n; j++) {
      r[IDX(n,i,j)]=10*i+j;
    }
  } return r;
}


void printb (int* b, int n) {
  printf(" +");
  for (int i = 0; i<n; i++) {printf(" -- ");}
  printf("+\n");
  for (int i=0; i<n; i++) {
    printf(" |");
    for (int j = 0; j<n; j++) {
      int k = b[IDX(n,i,j)];
      printf("%s", color(n,k));
      printf(" ");
      if (k<10) {printf("0");}
      printf("%d ", k);
      printf("\033[m");
    } printf("|\n");
  }
  printf(" +");
  for (int i = 0; i<n; i++) {printf(" -- ");}
  printf("+\n\n\n");
}



void left (int* b, int n, int r) {
  int m = b[IDX(n, r, 0)];
  for (int j=1; j<n; j++) {
    b[IDX(n, r, j-1)] = b[IDX(n, r, j)];
  }
  b[IDX(n, r, n-1)] = m;
}

void right (int* b, int n, int r) {
  int m = b[IDX(n, r, n-1)];
  for (int j=n-2; j>-1; j--) {
    b[IDX(n, r, j+1)] = b[IDX(n, r, j)];
  }
  b[IDX(n, r, 0)] = m;
}

void up (int* b, int n, int c) {
  int m = b[IDX(n, 0, c)];
  for (int j=1; j<n; j++) {
    b[IDX(n, j-1, c)] = b[IDX(n, j, c)];
  }
  b[IDX(n, n-1, c)] = m;
}

void down (int* b, int n, int c) {
  int m = b[IDX(n, n-1, c)];
  for (int j=n-2; j>-1; j--) {
    b[IDX(n, j+1, c)] = b[IDX(n, j, c)];
  }
  b[IDX(n, 0, c)] = m;
}






int* random_perm(int n, bool sign) { // for scramble, not working
  int* r = board(n);
  int s = 1;

  for (int i = 0; i<n*n; i++) {
    int rd = rand() % (n*n);
    int m=r[i];
    r[i]=r[rd];
    r[rd]=m;
    if (rd!=i) {s*=-1;}
  }
  if (s==-1) {
    int m = r[0];
    r[0] = r[1];
    r[1]=m;
  }
  return r;
}





void printbp (int* b, int n, int x, int y) {

  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n +");
  for (int j = 0; j<n; j++) {
    if (j == x) {printf(" vv ");}
    else {printf(" -- ");}
  }
  printf("+\n");
  for (int i=0; i<n; i++) {
    if (i==y) {printf(" >");}
    else {printf(" |");}

    for (int j = 0; j<n; j++) {
      int k = b[IDX(n,i,j)];
      printf("%s", color(n,k));
      printf(" ");
      if (k<10) {printf("0");}
      printf("%d ", k);
      printf("\033[m");
    }

    if (i==y) {printf("<\n");}
    else {printf("|\n");}
  }

  printf(" +");
  for (int j = 0; j<n; j++) {
    if (j == x) {printf(" ^^ ");}
    else {printf(" -- ");}
  } printf("+\n\n");
}



void play (int* b, int n) {
  char m = '\0';
  int x = 2; int y = 2;
  while (true) {

    if (m!='\n') {printbp(b, n, x, y);}

    scanf("%c", &m);

    if (m=='a') {
      free(b);
      b = random_perm(n, true);
    }


    if (m=='r') {x = (x+1)%n;}
    if (m=='l') {x = (x+4)%n;}
    if (m=='d') {y = (y+1)%n;}
    if (m=='u') {y = (y+4)%n;}

    if (m=='R') {right(b,n,y);}
    if (m=='L') {left(b,n,y);}
    if (m=='D') {down(b,n,x);}
    if (m=='U') {up(b,n,x);}

    if (win(b, n)) {
      printf("trop forte!\n");
    }

    if (m=='Q') {break;}
  }
}

bool win (int* b, int n) {
  for (int i=0; i<n*n; i++) {
    if (b[i] != solved[i]) {return false;}
  } return true;
}

int main () {
  int* b = board(5);

  play(b, 5);

  free(b);
}
