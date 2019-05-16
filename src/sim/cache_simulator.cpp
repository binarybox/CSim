/**
  * --------------------------------------------------------------------------
  * Cache Simulator
  * Levindo Neto (https://github.com/levindoneto/Cache-Simulator)
  * --------------------------------------------------------------------------
  */
#include <unistd.h>
// Libraries
#include "cache_simulator.h"           // Header file with the structs and functions prototypes
#include <stdlib.h>
#include <string>
#define CACHEVIEW 0                        // 1: print the currently cache (data and upper), 0: do not print
#define BYTES_PER_WORD 1               // All words with 1 Byte (bytes_per_word)
#define DATA true


/**************************** Functions ***************************************/
CacheClass::CacheClass(){
  this->acess_count = 0;
  this->read_hits = 0;
  this->read_misses = 0;
  this->write_hits = 0;
  this->write_misses = 0;
  this->write_conflicts = 0;
};
CacheClass::CacheClass(int line_size, int number_of_lines, int associativity, std::string replacement_policy, bool debug) : debug(debug), replacement_policy(replacement_policy), currently_clk(0), number_of_reads(0), number_of_writes(0), n_of_lines(number_of_lines), l_size(line_size) {
  // Init of results
  this->number_of_lines = 1 << number_of_lines;
  this->associativity = 1 << associativity;
  this->line_size = 1 << line_size;
  // if(this->debug){
  //   std::cout << this->number_of_lines <<" " << this->associativity << " " << this->line_size << "\n";
  // }
  this->acess_count = 0;
  this->read_hits = 0;
  this->read_misses = 0;
  this->write_hits = 0;
  this->write_misses = 0;
  this->write_conflicts = 0;

  // prepare the memory for the cache
  this->words_per_line = this->line_size/BYTES_PER_WORD; // 1 byte is the size of a word in this simulator
  this->number_of_sets = this->number_of_lines / this->associativity;

  /**************** Alloc space for Cache Memory Data ***********************/
  long unsigned i;                         // index for the allocation with the loop for
  this->Cache_Data = (bool**) malloc( this->number_of_sets * sizeof(bool *));
  for (i=0; i<this->number_of_sets; i++) {
    this->Cache_Data[i] = (bool*) malloc (this->associativity * sizeof(bool));
  }
  /**************************************************************************/

  /******************* Alloc space for Cache Upper **************************/
  this->Cache_Upper = (long unsigned int**) malloc( this->number_of_sets * sizeof(long unsigned *));
  for (i=0; i<this->number_of_sets; i++) {
    this->Cache_Upper[i] = (long unsigned int*) malloc (this->associativity * sizeof(long unsigned));
  }
  /**************************************************************************/

  /**************** Alloc space for Access Time Stamp************************/
  this->T_Access = (long unsigned int**) malloc( this->number_of_sets * sizeof(long unsigned *));
  for (i=0; i<this->number_of_sets; i++) {
    this->T_Access[i] = (long unsigned int*) malloc (this->associativity * sizeof(long unsigned));
  }
  /**************************************************************************/

  /**************** Alloc space for load Time Stamp************************/
  this->T_Load = (long unsigned int**) malloc( this->number_of_sets * sizeof(long unsigned *));
  for (i=0; i<this->number_of_sets; i++) {
    this->T_Load[i] = (long unsigned int*) malloc (this->associativity * sizeof(long unsigned));
  }

  this->T_Counter = (unsigned int**) malloc( this->number_of_sets * sizeof(unsigned int *));
  for (i = 0; i < this->number_of_sets; i++){
    this->T_Counter[i] = (unsigned int *) malloc(this->associativity * sizeof (unsigned int));
  }

  this->write_bit = (bool **) malloc( this->number_of_sets * sizeof(bool *));
  for (i = 0; i < this->number_of_sets; i++){
    this->write_bit[i] = (bool *) malloc(this->associativity * sizeof (bool));
  }
  /**************************************************************************/

  startCache();
};
/**
* This function makes the start of all cache with zeros.
*/
int CacheClass::startCache() {
  for (long unsigned index_i=0; index_i<this->number_of_sets; index_i++) {
    for (long unsigned block_i=0; block_i< this->associativity; block_i++ ) {
      this->Cache_Data[index_i][block_i]  = false;
      this->Cache_Upper[index_i][block_i] = 0;
      this->T_Access[index_i][block_i]    = 0;
      this->T_Load[index_i][block_i]      = 0;
      this->write_bit[index_i][block_i]   = false;
    }
  }
  return 0;
}

