#include "game.h"
#include "ida_star.h"

#include <time.h>

void stat (tile* board, Step* solution, int* mcount, double* total_time) {
  clock_t start = clock();
  int len = ida_star(board, solution);
  clock_t end = clock();
  double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
  mcount[len]++;
  total_time[len]+=elapsed;
}


void swap(tile *x, tile *y) {
    tile tmp = *x;
    *x = *y;
    *y = tmp;
}



void heap_alt (int n, tile *a, int len, bool parity, int* movecount, double* total_time, Step* solution, int* c)
{
    if (n == 1) {
        if (parity)
            {(*c)++; //if(c%100==0)
              printf("%d", *c); print(a); printf("%d", parity);
            stat(a, solution, movecount, total_time);}
        return;
    }

    for (int i = 0; i < n; i++) {
        heap_alt(n - 1, a, len, parity, movecount, total_time, solution, c);

        if (i == n - 1)
            break;

        if (n & 1)
            swap(&a[0], &a[n - 1]);
        else
            swap(&a[i], &a[n - 1]);

        parity = !parity;
    }
}




int main () {
  tile* board = solved_board();
  Step solution[MAX_DEPTH];
  int* mcount = calloc(MAX_DEPTH, sizeof(int));
  double* total_time = calloc(MAX_DEPTH, sizeof(double));
  int c=0;

  heap_alt(SIZE, board, SIZE, true, mcount, total_time, solution, &c);

  for (int i=0; i<MAX_DEPTH; i++) {
    printf("%d scram in %d moves, in %f seconds\n", mcount[i], i, (float) total_time[i]/mcount[i]);
  }

  free(board); free(mcount); free(total_time);

  return 0;
}
