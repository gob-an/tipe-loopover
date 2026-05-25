#ifndef IDA_STAR_H
#define IDA_STAR_H

#include "game.h"
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_DEPTH 128

typedef struct {
    char move;
    int index;
} Step;

/* lance l'IDA* */
int ida_star(tile* start, Step* solution);

#endif
