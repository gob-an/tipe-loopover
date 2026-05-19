#ifndef SOLVER_H
#define SOLVER_H

#include "game.h"

#define MAX_DEPTH 128
#define FOUND (-1)

typedef struct {
    char move;
    int index;
} Step;

int heuristic(tile* b);
int ida_star(tile* start, Step* solution);

#endif
