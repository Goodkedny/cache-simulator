/*************************************************************************
 * filename: cachesim.h
 * 
 * description: Defines classes used in cachesim.cpp
 * 
 * authors: William Fong, Theresa Guinard
 * 
 * class: CSE 331
 * instructor: Zheng
 * assignment: Lab Project #2
 * 
 * assigned: Oct 2, 2014
 * due: Oct 21, 2014
 * 
*************************************************************************/

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <deque>
#include <cmath>

class CacheConfig{
public:
    //constructor reads config from file
    CacheConfig(std::string config_filename);
    
    //getters
    unsigned get_line_size(){return this->line_size;}
    unsigned get_assoc(){return this->assoc;}
    unsigned get_data_size(){return this->data_size;}
    bool is_fifo(){return this->fifo;}
    unsigned get_miss_penalty(){return this->miss_penalty;}
    unsigned is_write_allocate(){return this->write_allocate;}
    unsigned get_offset_bits(){return this->offset_bits;}
    unsigned get_set_bits(){return this->set_bits;}

    //Print method
    void printConfig();

private:
    unsigned line_size;
    unsigned assoc;
    unsigned data_size;
    bool fifo;
    unsigned miss_penalty;
    bool write_allocate;

    unsigned offset_bits;   //number of bits specifing offset in address
    unsigned set_bits;      //number of bits specifing set index in address
};


class CacheSimulator{
public:
    CacheSimulator(CacheConfig* config);
    void simulateTrace(std::string filename);
    unsigned loadInstruction(unsigned address);
    unsigned writeInstruction(unsigned address);
    void writeResults(std::string filename);
private:
    CacheConfig* config;
    
    unsigned num_loads = 0;
    unsigned num_stores = 0;
    unsigned load_hits = 0;
    unsigned store_hits = 0;
    unsigned total_cycles = 0;
    unsigned total_memory_cycles = 0;
    
    std::vector< std::deque<unsigned> > cache_sets;
    
    void reset_cache();
};

class BlockIdentifier {
private:
	CacheConfig *config;
	//Create block byte field
	unsigned blockOffset;
	unsigned setIndex;
	unsigned tag;

public:
	BlockIdentifier(CacheConfig* config, unsigned address);
	unsigned getOffset(){return this->blockOffset;}
	unsigned getIndex(){return this->setIndex;}
	unsigned getTag(){return this->tag;}
	void printBlock();

};
