jvoid free_path (path* p) {
  if (p==NULL) {return;}
  free_path(p->next);
  free(p);
}

path* add (char hd_m, int hd_r, path* tl) {
  path* r = malloc(sizeof(path));
  r->move=hd_m; r->row=hd_r; r->next=tl;
  return r;
}

path* reverse (path* l, path* acc) { // appeler avec acc=NULL
  if (l==NULL) {return NULL;}
  return reverse (l->next, add(l->move, l->row, acc));
}







void swap (int* t, int i, int j) {
    int tmp = t[i];
    t[i] = t[j];
    t[j] = tmp;
}

void enlarge (prio* hp, int capa) {
    // allocation des nouvelles zones
    hp->capa = capa;
    int* prio = malloc(hp->capa*sizeof(int));
    int** elts = malloc(hp->capa * sizeof(int*));
    for (int i = 0; i < hp->size; i++) {
        prio[i] = hp->prio[i];
        elts[i] = hp->prio[i];
    }
    free(hp->elts);
    free(hp->prio);
    hp->elts = elts;
    hp->prio = prio;
}

void sift_up (prio* hp, int i) {
    int parent = (i - 1) / 2;
    if (i != 0 && hp->prio[i] < hp->prio[parent]) {
        swap(hp->prio, i, parent);
        swap(hp->elts, i, parent);
        sift_up(hp, parent);
    }
}

void sift_down (prio* hp, int i) {
    int lchild = 2*i + 1;
    int rchild = 2*i + 2;
    int j = i;
    if (lchild < hp->size && hp->prio[i] > hp->prio[lchild]) {
        j = lchild;
    }
    if (rchild < hp->size && hp->prio[j] > hp->prio[rchild]) {
        j = rchild;
    }
    if (j != indice_mal_place) {
        echange(hp->prio, j, indice_mal_place);
        echange(hp->elts, j, indice_mal_place);
        sift_down(hp, j);
    }

prio* hp_create (void) {
    prio* hp = malloc(sizeof(prio));
    hp->capa = CAPACITE_MAX_INITIALE;
    hp->size = 0;
    hp->prio = malloc(hp->capa * sizeof(int));
    hp->elts = malloc(hp->capa * sizeof(int*));
    return hp;
}

void push (prio* hp, int* elt, int prio) {
    // si on doit enfiler un élément et que la zone allouée est pleine,
    // on réalloue une zone deux fois plus grande
    if (hp->size == hp->capa) {
        enlarge(hp, 2 * hp->capa);
    }
    // on insère tout en bas à droite puis percolation vers le haut
    hp->elts[hp->size] = elt;
    hp->prio[hp->size] = prio;
    hp->size += 1;
    sift_up(hp, hp->size-1);
}

int* hp_pop (prio* hp) {
    assert(hp->size != 0);
    int* r = hp->elts[0];
    hp->size--;
    if (hp->size != 0) {
        swap(hp->elts, 0, hp->size);
        swap(hp->prio, 0, hp->size);
        sift_down(hp, 0);
    }
    if (hp->size < hp->capa / 4) {
        enlarge(hp, hp->capa / 2);
    }
    return r;
}

void hp_free (prio* hp) {
    free(hp->elts);
    free(hp->prio);
    free(hp);
}
