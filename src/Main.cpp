#include <cstdlib>
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <limits>
#include <regex>

#include "sim/cache_simulator.cpp"
#include "sim/Access.h"

int debug = 0;

using namespace std;

int compileProgramm(string fileName, string path, string flags, string linkFiles, int granularity){
  if(0 != system("mkdir -p /tmp/profile")){
    return 0;
  }

  string programmDir = (path.substr(0, path.find_last_of("\\/")));
  string cpp = "";
  if (std::regex_match (fileName, std::regex("(.*)(.cpp)") )){
    "++";
  }

  // compile to programm to llvm representation
  string compileLLVMCommand = "clang";
  compileLLVMCommand = compileLLVMCommand.append(cpp).append(" -S -emit-llvm -Xclang -disable-O0-optnone ");
  string llvmCommand = compileLLVMCommand.append(flags).append(" ").append(fileName).append(" -o /tmp/profile/output.ll");
  if(0 != system(llvmCommand.c_str())){
    cout << "basic compilation failed\n";
    return 0;
  }

  if(linkFiles.size() > 0){

    string linkCommand = "clang -S -emit-llvm ";
    linkCommand = linkCommand.append(" ").append(flags).append(" ");
    //TODO compile the linked code to the folder /tmp/profile/
    linkCommand = linkCommand.append(" ").append(linkFiles).append(" ");
    if(0 != system(linkCommand.c_str())){
      cout << "linking failed\n";
      return 0;
    }

    string link = "llvm-link -S ";
    link = link.append("*.ll /tmp/profile/output.ll -o /tmp/profile/output.ll");
    if(0 != system(link.c_str())){
      cout << "linking files together failed\n";
      return 0;
    }

    if(0 != system("rm -f *.ll")){
      cout << "removing linked llvm files failed\n";
      return 0;
    }
  }
  // use the custom path to profile the loads
  string opt = "opt --mem2reg -S -load ";
  opt = opt.append(programmDir).append("/src/llvmPass/InsertPass/ProfilePass.so -function");
  stringstream g;
  g << granularity;
  opt = opt.append(" -granularity ").append(g.str());
  opt = opt.append(" < ");
  opt = opt.append("/tmp/profile/output.ll").append(" -o ").append("/tmp/profile/output.ll").append(" > /dev/null");
  if(0 != system(opt.c_str())){
    return 0;
  }

  // link the class to call the functions which are included in the path
  string linkProgramm = "clang++ ";
  //linkProgramm = linkProgramm.append(linkFiles).append(" ");
  linkProgramm = linkProgramm.append(programmDir).append("/src/linkedFunctions/library/profile.o /tmp/profile/output.ll -o /tmp/profile/output");
  if(0 != system(linkProgramm.c_str())){
    return 0;
  }

  //remove useless files
  if(0 != system("rm /tmp/profile/output.ll")){
    exit(0);
  }
  return 1;
}
int runProgramm(string input){
  if(0 != system("rm -f /tmp/profile/01.profile /tmp/variables")){
    return 0;
  }
  string application = "/tmp/profile/output ";
  application = application.append(input).append(" > /tmp/profile/profilOutput");
  if(0 != system(application.c_str())){
    return 0;
  }

  if(0 != system("rm /tmp/profile/output")){
    return 0;
  }
  return 1;
}

int countLines(string file){
  std::string textLine;
  std::ifstream ptr_file_input(file);
  ptr_file_input.unsetf(std::ios_base::skipws);
  return std::count(std::istream_iterator<char>(ptr_file_input), std::istream_iterator<char>(), '\n');

}

void convert(string file, Access * access){

  int i = 0;
  int space = 0;
  int size = 0;
  long unsigned address;
  char RorW;
  std::string textLine;
  std::ifstream fill_access(file);

  if(fill_access.is_open()){
    while(getline(fill_access, textLine)){
      space = textLine.find(" ");
      address = stol(textLine.substr(0, space));                    // get address
      RorW = textLine[space+1];                                     // get load or store information
      size = stoi(textLine.substr(space+2));
      access[i] = Access(address, RorW, size);
      i++;
    }
  }
}

