#ifndef STRUCT_H
#define STRUCT_H

#include "game.h"

enum {L='L', R='R', U='U', D='D'};


/******** LIST **********/
typedef struct list {
  char move;
  int row;
  struct list* next;
} path;

void free_path(path*);
path* add(char, int, path*);
path* reverse(path*, path*); //appeler avec NULL


/******** HEAP **********/
typedef struct heap {
    int* prio; // priorités
    int** elts;
    int size;
    int capa;
} prio;
prio* hp_create(void);
void hp_push(prio*, int*, int);
contenu hp_pop(prio*);
void hp_free(prio*);


#endif
