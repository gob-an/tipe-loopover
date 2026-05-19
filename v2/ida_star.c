#include "solver.h"
#include <string.h>
#include <limits.h>

static Step path[MAX_DEPTH];

static void apply_move(tile* b, char move, int idx) {
    switch(move) {
        case 'L': L(b, idx); break;
        case 'R': R(b, idx); break;
        case 'U': U(b, idx); break;
        case 'D': D(b, idx); break;
    }
}

static void undo_move(tile* b, char move, int idx) {
    switch(move) {
        case 'L': R(b, idx); break;
        case 'R': L(b, idx); break;
        case 'U': D(b, idx); break;
        case 'D': U(b, idx); break;
    }
}

int heuristic(tile* b) {
    int h = 0;

    for (int i = 0; i < SIZE; i++) {
        if (i == SIZE - 1) {
            if (b[i] != 0)
                h++;
        }
        else {
            if (b[i] != i + 1)
                h++;
        }
    }

    return h;
}

static bool opposite(char a, char b) {
    return (a == 'L' && b == 'R') ||
           (a == 'R' && b == 'L') ||
           (a == 'U' && b == 'D') ||
           (a == 'D' && b == 'U');
}

static int search(
    tile* board,
    int g,
    int bound,
    char prev_move
) {
    int h = heuristic(board);
    int f = g + h;

    if (f > bound)
        return f;

    if (solved(board))
        return FOUND;

    int min = INT_MAX;

    char moves[4] = {'L', 'R', 'U', 'D'};

    for (int m = 0; m < 4; m++) {
        char move = moves[m];

        if (opposite(move, prev_move))
            continue;

        for (int idx = 0; idx < N; idx++) {

            apply_move(board, move, idx);

            path[g].move = move;
            path[g].index = idx;

            int t = search(board, g + 1, bound, move);

            if (t == FOUND)
                return FOUND;

            if (t < min)
                min = t;
}
