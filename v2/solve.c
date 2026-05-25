#include "game.h"
#include "ida_star.h"

#include <time.h>

int main(void) {

    srand(time(NULL));

    tile* board = solved_board();
    scram(board);
    tile* copy = malloc(SIZE * sizeof(tile));; memcpy(copy, board, SIZE*sizeof(tile));

    print(board);

    Step solution[MAX_DEPTH];

    clock_t start = clock();

    int len = ida_star(board, solution);

    clock_t end = clock();

    double elapsed =
        (double)(end - start)
        / CLOCKS_PER_SEC;

    if (len < 0) {

        printf("No solution found\n");
        return 0;
    }

    printf(
        "Solved in %d moves\n",
        len
    );

    printf(
        "Time: %.6f seconds\n\n",
        elapsed
    );

    for (int i = 0; i < len; i++) {

        printf(
            "%c%d ",
            solution[i].move,
            (int) solution[i].index + 1
        );
    } printf("\n");

    play(copy);

    free(board); free(copy);
    return 0;
}
