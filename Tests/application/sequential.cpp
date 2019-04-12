#include <iostream>
#include <cstdlib>
#include <iostream>
#include <list>
#include <ctime>

class Tree{
public:
  int apple;
  int leafs;
  Tree(int apple, int leafs): apple(apple), leafs(leafs){};
  void setApple(int a){
    this->apple = a;
  }

  void setLeafs(int l){
    this->leafs = l;
  }
  Tree(){};
  int getApple(){
    return apple;
  }

  int getLeafs(){
    return leafs;
  }
};

int main(int argc, char **argv){
  int counter = 1000;
  std::list<Tree*> forest;
  for(int i = 0; i < counter; i++){
    Tree * a = new Tree(std::rand() % 5, std::rand() % 3 * i);
    forest.push_back(a);
  }
  int allLeafs = 0;
  int allApple = 0;
  for(auto it = forest.begin(), end = forest.end(); it != end; ++it){
    allLeafs += (*it)->leafs;
    allApple += (*it)->apple;
  }
  std::cout << "the forest hast " << allLeafs << "treas and " << allApple << " apple\n";
  return 0;
}
