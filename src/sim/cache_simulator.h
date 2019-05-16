/**************************** Libraries ***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <fstream>

/*******************DEBUG***********************************************************/

/************************ Prototypes of used functions ************************/
class CacheClass{
  bool debug;
  unsigned currently_clk;  // To simulate a clock with a counter
  long unsigned number_of_sets; // number_of_lines/associativity
  long unsigned associativity;
  long unsigned line_size;
  long unsigned number_of_lines;
  long unsigned words_per_line;
  std::string replacement_policy;

  int number_of_reads;
  int number_of_writes;

  // Cache
  /***************************************************************/
  bool    ** Cache_Data;  /** The blocks are Cache_Data[set][line], with all words
                           *    from the block.
                           * Cache_Data [set][line], that's enough, because in
                           *    case we don't need informations about word and
                           *    all words have one byte, and the fourth position of
                           *    the array would be for acess the bytes in a word
                           *    Cache [number_of_sets][cache_description.associativity].
                           * This data is modeled with a value 1 (there is data)
                           *    and  0 (there isn't data) in the block.
                           */
  long unsigned ** Cache_Upper; /** The informations about Upper is used to differentiate
                                  *     two or more blocks with the same set (index)
                                  *     information. Sets aren't ordered by upper or any
                                  *     particular order.
                                  */
  long unsigned ** T_Access;      /** Time stamp (Used a counter) for the access
                                    *     in the cache memory (usedin the LRU Algorithm).
                                    */
  long unsigned ** T_Load;        /** Time stamp (Used a counter) for the data
                                    *     load in the cache memory(used in the FIFO Algorithm).
                                    */
  unsigned int ** T_Counter;       /** counter for accesses (used for LFU Algorithm)
                                  */

  bool ** write_bit;              /* this bit will be set if a write happens on this address to check for write trough r write back*/
  /***************************************************************/

  // results
  /***************************************************************/
  int acess_count;
  int read_hits;
  int read_misses;
  int write_hits;
  int write_misses;
  int write_conflicts;
  /***************************************************************/


  int startCache();
  long unsigned make_upper(long unsigned , int);
  int make_index(long unsigned );
  int getPosUpper (int , long unsigned );
  int there_Are_Space_Set( int );
  int random_free_space_set( int );
  int findLessAccessTSset( int );
  int findLessLoadTSset ( int );
  int findLeastUsedSet( int );
  void write_cache (int , long unsigned, int);
  void read_cache (int , long unsigned, int );
  /******************************************************************************/
public:
  unsigned int n_of_lines;
  unsigned int l_size;
  CacheClass();
  CacheClass(int, int, int, std::string, bool); // line size, number of lines, associativity, input file, debug
  void read(long unsigned, int);
  void write(long unsigned, int);
  int getNumberOfLines(){
    return this->number_of_lines;
  };
  int getLineSize(){
    return this->line_size;
  };
  float getWriteConflicts(){
    if(this->write_hits != 0 && this->write_misses != 0 && this->write_conflicts != 0){
      return (float)this->write_conflicts / (float)(this->write_hits + this->write_misses);
    }
    return 0.0;
  };
  int getAssociativity(){
    return this->associativity;
  }
  ~CacheClass(){
    this->Cache_Data  = NULL;  // "Free" in the memory for the Cache_Data[][]
    this->Cache_Upper = NULL;  // "Free" in the memory for the Cache_Upper[][]
    this->T_Access    = NULL;  // "Free" in the memory for the T_Access[][]
    this->T_Load      = NULL;  // "Free" in the memory for the T_Load[][]

  }
  void clear(){
    this->acess_count = 0;
    this->read_hits = 0;
    this->write_hits = 0;
    this->write_conflicts = 0;
    this->write_misses = 0;
    this->read_misses = 0;
    this->currently_clk = 0;
    this->debug = 0;
    this->number_of_reads = 0;
    this->number_of_writes = 0;
    this->Cache_Data  = NULL;  // "Free" in the memory for the Cache_Data[][]
    this->Cache_Upper = NULL;  // "Free" in the memory for the Cache_Upper[][]
    this->T_Access    = NULL;  // "Free" in the memory for the T_Access[][]
    this->T_Load      = NULL;  // "Free" in the memory for the T_Load[][]
  };
  void result();
  float readHits();
  float writeHits();
  int readHitsCount(){
    return this->write_hits;
  };
  std::string getPolicy(){
    return this->replacement_policy;
  }
};
