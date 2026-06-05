#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "struct.h"



void bfs_traversal_constrained(tile* initial_state, int p, int q) {

    HashNode* hash_table[HASH_SIZE] = { NULL };

    int* elements_per_depth = NULL;
    int max_depth_capacity = 0;
    int current_max_depth = -1;

    Queue* q_bfs = empty_q();

    push(q_bfs, initial_state, 0);
    hash_insert(hash_table, initial_state);

    // Tableaux de pointeurs de fonctions pour séparer les mouvements horizontaux et verticaux
    tile* (*horizontal_transitions[2])(tile*, int) = { Lcpy, Rcpy };
    tile* (*vertical_transitions[2])(tile*, int) = { Ucpy, Dcpy };

    while (!is_empty(q_bfs)) {
        tile* curr;
        int depth = pop(q_bfs, &curr);

        // Ajustement dynamique du tableau des profondeurs
        if (depth >= max_depth_capacity) {
            int old_capacity = max_depth_capacity;
            max_depth_capacity = (depth + 1) * 2;
            elements_per_depth = (int*)realloc(elements_per_depth, max_depth_capacity * sizeof(int));
            for (int i = old_capacity; i < max_depth_capacity; i++) {
                elements_per_depth[i] = 0;
            }
        }

        if (depth > current_max_depth) {
            current_max_depth = depth;
            printf("%d états explorés \n", elements_per_depth[depth-1]);
            printf("profondeur %d\n", current_max_depth);
        }

        elements_per_depth[depth]++;

        // 1. Mouvements Horizontaux (L, R) liés à l'indice de ligne i > q
        for (int i = 0; i < 2; i++) {
            for (int idx = q; idx < N; idx++) {
              tile* next_state = horizontal_transitions[i](curr, idx);
              if (hash_insert(hash_table, next_state)) {
                  push(q_bfs, next_state, depth + 1);

              } else {free(next_state);}
          }
      }

        // 2. Mouvements Verticaux (U, D) liés à l'indice de colonne j > p
        for (int i = 0; i < 2; i++) {
            for (int idx = p; idx < N; idx++) {

                tile* next_state = vertical_transitions[i](curr, idx);
                if (hash_insert(hash_table, next_state)) {
                    push(q_bfs, next_state, depth + 1);
                  } else {free(next_state);}
                }
      }

        free(curr);
    }

    // --- AFFICHAGE DES RÉSULTATS ---
    printf("\nNombre d'elements par profondeur (avec contraintes) :\n");
    for (int d = 0; d <= current_max_depth; d++) {
        printf("Profondeur %d : %d noeuds\n", d, elements_per_depth[d]);
    }

    // --- NETTOYAGE ---
    free(elements_per_depth);
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* curr = hash_table[i];
        while (curr != NULL) {
            HashNode* temp = curr;
            curr = curr->next;
            free(temp->state);
            free(temp);
        }
    }
}


int main () {
  int p1=1; int q1=4;
  int p2=2; int q2=4;
  printf("%d*%d->%d*%d\n", p1,q1,p2,q2);
  tile* init = calloc(SIZE, sizeof(tile));
  for (int i=0; i<p2; i++) {
    for (int j=0; j<q2;j++) {
      init[IDX(i, j)] = IDX(i, j)+1;
    }
  }
  bfs_traversal_constrained(init, p1, q1);
}