/**
* This function get the line (upper) of a given address by the CPU
* This upper is equal to the Tag information
*/
long unsigned CacheClass::make_upper(long unsigned address, int bytes_per_word) {
  return (long unsigned)((address/bytes_per_word))/this->words_per_line;
}

/**
* This function generates the index for the set in the cache
*/
int CacheClass::make_index ( long unsigned upper) {
  int index;
  //printf("NUMBER OF SETS: %d\n", number_of_sets);
  //printf("UPPER: %lu\n", upper);
  index = upper % this->number_of_sets;

  return index;                               // index of the set in the cache
}

/**
* This function give the position in a set (by index) of a line with a determined upper
*/
int CacheClass::getPosUpper (int index, long unsigned line) {
  int block_i;
  //printf("INDEX POS UPPER %d, ASSOC: %d, LINE: %lu\n", index, associativity, line);
  for (block_i=0; block_i<this->associativity; block_i++) {
    if (this->Cache_Upper[index][block_i] == line){
      return block_i;               // Upper inside of the set found
    }
  }
  return -1;                           // Upper inside of the set not found
}

/** This function verifies if are or aren't free blocks (lines) available in
*     a set (by index1) in the cache memory.
* @return: 1 (set is full), -1 (There are free space in the set by index1)
*/
int CacheClass::there_Are_Space_Set(int index1) {
  int block_i;
  for (block_i=0; block_i< this->associativity; block_i++) {
    if (!this->Cache_Data[index1][block_i]) {
      return -1;
    }
  }
  return 1;
}

/** This function gives a block (line) free in a set (by index1) of the cache
*     memory with no particular order. It returns the first block (position of line)
*      free found, independent of its positon at the set.
*/
int CacheClass::random_free_space_set (int index1) {
  int block_i;
  for (block_i=0; block_i< this->associativity; block_i++) {
    if (!this->Cache_Data[index1][block_i]) {
      return block_i;
    }
  }
  return -1;                                   // None block free to be filled
}

int CacheClass::findLessAccessTSset (int index1) {
  int block_i;
  int p_lessAc=0; // Position when is the less T_Access in the set (by index)
  long unsigned lessAc = this->T_Access[index1][0];
  for (block_i=0; block_i< this->associativity; block_i++) {
    if (this->T_Access[index1][block_i] < lessAc) {
      lessAc = this->T_Access[index1][block_i];
      p_lessAc = block_i;
    }
  }
  return p_lessAc;
}

int CacheClass::findLessLoadTSset (int index1) {
  int block_i;
  int p_lessLd=0;  // Position when is the less T_Access in the set (by index)
  long unsigned lessLd = this->T_Load[index1][0];
  for (block_i=0; block_i< this->associativity; block_i++) {
    if (this->T_Load[index1][block_i] < lessLd) {              // b < b+1
      lessLd = this->T_Load[index1][block_i];
      p_lessLd = block_i;
    }
  }
  return p_lessLd;
}

int CacheClass::findLeastUsedSet(int index){
  int block_i;
  int p_lessUs=0;
  unsigned int lessUsed = this->T_Counter[index][0];
  for (block_i = 0; block_i < this->associativity; block_i++){
    if(this->T_Counter[index][block_i] < lessUsed){
      lessUsed = this->T_Counter[index][block_i];
      p_lessUs = block_i;
    }
  }
  return p_lessUs;
}

