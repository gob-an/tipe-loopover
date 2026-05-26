#include "ida_star.h"
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

/*
    Generic multi-phase IDA* framework for cyclic NxN puzzles.

    Idea:

    phase 0 -> solve 2x2
    phase 1 -> solve 2x3
    phase 2 -> solve 3x3
    ...

    Each phase:

    - freezes a solved top-left block
    - restricts allowed generators
    - uses a partial heuristic

    This file assumes:

        tile
        SIZE
        N
        MAX_DEPTH
        Step

    are already defined in ida_star.h

    and that these functions exist:

        void L(tile*, int);
        void R(tile*, int);
        void U(tile*, int);
        void D(tile*, int);
*/

#define MAX_ALLOWED 64
#define MAX_PHASES  16


typedef struct {
    char move;
    int index;
} AllowedMove;


typedef struct {

    int solved_h;
    int solved_w;

    AllowedMove allowed[MAX_ALLOWED];
    int allowed_count;

} Phase;


static Step path[MAX_DEPTH];
static int solution_depth = 0;


/* ============================================================ */
/*                          MOVES                               */
/* ============================================================ */

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


/* ============================================================ */
/*                      PHASE HELPERS                           */
/* ============================================================ */

static bool solved_block(tile* b, int h, int w)
{
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            int idx = y * N + x;

            if (b[idx] != idx + 1)
                return false;
        }
    }

    return true;
}


/*
    Cyclic Manhattan distance restricted to the phase block.
*/
static int heuristic_block(tile* b, int h, int w)
{
    int H = 0;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            int idx = y * N + x;
            tile t = b[idx];

            if (t == 0)
                continue;

            int goal = t - 1;

            int gx = goal % N;
            int gy = goal / N;

            int dx = abs(x - gx);
            int dy = abs(y - gy);

            dx = dx < (N - dx) ? dx : (N - dx);
            dy = dy < (N - dy) ? dy : (N - dy);

            H += dx + dy;
        }
    }

    return H;
}


/* ============================================================ */
/*                        IDA* SEARCH                           */
/* ============================================================ */

static int search(tile* board,
                  int g,
                  int bound,
                  char prev_move,
                  const Phase* phase)
{
    if (g >= MAX_DEPTH)
        return INT_MAX;

    int h = heuristic_block(board,
                            phase->solved_h,
                            phase->solved_w);

    int f = g + h;

    if (f > bound)
        return f;

    if (solved_block(board,
                     phase->solved_h,
                     phase->solved_w)) {

        solution_depth = g;
        return -1;
    }

    int min = INT_MAX;

    for (int i = 0; i < phase->allowed_count; i++) {

        AllowedMove mv = phase->allowed[i];

        if (opposite(mv.move, prev_move))
            continue;

        apply_move(board, mv.move, mv.index);

        path[g].move  = mv.move;
        path[g].index = mv.index;

        int t = search(board,
                       g + 1,
                       bound,
                       mv.move,
                       phase);

        if (t == -1)
            return -1;

        if (t < min)
            min = t;

        undo_move(board, mv.move, mv.index);
    }

    return min;
}


static int ida_phase(tile* board,
                     Step* solution,
                     const Phase* phase)
{
    int bound = heuristic_block(board,
                                phase->solved_h,
                                phase->solved_w);

    while (1) {

        solution_depth = 0;

        int t = search(board,
                       0,
                       bound,
                       '\0',
                       phase);

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


/* ============================================================ */
/*                    PHASE CONSTRUCTION                        */
/* ============================================================ */

/*
    Build generators preserving an i*j solved block.

    Your theorem says:

        allowed:
            U_k, D_k for k >= i# multiphase.c

```c
#include "ida_star.h"
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

/*
    Generic multi-phase IDA* framework for cyclic NxN puzzles.

    Idea:

    phase 0 -> solve 2x2
    phase 1 -> solve 2x3
    phase 2 -> solve 3x3
    ...

    Each phase:

    - freezes a solved top-left block
    - restricts allowed generators
    - uses a partial heuristic

    This file assumes:

        tile
        SIZE
        N
        MAX_DEPTH
        Step

    are already defined in ida_star.h

    and that these functions exist:

        void L(tile*, int);
        void R(tile*, int);
        void U(tile*, int);
        void D(tile*, int);
*/

#define MAX_ALLOWED 64
#define MAX_PHASES  16


typedef struct {
    char move;
    int index;
} AllowedMove;


typedef struct {

    int solved_h;
    int solved_w;

    AllowedMove allowed[MAX_ALLOWED];
    int allowed_count;

} Phase;


static Step path[MAX_DEPTH];
static int solution_depth = 0;


/* ============================================================ */
/*                          MOVES                               */
/* ============================================================ */

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


/* ============================================================ */
/*                      PHASE HELPERS                           */
/* ============================================================ */

static bool solved_block(tile* b, int h, int w)
{
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            int idx = y * N + x;

            if (b[idx] != idx + 1)
                return false;
        }
    }

    return true;
}


