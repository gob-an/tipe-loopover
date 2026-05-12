int heur (int* state) {
  switch{
    return '\0';
}

char opp (char move) {
  switch (move) {
    case L: return R;
    case R: return L;
    case U: return D;
    case D: return U;
    default: return '\0';
  }
}



path* idastar (int* root) {
  int* bound = heur(root);
  return search(root, 0, bound,'\0');
}

path* search (int** current_state, int depth, int* bound, char last_move, int last_row) {
  int h = heur(*current_state);
  if (depth+h > bound) return NULL; else bound=depth+h
  if (h == 0) return add(last_move, last_row, NULL);

  path* r = NULL;
  for (int i=0; i<N; i++) {
    // L R U D
    char move = L;
    L(current_state, i);
    path* new = search(current_state, depth+1, bound, L, i);
  }
}

typedef struct {
    int tiles[SIZE];
    int zero; /* index of blank tile */
} State;

const char moves[] = {'U', 'D', 'L', 'R'};

/* movement deltas */
const int dr[] = {-1, 1, 0, 0};
const int dc[] = {0, 0, -1, 1};

/* opposite moves */
const int opposite[] = {1, 0, 3, 2};

int solution[MAX_DEPTH];

/* goal positions */
int goal_row[SIZE];
int goal_col[SIZE];

/* Manhattan distance heuristic */
// int heuristic(State *s) {
//     int h = 0;
//
//     for (int i = 0; i < SIZE; i++) {
//         int val = s->tiles[i];
//
//         if (val == 0)
//             continue;
//
//         int r = i / N;
//         int c = i % N;
//
//         h += abs(r - goal_row[val]) + abs(c - goal_col[val]);
//     }
//
//     return h;
// }
//
// int is_goal(State *s) {
//     for (int i = 0; i < SIZE - 1; i++) {
//         if (s->tiles[i] != i + 1)
//             return 0;
//     }
//
//     return s->tiles[15] == 0;
// }

/*
 * IDA* depth-first search
 *
 * g = current depth
 * bound = current f-cost limit
 * prev_move = previous move index
 */
path* search(State *s, int depth, int bound, char move, int row) {
    int h = heur(s);
    int f = depth + h;

    if (f > bound) return NULL;

    if (h == 0) return move='\0'? NULL: add(move, row, NULL);

    int min = 1 << 30;

    for (int m = 0; m < N; m++) {
        int nr = zr + dr[m];
        int nc = zc + dc[m];

        if (nr < 0 || nr >= N || nc < 0 || nc >= N)
            continue;

        int new_zero = nr * N + nc;

        /* make move */
        int moved_tile = s->tiles[new_zero];

        s->tiles[s->zero] = moved_tile;
        s->tiles[new_zero] = 0;

        int old_zero = s->zero;
        s->zero = new_zero;

        solution[g] = m;

        int t = search(s, g + 1, bound, m);

        if (t == FOUND)
            return FOUND;

        if (t < min)
            min = t;

        /* undo move */
        s->tiles[new_zero] = moved_tile;
        s->tiles[old_zero] = 0;
        s->zero = old_zero;
    }

    return min;
}

path* ida_star(int* root) {
    int bound = heur(root);

    while (true) {
        path* sol = search(root, 0, bound, '\0', 0);
    }
}
