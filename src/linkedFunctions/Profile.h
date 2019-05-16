#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

#include <stdint.h>

using namespace std;

static map<int, ofstream> indexes = {};
static int granularity = 0;
static int depth = 0;
static int counter = 0;
static int counterMax = -1;
static intptr_t baseAddress;
static bool init = false;
static ofstream profileFile;
static int iterations[1000];
