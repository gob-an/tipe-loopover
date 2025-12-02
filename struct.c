#include "struct.h"



/////////////// LIST ///////////////
typedef struct node_l {
  elem_l val;
  struct node_l* next;
} node_l;


List empty_list () {
  return NULL;
}

bool is_empty_list (List l) {
  return l==NULL;
}

elem_l head (List l) {
  assert(l!=NULL);
  return l->val;
}

List tail (List l) {
  assert(l!=NULL);
  return l->next;
}

List add_hd (elem_l hd, List tl) {
  List r = malloc(sizeof(node_l));
  assert(r!=NULL);
  r->val = hd; r->next=tl;
  return r;
}

void destr_l (List l) {
  if (l==NULL) {return;}
  destr_l(l->next);
  free(l);
}


List copy_l (List l) {
  if (l==NULL) {return l;}
  return add_hd(head(l), copy_l(tail(l)));
}
List aux (List l, List acc) {
  if (l==NULL) {return acc;}
  return aux(tail(l), add_hd(head(l), acc));
}
List reverse (List l) {
  return aux(l, empty_list());
}
int taille (List l) {
  return l==NULL? 0: 1+taille(tail(l));
}



/////////////// QUEUE ///////////////
typedef struct node_q {
  elem_q val;
  struct node_q* next;
} node_q;

struct queue {
  node_q* first;
  node_q* last;
};

Queue empty_queue () {
  Queue q = malloc(sizeof(struct queue));
  assert(q!=NULL);
  q->first = NULL; q->last = NULL;
  return q;
}

bool is_empty_queue (Queue q) {
  assert(q!=NULL);
  return q->first==NULL && q->last==NULL;
}

void push (Queue q, elem_q x) {
  assert(q!=NULL);
  node_q* n = malloc(sizeof(node_q));
  assert(n!=NULL);
  n->val=x; n->next=NULL;
  if (is_empty_queue(q)) {q->first=n; q->last=n;}
  else {q->last->next=n; q->last=n;}
}

elem_q pop (Queue q) {
  assert(!is_empty_queue(q));
  node_q* hd = q->first;
  elem_q r = hd->val;
  q->first = hd->next;
  if (q->first==NULL) {q->last=NULL;}
  free(hd); return r;
}

void destr_q (Queue q) {
  while (!is_empty_queue(q)) {elem_q e = pop(q); free(e);}
  free(q);
}