void CacheClass::write_cache (int index1, long unsigned line1, int size) {
  /** Writing the data in the set (by index) in the position that contains the
  *     upper (by line).
  */
  int is_full;
  int rpl, rpf;
  int free_block;
  int p_lAc, p_lLd;     // Position of the less counter for LRU(l) and FIFO(f)

  int position_that_has_this_upper = getPosUpper(index1, line1);

  /****************************** Write Miss ********************************/
  if (position_that_has_this_upper == -1) { // position with the right upper was not found
    /** None position contains a line with the solicited upper.
    * The cache can be full, so it have to use a replacement policy (FIFO or LRU).
    * If it still a place in the cache, it's enough to draft one of free
    *     lines to put the new upper and the "new" data.
    * To know if are free places in the set (by index1) of cache, it is
    *     used the function there_Are_Space_Set(...).
    */
    this->write_misses++; // (*result1).write_misses++  this points to the cache_mem in the main

    /** It is necessary to know if the set is full (it will use FIFO or LRU
    *     replacement policy) or not.
    */
    is_full = there_Are_Space_Set(index1);

    /*************************** Set is full *****************************/
    if (is_full == 1) {
      int newIndex = 0;
      // *** LRU ***
      if (this->replacement_policy == "LRU") { // It's LRU
        newIndex = findLessAccessTSset(index1);
      }
      // *** FIFO ***
      else if (this->replacement_policy == "FIFO") { // It's FIFO
        newIndex = findLessLoadTSset(index1);
      }
      else if(this->replacement_policy == "LFU"){
        newIndex = findLeastUsedSet(index1);
      }
      this->Cache_Upper[index1][newIndex] = line1; // line1 = upper
      if(this->write_bit[index1][free_block]){
        this->write_conflicts ++;
      }
      this->write_bit[index1][free_block] = true;
      this->Cache_Data[index1][newIndex] = DATA;
      this->T_Access[index1][newIndex] = this->currently_clk;  // Update T_Access
      this->T_Load[index1][newIndex]   = this->currently_clk ; // Update T_Load
      this->T_Counter[index1][newIndex] = 1; // update T_Counter

    }
    /**********************************************************************/

    /************************** Set isn't full ****************************/
    else {                  // There are a free block in the set (by index1)
      free_block = random_free_space_set (index1);

      this->Cache_Upper[index1][free_block] = line1;
      if(this->write_bit[index1][free_block]){
        this->write_conflicts ++;
      }
      this->write_bit[index1][free_block] = true;
      this->Cache_Data[index1][free_block] = DATA;
      this->T_Access[index1][free_block] = this->currently_clk; // Update the T_Access
      this->T_Load[index1][free_block] = this->currently_clk;   // Update the T_Load
      this->T_Counter[index1][free_block] = 1; // update T_Counter
    }
  }
  /**************************************************************************/

  /****************************** Write Hit ********************************/
  else {               // position with the right upper was found
    /** The position with the upper was found in the passed set (index1)
    *     The T_Access and T_Load is updated in this case, because a number_of_writes
    *     data is writed in a position at the set in cache that already have
    *     an another data.
    */
    this->write_hits++;
    this->Cache_Upper[index1][position_that_has_this_upper] = line1;
    //cache_mem->Cache_Data[index1][position_that_has_this_upper] = DATA; // Write the "new" data in the right position at the cache memory
    this->T_Access[index1][position_that_has_this_upper] = this->currently_clk; // Update the T_Access
    this->T_Counter[index1][position_that_has_this_upper] ++; // Increase the counter
    // for(int i = 0; i < this->number_of_sets; i++){
    //   for(int j = 0; j < this->associativity; j++){
    //     if(index1 != i && position_that_has_this_upper != j && this->T_Counter[i][j] > 1)
    //     this->T_Counter[i][j] --;
    //   }
    // }
  }
}

