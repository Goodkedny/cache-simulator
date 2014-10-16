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
    this->cache_sets = std::vector< std::deque<unsigned> >();
    
    int num_sets = 1 << this->config->get_set_bits();
    
    for(int i = 0; i < num_sets; ++i){
        this->cache_sets.push_back(std::deque<unsigned>());
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

/**
 * Create new configuration from filename
 */
CacheConfig::CacheConfig(std::string config_filename) {
    std::ifstream configFile;
    short int buf;
    configFile.open(config_filename);
    configFile >> this->line_size; //Read in Line Size
    configFile >> this->assoc; //Read in Cache Associativity
    configFile >> this->data_size; //Read in Data Size
    configFile >> buf; //Read in Replacement Policy
    if (buf == 0) {
        //Replacement Policy is Random Placement
        this->fifo = false;
    } else if (buf == 0) {
        //Replacement Policy is FIFO
        this->fifo = true;
    }
    configFile >> this->miss_penalty; //Read in Miss Penalty
    
    configFile >> buf; //Read in Write Policy
    if (buf == 0) {
        //Write Policy is No Write Allocate
        this->write_allocate = false;
    } else if (buf == 1) {
        //Write Policy is Write Allocate
        this->write_allocate = true;
    }
    //Clean up
    configFile.close();
    
}


int main(int argc, char** argv){
    if(argc != 3){
        std::cerr << "Usage: " << argv[0] << " config_file trace_file" << std::endl;
        exit(1);
    }
    //Test config
    CacheConfig testConfig = CacheConfig(argv[1]);
    std::cout << "Line Size:     " << testConfig.get_line_size() << std::endl;
    std::cout << "Associativity: " << testConfig.get_assoc() << std::endl;
    std::cout << "Data Size:     " << testConfig.get_data_size() << std::endl;
    std::cout << "Miss Penalty:  " << testConfig.get_miss_penalty() << std::endl;
}