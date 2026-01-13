#ifndef _IDASTAR_H_
#define _IDASTAR_H_

#include "game.h"

typedef struct state {
  Board b;
  int h;
} State;

typedef struct move {
  char move;
  int i;
} Move;