void CacheClass::read_cache (int index1, long unsigned line1, int sizeIndex) {
  /** Reading the data in the set (by index) in the position that contains the
  *     upper (by line).
  */
  int is_full;
  int rpl, rpf;
  int free_block;
  int p_lAc, p_lLd;     // Position of the less counter for LRU(l) and FIFO(f)
  int position_that_has_this_upper = getPosUpper(index1, line1);

  /****************************** Read Misses ********************************/
  if (position_that_has_this_upper == -1) { // position with the right upper was not found
    /** None position contains a line with the solicited upper.
    * The cache can be full, so it have to use a replacement policy (FIFO or LRU).
    * If it still a place in the cache, it's enough to draft one of free
    *     lines to put the new upper and the "new" data.
    * To know if are free places in the set (by index1) of cache, it is
    *     used the function there_Are_Space_Set(...).
    */
    this->read_misses++; // (*result1).write_misses++  this points to the cache_mem in the main

    /** It is necessary to know if the set is full (it will use FIFO or LRU
    *     replacement policy) or not.
    */
    is_full = there_Are_Space_Set(index1);

    /*************************** Set is full ******************************/
    if (is_full == 1) {
      int new_index = 0;
      //printf("SET FULL: %d\n", index1);
      // *** LRU ***
      if (this->replacement_policy == "LRU") { // It's LRU
      new_index = findLessAccessTSset(index1);
      }
      // *** FIFO ***
      else if (this->replacement_policy == "FIFO") { // It's FIFO
      new_index = findLessLoadTSset(index1);
      }
      else if(this->replacement_policy == "LFU"){
        new_index = findLeastUsedSet(index1);
      }
      this->Cache_Upper[index1][new_index] = line1;     // line1 = upper
      if(this->write_bit[index1][free_block]){
        this->write_conflicts ++;
        this->write_bit[index1][free_block] = false;
      }
      this->Cache_Data[index1][new_index] = DATA;
      this->T_Access[index1][new_index] = this->currently_clk;  // Update T_Access
      this->T_Load[index1][new_index]   = this->currently_clk ; // Update T_Load
      this->T_Counter[index1][new_index] = 1; // update T_Counter

    }
    /**********************************************************************/

    /************************** Set isn't full ****************************/
    else {                  // There are a free block in the set (by index1)
      free_block = random_free_space_set (index1);

      this->Cache_Upper[index1][free_block] = line1;
      if(this->write_bit[index1][free_block]){
        this->write_conflicts ++;
        this->write_bit[index1][free_block] = false;
      }
      this->Cache_Data[index1][free_block] = DATA;
      this->T_Access[index1][free_block] = this->currently_clk; // Update the T_Access
      this->T_Load[index1][free_block] = this->currently_clk;   // Update the T_Load
      this->T_Counter[index1][free_block] = 1; // update T_Counter
    }
  }
  /**************************************************************************/

  /****************************** Read Hit ********************************/
  else {               // position with the right upper was found
    /** The position with the upper was found in the passed set (index1)
    *     The T_Access and T_Load is updated in this case, because a number_of_writes
    *     data is writed in a position at the set in cache that already have
    *     an another data.
    */
    this->read_hits++;
    this->Cache_Upper[index1][position_that_has_this_upper] = line1;
    //this->cache_mem->Cache_Data[index1][position_that_has_this_upper] = DATA;  // Modified bit
    this->T_Access[index1][position_that_has_this_upper] = this->currently_clk; // Update the T_Access
    this->T_Counter[index1][position_that_has_this_upper] ++; // Increase the counter for LFU Algorithm
    // for(int i = 0; i < this->number_of_sets; i++){
    //   for(int j = 0; j < associativity; j++){
    //     if(index1 != i && position_that_has_this_upper != j && this->T_Counter[i][j] > 1)
    //     this->T_Counter[i][j] --;
    //   }
    // }
  }
}

void CacheClass::read(long unsigned address, int size){
  this->currently_clk ++;
  this->acess_count ++;
  long unsigned line = make_upper(address, BYTES_PER_WORD);
  int index = make_index (line);
  // DEBUG prints
  if (this->debug){

    printf("read index: %d\n", index);
    printf("read the line: %lu\n", line);
  }

  this->number_of_reads++;
  read_cache(index, line, size);
  if (this->debug){
    printf("\nR:%d, W:%d\n", this->number_of_reads, this->number_of_writes);
  }
}

void CacheClass::write(long unsigned address, int size){
  this->currently_clk ++;
  this->acess_count ++;
  long unsigned line = make_upper(address, BYTES_PER_WORD);
  int index = make_index (line);
  // DEBUG prints
  if (this->debug){

    printf("write Index: %d\n", index);
    printf("write the line: %lu\n", line);
  }

  this->number_of_reads++;
  write_cache(index, line, size);
  if (this->debug){
    printf("\nR:%d, W:%d\n", this->number_of_reads, this->number_of_writes);
  }
}

void CacheClass::result(){
  printf("replacement policy %s\n", replacement_policy.c_str());
  //output
  float totalMisses = (float) this->read_hits +  (float) this->read_misses;
  float totalHits = this->write_hits + this->write_misses;
  printf("access counts = %d\n", this->acess_count);

  printf("read hits = %d \n", this->read_hits );

  printf("read misses = %d \n", this->read_misses);
  printf("write hits = %d\n", this->write_hits);
  printf("write misses = %d\n", this->write_misses);
  std::cout << "write conflicts = " << this->write_conflicts << "\n";

  // printf("read  hits %% = %f \n", (float)this->read_hits / (float)(this->read_hits + this->read_misses) );
  // printf("write hits %% = %f \n", (float)this->write_hits / (float)(this->write_hits + this->write_misses) );
}
float CacheClass::readHits(){
  if(this->read_hits != 0 && this->read_misses != 0){
    return (float)this->read_hits / (float)(this->read_hits + this->read_misses);
  }
  return 0.0;
}
float CacheClass::writeHits(){
  if(this->write_hits != 0 && this->read_misses != 0){
    return (float)this->write_hits / (float)(this->write_hits + this->write_misses);
  }
  return 0.0;
}
