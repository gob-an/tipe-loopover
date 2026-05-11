#include "game.c"

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