CacheClass simulate(Access * acc, int acc_size, int line_size, int number_of_lines, int associativity){
  int size = 3;
  CacheClass caches[size];
  caches[0] = CacheClass(line_size, number_of_lines, associativity, "LFU", false);
  caches[1] = CacheClass(line_size, number_of_lines, associativity, "FIFO", false);
  caches[2] = CacheClass(line_size, number_of_lines, associativity, "LRU", false);

  for(int i = 0; i < acc_size; i++){
    if(acc[i].type == 'R'){
      for(int j = 0; j < size; j++){
        //for(int k = 0; k < acc[i].size; k++){
          caches[j].read(acc[i].address , acc[i].size);
        //}
      }
    }
    else if(acc[i].type == 'W'){
      for(int j = 0; j < size; j++){
        //for(int k = 0; k < acc[i].size; k++){
          caches[j].write(acc[i].address , acc[i].size);
        //}
      }
    }
    else {
      std::cout << "undefined value in file\n";
      exit(1);
    }
  }

  CacheClass c = caches[0];
  for(int i = 1; i < size; i++){
    if(caches[i].readHits() < c.readHits()){
      c = caches[i];
    }
  }
  return c;
}

void validate(string inputFile, float expectations){

  int acc_size = countLines(inputFile);
  Access * acc = (Access*) malloc( acc_size * sizeof(Access));
  convert(inputFile, acc);
  if(debug){
    std::cout << acc_size << " memory accesses\n";
  }
  // description
  int size = 3;
  /****************************/
  // MAKE the search strategy //
  /****************************/

  float last_result = -1.0;

  float thresh = 0.1;

  // The search strategy is all about the line size and number of lines.
  int line_size = 0;
  int number_of_lines = 0;


  CacheClass best = simulate(acc, acc_size, line_size+1, number_of_lines+1, number_of_lines + 1);
  CacheClass last;
  int increase = 10;
  while(increase > 2){
    last = simulate(acc, acc_size, line_size + increase, number_of_lines + increase, number_of_lines + increase);
    if(debug){
      std::cout << line_size << " "  << best.readHits() << " | " << line_size + increase << " " << last.readHits() << " " << "\n";
    }
    if(last.readHits() > expectations || last.readHits() < best.readHits()){
      increase /= 2;
      last = CacheClass();
      continue;
    }
    number_of_lines += increase;
    line_size += increase;
    best = last;
  }

  while(best.readHits() < expectations){
    // first we simulate with the highest associativity.
    // when the expectations are lower then the best results the associativity will be reduced to find the minimum settings
        last = best;
    CacheClass tmp = simulate(acc, acc_size, line_size + 1, number_of_lines + 1, number_of_lines + 1);
    CacheClass incSize = simulate(acc, acc_size, line_size + 1, number_of_lines, number_of_lines);
    if(tmp.readHits() > incSize.readHits()){
      line_size ++;
      number_of_lines ++;
      best = tmp;
    }
    else{
      CacheClass incLines = simulate(acc, acc_size, line_size, number_of_lines + 1, number_of_lines + 1);

      if(incLines.readHits() > incSize.readHits()){
        number_of_lines ++;
        best = incLines;
      }
      else{
        line_size ++;
        best = incSize;
      }
    }
    if (debug){
      std::cout << "##############\n" << "finished run with: \n";
      std::cout << number_of_lines << " number of lines\n " ;
      std::cout << line_size << " line size\n";
      std::cout << best.readHits() << " hits\n";
    }
    if(last.readHits() > best.readHits()){
      best = last;
      break;
    }
  }

  // find mimimum associativity
  // associativity can only be equal or less then the number of lines.
  // It is also the variable we will search after we decided about the line size and number of lines.
  int associativity = 1;
  CacheClass findAssoc;
  while ( associativity < 16 ){

    findAssoc = simulate(acc, acc_size, best.l_size, best.n_of_lines, best.n_of_lines / associativity);

    if(findAssoc.readHits() < best.readHits() || findAssoc.readHits() > expectations){
      break;
    }
    associativity = associativity << 1;
    best = findAssoc;
  }


  std::cout << "read Hits " << (int) 100 * best.readHits() << "\n";
  std::cout << "write hits " << (int) 100* best.writeHits() << "\n";
  std::cout << "write conflicts "  << (int) 100 * best.getWriteConflicts() << "\n";
  std::cout << "lines " << best.getNumberOfLines() << "\n";
  std::cout << "line size " << best.getLineSize() << "\n";
  std::cout << "associativity " << associativity << "\n";
  std::cout << "replacement policy " << best.getPolicy() << "\n";
  //}
}


