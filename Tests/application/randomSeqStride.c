#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int getMin(int start, int end, int * a){
  int min = a[start];
  int index = start;
  for(int i = start+1; i < end; i++){
    if(min > a[i]){
      min = a[i];
      index = i;
    }
  }
  return index;
}

void sort(int * a, int size){
  for(int i = 0; i < size; i++){
    int min = getMin(i, size, a);
    if(min != i){
      int tmp = a[i];
      a[i] = a[min];
      a[min] = tmp;
    }
  }
}

int main(){
  int length = 100;
  int a[length];
  int b[length];
  srand(time(NULL));
  for(int i = 0; i < length; i++){
    a[i] = rand() % 100;
    printf("%d, ", a[i]);
  }
  printf("\n");
  sort(a, length);
  for(int i = 0; i < length; i++){
    printf("%d, ", a[i]);
  }
  printf("\n");
}
