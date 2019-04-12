#include <stdio.h>
#include <stdlib.h>


int loop2(){
  char a[] = "abcdefg";
  int i = 0;
  for(; i < 500; i++ ){
    if((i % 200) == 3){
      printf("loop3 %c\n", a[i]);
    }
  }
  return 0;
}
int loop(){
  int loopA = 500;
  int a[loopA];
  //int j = 0;
  char b[] = "abcd";
  int counter = 0;
  for(int i = 0; i < 4; i++){
    b[i] = b[i] + 1;
    printf("loop1 %c\n", b[i]);
      for(int j = 0; j < 500; j = j + 2){
	counter = a[j];
        counter = a[j+1];
      }
      for(int j = 0; j < 50; j += 10){
        counter = a[j];
        }
  }
  return 0;
}


int main(){
 loop();
return 0;
}
