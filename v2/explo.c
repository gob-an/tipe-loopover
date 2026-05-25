#include "game.h"
#include "ida_star.h"

#include <time.h>

int main () {
    srand(time(NULL));
    tile* board = solved_board();
    int* mcount = calloc(40, sizeof(int));
    double* total_time = calloc(40,sizeof(double));

    Step solution[MAX_DEPTH];
    for (int i=0; i<100; i++) {
      printf("%d ", i);

          fflush(stdout);
      scram(board);
      clock_t start = clock();
      int len = ida_star(board, solution);
      clock_t end = clock();

      double elapsed =
          (double)(end - start)
          / CLOCKS_PER_SEC;

      mcount[len]++;
      total_time[len]+=elapsed;
    }


    free(board);

    for (int i=0; i<40; i++) {
      printf("%d scram in %d moves, in %f seconds\n", mcount[i], i, (float) total_time[i]/mcount[i]);
    }

    return 0;
}
