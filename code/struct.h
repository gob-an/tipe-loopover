#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include "game.h"


typedef Move node_l;
typedef struct node_l* List;

List empty_list ();
bool is_empty_list (List);
elem_l head (List);
List tail (List);
List add_hd (elem_l, List);
void destr_l (List);

List copy_l (List);
List reverse (List);
int taille (List);


typedef struct {
  
} node_q;
typedef struct queue* Queue;

Queue empty_queue(void);
bool is_empty_queue(Queue);
void push(Queue, elem_q);
elem_q pop(Queue);
void destr_q(Queue);

#endif
