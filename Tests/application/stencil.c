#include <stdio.h>
#include <stdlib.h>

int stencilA(int * a, int length){
  int count = 0;
  for(int i = 0; i < length / 10; i++){
    for(int j = 1; j <= length / 50; j++){
      for(int k = 2; k >= 0; k--){
        count = a[i + j + k];
      }
    }
  }
  return 0;
}

int stencilB(int * a, int length){
  int count = 0;
  for(int i = 0; i < length / 10; i++){
    for(int j = 1; j <= length / 50; j++){
        for(int k = 0; k <= 2; k++){
          count = a[i + j + k];
        }
    }
  }
  return 0;
}


int main(){
  int length = 500;
  int a[length];
  stencilB(a, length);
  stencilA(a, length);
  return 0;
}
