#include <iostream>
#include <cstdlib>
#include <iostream>

class Counter{
  int counter;
  int * array;
public:
  Counter(int * array): counter(0), array(array){ };
  int getValue(){
    return array[counter];
  }
  void changeCounter(int i){
    this->counter = i;
  }
};


int main(){
  return 0;
}
