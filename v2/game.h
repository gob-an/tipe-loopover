#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#define N 5
#define SIZE (N*N)
#define IDX(i, j) ((i)*N+j)
typedef uint8_t tile;

#define IS_INT(x) _Generic((x), \ int: true, \ default: false)
#define IS_CHAR(x) _Generic((x), \ char: true, \ default: false)

void print(tile*);

void R(tile*, int);
void L(tile*, int);
void D(tile*, int);
void U(tile*, int);

tile* Rcpy(tile*, int);
tile* Lcpy(tile*, int);
tile* Dcpy(tile*, int);
tile* Ucpy(tile*, int);

bool solved(tile*);
void scram(tile*);
tile* solved_board(void);
void play(tile*);

#endif
