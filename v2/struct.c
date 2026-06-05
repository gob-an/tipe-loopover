#include "struct.h"



Queue* empty_q() {
  Queue* r = malloc(sizeof(Queue));
  r->front=NULL; r->rear=NULL;
  return r;
}

bool is_empty(Queue* q) {
  return q->front==NULL && q->rear==NULL;
}
void push(Queue* q, tile* st, int d) {
  Node n = malloc(sizeof(struct Nd));
  n->val=st; n->next=q->front; n->prev=NULL; n->depth=d;
  if (is_empty(q))q->rear=n;
  else q->front->prev=n;
  q->front = n;
}
int pop(Queue* q, tile** r) {
  if (is_empty(q)) {*r=NULL; return -1;}
  Node popped=q->rear;
  *r = popped->val; int d=popped->depth;
  q->rear=popped->prev;
  if(q->rear==NULL) q->front=NULL;
  else q->rear->next=NULL;
  free(popped); return d;
}

void hash_init(HashMap map) {
    for (int i = 0; i < HASH_SIZE; i++) {
        map[i] = NULL;
    }
}

// 2. Fonction de hachage ultra simple (djb2)
unsigned int hash_function(tile* state) {
    unsigned int hash = 5381;
    for (int i = 0; i < SIZE; i++) {
        // On convertit la valeur de la tile en entier pour le calcul
        hash = ((hash << 5) + hash) + (unsigned int)state[i];
    }
    return hash % HASH_SIZE;
}

// 3. Insertion + Vérification de doublon
// Renvoie 'true' si l'élément a été inséré, 'false' s'il y était déjà
bool hash_insert(HashMap map, tile* state) {
    unsigned int slot = hash_function(state);
    HashNode* curr = map[slot];

    // On parcourt la liste de cette case pour voir si l'état existe déjà
    while (curr != NULL) {
        if (memcmp(curr->state, state, SIZE * sizeof(tile)) == 0) {
            return false; // Déjà présent !
        }
        curr = curr->next;
    }

    // Si on arrive ici, l'état n'existe pas : on l'ajoute en tête de liste
    HashNode* new_node = malloc(sizeof(HashNode));
    new_node->state = malloc(SIZE * sizeof(tile));
    memcpy(new_node->state, state, SIZE * sizeof(tile));

    new_node->next = map[slot];
    map[slot] = new_node;

    return true; // Bien inséré
}

// 4. Libération de la mémoire
void hash_free(HashMap map) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* curr = map[i];
        while (curr != NULL) {
            HashNode* temp = curr;
            curr = curr->next;
            free(temp->state);
            free(temp);
        }
        map[i] = NULL;
    }
}