void help(){
  std::cout << "Profile a programm with llvm. To get this run llvm and clang must be installed\n";
  std::cout << "All variables must be typed in without = or space e.g. profile T200 G1 programm.c\n";
  std::cout << "variables: \n";
  std::cout << "-g= \t\t|| which granularity will be used to profile. DEFAULT 0\n";
  std::cout << "-e= \t\t|| the expectation of reads / read hits in percentent (float => 0.9 == 90%). DEFAULT 0.9\n";
  std::cout << "\t\t|| -g=0: the application will be profiled in a stream for the whole programm\n";
  std::cout << "\t\t|| -g=1: the application will be profiled in a stream for a variable\n";
  std::cout << "\t\t|| -g=2: the application will be profiled relative to the loads\n";
  std::cout << "-d= \t\t|| the debug level (currently only 1 supportet)\n";
  std::cout << "-flag= || the flags to compile the file. Must be wrapped with \" \n";
  std::cout << "-input=\t|| Input data to run the application\n";
  std::cout << "-link-files=\t|| Files to be linked with\n";
  exit(0);
}



int main(int argc, char **argv){
  if(argc < 2){
    help();

  }
  std::list<string> arguments (argv, (argv) + argc);

  int granularity = 0;
  string inputData = "";
  string linkFiles = "";
  float expectations = 0.9;

  int oldData = 0;

  std::stringstream applicationName;
  std::string compileFlags = "";
  // remove the name of the programm (standard name is profile)
  auto it = arguments.cbegin(), end = arguments.cend();
  ++it;
  for(; it != end; ++it){
    arguments.pop_front();

    if (std::regex_match ((*it), std::regex("(-flag=)(.*)") )){
      compileFlags = std::string((*it)).substr(6);
    }
    else if (std::regex_match ((*it), std::regex("(-e=)(.*)") )){
      expectations = atof(std::string((*it)).substr(3).c_str());
    }
    else if (std::regex_match ((*it), std::regex("(-link-files=)(.*)") )){
      linkFiles = std::string((*it)).substr(12);
    }
    else if (std::regex_match ((*it), std::regex("(-input=)(.*)") )){
      inputData = std::string((*it)).substr(7);
    }
    else if(std::regex_match ((*it), std::regex("(-g=)(.*)") )){
      granularity = atoi(std::string((*it)).substr(3).c_str());
    }
    else if(std::regex_match ((*it), std::regex("(-d=)(.*)") )){
      debug = atoi(std::string((*it)).substr(3).c_str());
    }
    else if(std::regex_match ((*it), std::regex("(-oD=)(.*)") )){
      oldData = atoi(std::string((*it)).substr(4).c_str());
    }
    else if(std::regex_match ((*it), std::regex("help"))){
      help();
    }
    else {
      applicationName << std::string(*it);
    }
  }
  if (oldData){
    if(granularity == '1'){
      int id;
      string line;
      ifstream variables_file ("/tmp/profile/profile.variables");
      if(variables_file.is_open()){
          while(getline(variables_file, line)){
            int space = line.find(" ");
            string id = line.substr(0, space);
            string name = line.substr(space+1);
            std::cout << "variable: " << name << "\n";
            string inFile = "/tmp/profile/";
            inFile = inFile.append(id).append(".profile");
            validate(inFile, expectations);
          }
          variables_file.close();
      }
    }
    else{
      validate("/tmp/profile/0.profile", expectations);
    }
  }
  // compile the programm from the current path in the os.
  // the profiled data file will be put in the /tmp/ directory
  else if(compileProgramm(applicationName.str(), realpath(argv[0], NULL), compileFlags, linkFiles, granularity)){

    if(runProgramm(inputData)){

      if(granularity == '1'){
        int id;
        string line;
        ifstream variables_file ("/tmp/profile/profile.variables");
        if(variables_file.is_open()){
            while(getline(variables_file, line)){
              int space = line.find(" ");
              string id = line.substr(0, space);
              string name = line.substr(space+1);
              std::cout << "variable: " << name << "\n";
              string inFile = "/tmp/profile/";
              inFile = inFile.append(id).append(".profile");
              validate(inFile, expectations);
            }
            variables_file.close();
        }
      }
      else{
        validate("/tmp/profile/0.profile", expectations);
      }
    };
  };
  exit(0);
};
