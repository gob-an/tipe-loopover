#include "game.h"
#include "ida_star.h"
#include <time.h>

path* solve (int* b) { //rajouter le type
  time_t t0=time(NULL);
  path* r = idastar(b) //pas oublier param?
  printf("exécution en %ds\n", (int) time(NULL)-t0);
}

int main () {
  int* b = solved();
  path* sol = solve(b);
  if (sol==NULL) {
    if (!solved(start)) {printf("Pas de solution :/\n");}
    printf("Déjà résolu :D\n")
  }
  print_list(sol);
  free(b); lst_free(sol);
}
