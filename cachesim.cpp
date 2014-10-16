/*************************************************************************
 * filename: cachesim.cpp
 * 
 * description: Implements the cache simulator.
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

#include "cachesim.h"

void CacheSimulator::simulateTrace(std::string filename){
    this->reset_cache();
    
    //todo: simulation
}

/**
 * Reset the cache
 * Called at the beginning of simulation
 */
void CacheSimulator::reset_cache()
{
    this->cache_sets = std::vector< std::deque<int> >();
    
    int num_sets = 1 << this->config->get_set_bits();
    
    for(int i = 0; i < num_sets; ++i){
        this->cache_sets.push_back(std::deque<int>());
    }
    
    this->num_loads = 0;
    this->num_stores = 0;
    this->load_hits = 0;
    this->store_hits = 0;
    this->total_cycles = 0;
    this->total_memory_cycles = 0;
}

CacheSimulator::CacheSimulator(CacheConfig* config){
    this->config = config;
}

int main(int argc, char** argv){
    if(argc != 3){
        std::cerr << "Usage: " << argv[0] << " config_file trace_file" << std::endl;
        exit(1);
    }
}