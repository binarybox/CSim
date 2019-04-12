#include <stdio.h>
#include <stdlib.h>

int simple(int * a, int size){
  int counter = 0;
  for(int i = 0; i < size; i+=1){
    counter += a[i];
  };
  return counter;
}

int doubl(int * c, int size){
  int counter = 0;
  for(int i = 0; i < size - 1; i += 2){
    counter += c[i];
    counter += c[i + 1];
  }
  return counter;
}

int foo(int * a, int * b, int size){
  int counter = 0;
  for(int i = 0; i < size; i += 2){
    counter += b[i];
    counter += b[i+1];
  }
  for(int i = 0; i < size; i += 2){
    counter += a[i];
    counter += a[i+1];
  }
  return counter;
}

int main(){
  int size = 512;
  int b[size];
  int a[size];
  int one = 0;
  int two = 0;

  for(int i = 0; i < size; i ++){
    b[i] += i;
  }
  for(int i = 0; i < size - 1; i ++){
    for(int j = -1; j < 2; j++){
      a[i] = b[i+j];
    }
  }

  return 0;
}
