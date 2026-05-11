#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#define N 3
#define SIZE N*N
#define IDX(i, j) ((j)*N+i)

void print(int*);

void R(int*, int);
void L(int*, int);
void D(int*, int);
void U(int*, int);

bool solved(int*);
void scram(int*);
void play(int*);

#endif
