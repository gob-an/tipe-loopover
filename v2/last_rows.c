#include "game.h"
#include "ida_star.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Macro de conversion (supposée, à ajuster selon votre game.h)
// Si IDX est défini différemment dans game.h, assurez-vous qu'il correspond.
#ifndef IDX
#define IDX(i, j) ((i) * N + (j))
#endif

void stat(tile* board, Step* solution, int* mcount, double* total_time) {
    clock_t start = clock();
    int len = ida_star(board, solution);
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    mcount[len]++;
    total_time[len] += elapsed;
}

// Permutation de deux indices dans le tableau de configuration
void swap_indices(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

// Permute les tuiles du board selon l'ordre actuel des indices cibles
void apply_permutation_and_run(tile* board, int* target_indices, int num_targets, bool parity, Step* solution, int* mcount, double* total_time, int* c) {
    if () {if (*c % 100 == 0)
      printf("Processing border permutation #%d\n", *c);
    stat(board, solution, mcount, total_time);
    (*c)++;}
}

// Algorithme de Heap modifié pour permuter les valeurs aux indices spécifiés
void heap_borders(int n, tile* board, int* target_indices, int num_targets, bool *parity, int* movecount, double* total_time, Step* solution, int* c)
{
    if (n == 1) {
        apply_permutation_and_run(board, target_indices, num_targets, *parity, solution, movecount, total_time, c);
        return;
    }

    for (int i = 0; i < n; i++) {
        heap_borders(n - 1, board, target_indices, num_targets, parity, movecount, total_time, solution, c);

        if (i == n - 1)
            break;

        // On permute physiquement les tuiles sur le board aux indices cibles
        if (n & 1) {
            tile tmp = board[target_indices[0]];
            board[target_indices[0]] = board[target_indices[n - 1]];
            board[target_indices[n - 1]] = tmp;
            swap_indices(&target_indices[0], &target_indices[n - 1]);
        } else {
            tile tmp = board[target_indices[i]];
            board[target_indices[i]] = board[target_indices[n - 1]];
            board[target_indices[n - 1]] = tmp;
            swap_indices(&target_indices[i], &target_indices[n - 1]);
        }

        *parity = !(*parity);
    }
}

int main () {
    tile* board = solved_board();
    Step solution[MAX_DEPTH];
    int* mcount = calloc(MAX_DEPTH, sizeof(int));
    double* total_time = calloc(MAX_DEPTH, sizeof(double));
    int c = 0;
    bool par = true;

    // 1. Collecter les indices uniques de la dernière ligne et de la dernière colonne
    // Taille max théorique : N (ligne) + N-1 (colonne sans le doublon du coin)
    int max_indices = 2 * N - 1;
    int* target_indices = malloc(max_indices * sizeof(int));
    int count_indices = 0;

    // Dernière ligne : IDX(N-1, j) pour j de 0 à N-1
    for (int j = 0; j < N; j++) {
        target_indices[count_indices++] = IDX(N - 1, j);
    }
    // Dernière colonne : IDX(i, N-1) pour i de 0 à N-2 (pour éviter de reprendre le coin)
    for (int i = 0; i < N - 1; i++) {
        target_indices[count_indices++] = IDX(i, N - 1);
    }

    // 2. Lancer le Heap modifié sur ce sous-ensemble d'indices
    heap_borders(count_indices, board, target_indices, count_indices, &par, mcount, total_time, solution, &c);

    // 3. Affichage des statistiques
    printf("\n--- Results (Border Permutations Only) ---\n");
    for (int i = 0; i < MAX_DEPTH; i++) {
        if (mcount[i] > 0) {
            printf("%d scrambles solved in %d moves, average time: %f seconds\n",
                   mcount[i], i, (float)total_time[i] / mcount[i]);
        }
    }

    free(target_indices);
    free(board);
    free(mcount);
    free(total_time);

    return 0;
}
