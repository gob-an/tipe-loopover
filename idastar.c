#include "idastar.h"

char m = {U, D, L, R};

int norm (int n, int v) { // <=n/2
  v = v>0?v:-v;
  v = v>n/2?n-v:v;
  return v;
}

int distance (int n, int pos, int val) {
  int dx = pos/n - val/n
  int dy = (pos%n) - (val%n);
  return norm(dx) + norm(dy);
}

void h (State st) {
  st.h = 0;
  int n = st.b.n;
  for (int i=0; i<n*n; i++) {
    st.h+=distance(i, st.b.b[i]);
  }
}

int newpos (int n, int pos, int i) {
  if (m.move == U) {
    return (i+n*n-n)%(n*n);
  } else if (m.move == D) {
    return (i+n)%(n*n);
  } else if (m.move == L) {
    return (i+n*n-1)%(n*n);
  } else if (m.move == R) {
    return (i+1)%(n*n);
  } return -1;
}

int dh (State st, Move m) {
  int r = 0;
  for (int i=0; i<st.b.n; i++) {
    r+=distance(newpos(i, m), st.b.b[i])-distance(i, st.b.b[i]);
  } return r;
}

/* create a copy + move */
Board apply (Board b; Move m) {

}

Board
