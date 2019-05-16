#include "Profile.h"

void iteration(){
  iterations[depth]--;
}

void load(int * address, int size, int loadCallID){
  if(indexes.find(loadCallID) == indexes.end()){
    string name = "/tmp/profile/";
    stringstream id;
    id << loadCallID;
    name = name.append(id.str()).append(".profile");
    std::ofstream initFile(name);
    ofstream * idFile;
    indexes[loadCallID].open(name);
  }
  indexes[loadCallID] << (intptr_t)address << " " << "R" << size / 8 <<"\n";

}


void store(int * address, int size, int loadCallID){
  if(indexes.find(loadCallID) == indexes.end()){
    string name = "/tmp/profile/";
    stringstream id;
    id << loadCallID;
    name = name.append(id.str()).append(".profile");
    std::ofstream initFile(name);
    ofstream * idFile;
    indexes[loadCallID].open(name);
  }
  indexes[loadCallID] << (intptr_t)address << " " << "W" << size / 8 <<"\n";
}


// before every loop begins this function is called
void loopStart(int gran, int counter, int iteration){
  // TODO move this to the beginning of function calls
  if(!init){
    init = true;
    std::ofstream initFile("/tmp/profile/0.profile");
    profileFile.open("/tmp/profile/0.profile");
    counterMax = counter;
  }
  granularity = gran;
  depth++;
  iterations[depth] = iteration ;
  if(depth > 1 && iterations[depth-1] < 0){
    iterations[depth] = -1;
  }
}

// every time a loop is ending this function is called
void loopExit(){
  depth--;
}
