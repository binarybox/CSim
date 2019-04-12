#include <stdio.h>
#include <stdlib.h>

int loop(){
  int loopA = 500;
  int a[loopA];
  int c[loopA];
  int i = 0;
  //int j = 0;
  char b[] = "abcdasdfasdfasdfasdf asdfasdfafscfscd";
  for(; i < 20; i++){
    b[i] = b[i] + 1;
      for(int j = 0; j < 250; j++){
	       c[j] = a[j*2];
      }
  }
  return 0;
}

int overlapping(){
	int x[150];
	for (int i = 0; i < 150; i++){
		for(int j = 0; j < 3; j++){
			x[i+j] = x[i + j] + 1;
		}
	}
	return 0;
}


int main(){
 overlapping();
return 0;
printf("posrn");
}