/*
    Cyclic Manhattan distance restricted to the phase block.
*/
static int heuristic(tile* b, int m, int n)
{
    int h = 0;

    for (int i = 0; i < SIZE; i++) {
        tile t = b[i];
        if (t != 0 && t != i + 1 && (t-1)%N<=n && (t-1)/N<=m)
            h++;
    }

    return h;
}

static int search(tile* board,
                  int g,
                  int bound,
                  char prev_move,
                  const Phase* phase)
{
    if (g >= MAX_DEPTH)
        return INT_MAX;

    int h = heuristic_block(board,
                            phase->solved_h,
                            phase->solved_w);

    int f = g + h;

    if (f > bound)
        return f;

    if (solved_block(board,
                     phase->solved_h,
                     phase->solved_w)) {

        solution_depth = g;
        return -1;
    }

    int min = INT_MAX;

    for (int i = 0; i < phase->allowed_count; i++) {

        AllowedMove mv = phase->allowed[i];

        if (opposite(mv.move, prev_move))
            continue;

        apply_move(board, mv.move, mv.index);

        path[g].move  = mv.move;
        path[g].index = mv.index;

        int t = search(board,
                       g + 1,
                       bound,
                       mv.move,
                       phase);

        if (t == -1)
            return -1;

        if (t < min)
            min = t;

        undo_move(board, mv.move, mv.index);
    }

    return min;
}


static int ida_phase(tile* board,
                     Step* solution,
                     const Phase* phase)
{
    int bound = heuristic_block(board,
                                phase->solved_h,
                                phase->solved_w);

    while (1) {

        solution_depth = 0;

        int t = search(board,
                       0,
                       bound,
                       '\0',
                       phase);

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

static void build_phase(Phase* p,
                        int solved_h,
                        int solved_w)
{
    p->solved_h = solved_h;
    p->solved_w = solved_w;

    p->allowed_count = 0;

    /* rows >= solved_h */
    for (int row = solved_h; row < N; row++) {

        p->allowed[p->allowed_count++] =
            (AllowedMove){'L', row};

        p->allowed[p->allowed_count++] =
            (AllowedMove){'R', row};
    }

    /* cols >= solved_w */
    for (int col = solved_w; col < N; col++) {

        p->allowed[p->allowed_count++] =
            (AllowedMove){'U', col};

        p->allowed[p->allowed_count++] =
            (AllowedMove){'D', col};
    }
}

int multiphase_solve(tile* board,
                     Step* solution)
{
    Phase phases[MAX_PHASES];

    int phase_count = 0;

    build_phase(&phases[phase_count++], 2, 3);
    build_phase(&phases[phase_count++], 3, 4);
    build_phase(&phases[phase_count++], 5, 5);

    int total_steps = 0;

    for (int p = 0; p < phase_count; p++) {

        Step phase_solution[MAX_DEPTH];

        int len = ida_phase(board,
                            phase_solution,
                            &phases[p]);

        if (len < 0)
            return -1;

        /* apply solution to board */
        for (int i = 0; i < len; i++) {

            Step s = phase_solution[i];

            apply_move(board,
                       s.move,
                       s.index);

            solution[total_steps++] = s;
        }
    }

    return total_steps;
}
            L_l, R_l for l >= j
*/

static void build_phase(Phase* p,
                        int solved_h,
                        int solved_w)
{
    p->solved_h = solved_h;
    p->solved_w = solved_w;

    p->allowed_count = 0;

    /* rows >= solved_h */
    for (int row = solved_h; row < N; row++) {

        p->allowed[p->allowed_count++] =
            (AllowedMove){'L', row};

        p->allowed[p->allowed_count++] =
            (AllowedMove){'R', row};
    }

    /* cols >= solved_w */
    for (int col = solved_w; col < N; col++) {

        p->allowed[p->allowed_count++] =
            (AllowedMove){'U', col};

        p->allowed[p->allowed_count++] =
            (AllowedMove){'D', col};
    }
}

int multiphase_solve(tile* board,
                     Step* solution)
{
    Phase phases[MAX_PHASES];

    int phase_count = 0;

    /* ============================================================ */
    /*        AUTOMATIC PHASE CHAIN (2x3 -> NxN)                  */
    /* ============================================================ */

    int i = 2;
    int j = 3;

    /* generate stabilizer chain */
    while (i < N || j < N) {
        build_phase(&phases[phase_count++], i, j);

        if (i < N) i++;
        if (j < N) j++;
    }

    /* final full solve phase */
    build_phase(&phases[phase_count++], N, N);

    int total_steps = 0;

    for (int p = 0; p < phase_count; p++) {

        Step phase_solution[MAX_DEPTH];

        int len = ida_phase(board,
                            phase_solution,
                            &phases[p]);

        if (len < 0)
            return -1;

        /* apply solution to board */
        for (int i = 0; i < len; i++) {

            Step s = phase_solution[i];

            apply_move(board,
                       s.move,
                       s.index);

            solution[total_steps++] = s;
        }
    }

    return total_steps;
}
