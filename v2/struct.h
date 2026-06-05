#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "game.h"
#include "struct.h"




//////////// QUEUE ////////////
struct Nd {
  tile* val;
  struct Nd* prev;
  struct Nd* next;
  int depth;
}; typedef struct Nd* Node;

typedef struct Q {
  Node front;
  Node rear;
} Queue;

Queue* empty_q(void);
bool is_empty(Queue*);
void push(Queue*, tile*, int);
int pop(Queue*, tile**);




////////// HASHMAP //////////
#define HASH_SIZE 65521

typedef struct HashNode {
    tile* state;
    struct HashNode* next;
} HashNode;

typedef HashNode* HashMap[HASH_SIZE];

void hash_init(HashMap map);
bool hash_insert(HashMap map, tile* state);
void hash_free(HashMap map);

#endif
