#include <stdio.h>
#include <stdlib.h>

int main () {
  char* str = malloc(8*sizeof(char));
  for (int i=0; i<8; i++) {str[i] = (char) (65+i);}
  int* casted = (int*) str;
  for (int i=0; i<8; i++) {
    printf("%c%d", str[i], casted[i]);
  } free(str);
}
// DO NOT CAST ARRAYS INTO OTHER TYPE ARRAY!!!!!!
