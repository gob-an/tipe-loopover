#include "game.h"

#include <string.h>

#define MAX_STATES 161440  // 9! max pour 3x3

typedef struct {
    int state[SIZE];
    int parent;
    char move[10];
} Node;

Node nodes[MAX_STATES];
int front = 0, back = 0;

// comparaison d'états
int equal(int *a, int *b) {
    return memcmp(a, b, SIZE * sizeof(int)) == 0;
}

// copie
void copy(int *dst, int *src) {
    memcpy(dst, src, SIZE * sizeof(int));
}

// hash simple (très basique)
int visited[MAX_STATES];
int visited_count = 0;

int is_visited(int *s) {
    for (int i = 0; i < visited_count; i++) {
        if (equal(nodes[visited[i]].state, s))
            return 1;
    }
    return 0;
}

void add_visited(int idx) {
    visited[visited_count++] = idx;
}

// BFS
int bfs(int *start, int *goal) {
    copy(nodes[0].state, start);
    nodes[0].parent = -1;
    front = 0; back = 1;
    add_visited(0);

    while (front < back) {
        int cur = front++;

        if (equal(nodes[cur].state, goal)) {
            return cur;
        }

        int tmp[SIZE];

        for (int i = 0; i < N; i++) {
            // 4 types de moves
            copy(tmp, nodes[cur].state);
            L(tmp, i);
            if (!is_visited(tmp)) {
                copy(nodes[back].state, tmp);
                nodes[back].parent = cur;
                sprintf(nodes[back].move, "L%d", i);
                add_visited(back++);
            }

            copy(tmp, nodes[cur].state);
            R(tmp, i);
            if (!is_visited(tmp)) {
                copy(nodes[back].state, tmp);
                nodes[back].parent = cur;
                sprintf(nodes[back].move, "R%d", i);
                add_visited(back++);
            }

            copy(tmp, nodes[cur].state);
            U(tmp, i);
            if (!is_visited(tmp)) {
                copy(nodes[back].state, tmp);
                nodes[back].parent = cur;
                sprintf(nodes[back].move, "U%d", i);
                add_visited(back++);
            }

            copy(tmp, nodes[cur].state);
            D(tmp, i);
            if (!is_visited(tmp)) {
                copy(nodes[back].state, tmp);
                nodes[back].parent = cur;
                sprintf(nodes[back].move, "D%d", i);
                add_visited(back++);
            }
        }
    }

    return -1;
}

// reconstruction du chemin
void print_solution(int idx) {
    if (idx == -1) return;
    print_solution(nodes[idx].parent);
    if (nodes[idx].parent != -1)
        printf("%s ", nodes[idx].move);
}

int main() {
    int start[SIZE] = {
        3, 2, 7,
        1, 6, 4,
        6, 5, 9
    };

    int goal[SIZE] = {
        1,2,3,
        4,5,6,
        7,8,9
    };

    int res = bfs(start, goal);

    if (res != -1) {
        printf("Solution trouvée:\n");
        print_solution(res);
        printf("\n");
    } else {
        printf("Pas de solution\n");
    }

    return 0;
}
