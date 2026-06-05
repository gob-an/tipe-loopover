#include "multiphase.h"

bool solved_block (tile* b, int p, int q) {
  for (int i=0; i<p; i++) {
    for (int j=0; j<q; j++) {
      if (b[IDX(i, j)]!=IDX(i,j)+1 && b[IDX(i,j)]!=0) return false; // gérer le polymorphisme
    }
  }
  return true;
}

static int heuristic_block(tile* b, int p, int q) {
    int h = 0;

    for (int i = 0; i < SIZE; i++) {
        tile t = b[i];
        if (t != 0 && t != i + 1 && t%N<p && t/N<q)
            h++;
    }

    return h;
}


static void apply_move(tile* b, char move, int idx)
{
    switch (move) {
        case 'L': L(b, idx); break;
        case 'R': R(b, idx); break;
        case 'U': U(b, idx); break;
        case 'D': D(b, idx); break;
    }
}

static void undo_move(tile* b, char move, int idx)
{
    switch (move) {
        case 'L': R(b, idx); break;
        case 'R': L(b, idx); break;
        case 'U': D(b, idx); break;
        case 'D': U(b, idx); break;
    }
}

static bool opposite(char a, char b)
{
    return (a == 'L' && b == 'R') ||
           (a == 'R' && b == 'L') ||
           (a == 'U' && b == 'D') ||
           (a == 'D' && b == 'U');
}


static Step path[MAX_DEPTH];
static int solution_depth = 0;


static int search_block(tile* board,
                  int g,
                  int bound,
                  char prev_move, int p, int q)
{
    int f = g + 2*heuristic_block(board, p, q);

    if (f > bound)
        return f;

    if (solved_block(board, p, q)) {
        solution_depth = g;
        return -1; // FOUND
    }

    int min = INT_MAX;

    const char moves[4] = {'L','R','U','D'};

    for (int m = 0; m < 4; m++) {

        char move = moves[m];

        if (opposite(move, prev_move))
            continue;

        for (int idx = 0; idx < N; idx++) {

            apply_move(board, move, idx);

            path[g].move = move;
            path[g].index = idx;

            int t = search_block(board, g + 1, bound, move, p, q);

            undo_move(board, move, idx);

            if (t == -1)
                return -1;

            if (t < min)
                min = t;
        }
    }

    return min;
}


int ida_star_block(tile* start, Step* solution, int p, int q)
{
    int bound = heuristic_block(start, p, q);

    while (1) {

        solution_depth = 0;

        int t = search_block(start, 0, bound, '\0', p, q);

        if (t == -1) {

            for (int i = 0; i < solution_depth; i++)
                solution[i] = path[i];

            return solution_depth;
        }

        if (t == INT_MAX)
            return -1;

        bound = t;
    }
}
