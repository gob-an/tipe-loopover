#ifndef CUBE_H
#define CUBE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#define IDX(n, i, j) ((j)*n+i)

typedef struct b {
  int* Board
  int n;
  int m;
} Board;

typedef struct move {
  void m (int*, int, int);
  int pos;
  int n;
} Move;

const int solved[25] = {0, 10, 20, 30, 40, 1, 11, 21, 31, 41, 2, 12, 22, 32, 42, 3, 13, 23, 33, 43, 4, 14, 24, 34, 44};

char* color (int, int);
int* board (int);
void printb (int*, int);

void left (int*, int, int);
void right (int*, int, int);
void up (int*, int, int);
void down (int*, int, int);

int* random_perm (int, bool);

bool win (int*, int);

void printbp (int*, int, int, int);
void play (int*, int);

#endif
