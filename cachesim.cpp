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
    this->resetCache();

    std::ifstream traceFile;
    std::string buffer;
    char instruction;     //Variable to hold load or write instruction specifier
    unsigned address;     //Variable to hold instruction address
    unsigned nonMemInstr; //Variable to hold non-memory access instruction count

    traceFile.open(filename);
    //Get each instruction from trace file
    while(getline(traceFile, buffer)) {

        sscanf(buffer.c_str(), "%c %x %d", &instruction, &address, &nonMemInstr);
        if (instruction == 's') {
            //This instruction is a Write!
            this->total_memory_cycles += writeInstruction(address); //Add number of memory cycles this took
        } else {
            //This instruction is a Load!
            this->total_memory_cycles += loadInstruction(address); //Add number of memory cycles this took
        }
        this->total_non_memory_cycles += nonMemInstr; //Add previous non-memory instructions (Assumed all to take only 1 cycle each)
    }
    //Cleanup
    traceFile.close();
}

/**
 * determine if an address is in the cache
 */
bool CacheSimulator::isHit(BlockIdentifier block){
    unsigned tag = block.getTag();
    unsigned set = block.getIndex();
    
    for(unsigned entry : cache_sets[set]){
        if(tag == entry){
            return true;
        }
    }
    return false;
}

/**
 * Load block into cache
 * assumes block is not in cache
 */
void CacheSimulator::loadIntoCache(BlockIdentifier block){
    unsigned tag = block.getTag();
    unsigned set = block.getIndex();
    
    //kick someone out if necessary
    if(this->cache_sets[set].size() == this->config->getAssoc()){
        if(this->config->isFifo()){
            this->cache_sets[set].pop_front();
        }
        else{
            int rand_pos = rand() % this->cache_sets[set].size();
            this->cache_sets[set].erase(this->cache_sets[set].begin() + rand_pos);
        }
    }
    
    this->cache_sets[set].push_back(tag);
}

/**
 * Perform a load instruction at the specified address
 * @return The total number of cycles this instruction took
 */
unsigned CacheSimulator::loadInstruction(unsigned address) {
    unsigned totalCycleTime = 1; //assume hit time is one
    
    (this->num_loads)++;
    
    BlockIdentifier block = BlockIdentifier(this->config, address);
    
    if(this->isHit(block)){
        (this->load_hits)++;
    }
    else{
        this->loadIntoCache(block);
        totalCycleTime += this->config->getMissPenalty();
    }

    return totalCycleTime;
}

/**
 * Perform a write instruction at the specified address
 * @return The total number of cycles this instruction took
 */
unsigned CacheSimulator::writeInstruction(unsigned address) {
    unsigned totalCycleTime = 1; //assume hit time is one
    
    (this->num_stores)++;
    
    BlockIdentifier block = BlockIdentifier(this->config, address);
    
    if(this->isHit(block)){
        (this->store_hits)++;
        if(! this->config->isWriteAllocate()){ //write through
            totalCycleTime += this->config->getMissPenalty();
        }
    }
    else{
        totalCycleTime += this->config->getMissPenalty();
        
        if(this->config->isWriteAllocate()){
            this->loadIntoCache(block);
        }
    }
    
    return totalCycleTime;
}

/**
 * Write result of simulation to output file
 */
void CacheSimulator::writeResults(std::string filename){
    std::ofstream outfile;
    outfile.open(filename);
    
    //total hit rate
    outfile << ((float) (this->load_hits + this->store_hits)) / (this->num_loads + this->num_stores) << std::endl;
    
    //load hit rate
    outfile << ((float) this->load_hits) / this->num_loads << std::endl;
    
    //store hit rate
    outfile << ((float) this->store_hits) / this->num_stores << std::endl;
    
    //total run time
    outfile << this->total_memory_cycles + this->total_non_memory_cycles << std::endl;
    
    //average memory access latency
    outfile << ((float) this->total_memory_cycles) / (this->num_loads + this->num_stores) << std::endl;
    
    outfile.close();
}

/**
 * Reset the cache
 * Called at the beginning of simulation
 */
void CacheSimulator::resetCache()
{
    this->cache_sets = std::vector< std::deque<unsigned> >();
    
    int num_sets = 1 << this->config->getSetBits();
    
    for(int i = 0; i < num_sets; ++i){
        this->cache_sets.push_back(std::deque<unsigned>());
    }
    
    this->num_loads = 0;
    this->num_stores = 0;
    this->load_hits = 0;
    this->store_hits = 0;
    this->total_non_memory_cycles = 0;
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
    configFile >> this->line_size;	//Read in Line Size
    configFile >> this->assoc; 		//Read in Cache Associativity
    configFile >> this->data_size; 	//Read in Data Size
    configFile >> buf; 				//Read in Replacement Policy
    if (buf == 0) {
        this->fifo = false;	//Replacement Policy is Random Placement
    } else if (buf == 1) {
        this->fifo = true;	//Replacement Policy is FIFO
    }
    configFile >> this->miss_penalty; //Read in Miss Penalty
    
    configFile >> buf; //Read in Write Policy
    if (buf == 0) {
        this->write_allocate = false; //Write Policy is No Write Allocate
    } else if (buf == 1) {
        this->write_allocate = true;  //Write Policy is Write Allocate
    }
    //Clean up
    configFile.close();
    
    int num_blocks = this->data_size * 1024 / this->line_size;

    //if fully associative, can change associativity to num_blocks (more uniform to work with)
    if(this->assoc == 0){
        this->assoc = num_blocks;
    }
    
    int num_sets = num_blocks / assoc;
    this->set_bits = 0;
    while(num_sets > 1){
        this->set_bits += 1;
        num_sets >>= 1;
    }
    
    
    this->offset_bits = 0;
    int tmp_blocksize = this->line_size;
    while(tmp_blocksize > 1){
        this->offset_bits += 1;
        tmp_blocksize >>= 1;
    }
}

/**
 * This method prints out the config file
 */
void CacheConfig::printConfig() {
    std::cout << "Line Size:............" << this->line_size << "b" << std::endl;
    std::cout << "Associativity:........" << this->assoc << std::endl;
    std::cout << "Data Size:............" << this->data_size << "Kb" << std::endl;
    std::cout << "Replacement Policy:...";
    if (this->fifo == true)
        std::cout << "FIFO" << std::endl;
    else
        std::cout << "Random" << std::endl;
    std::cout << "Miss Penalty:........." << this->miss_penalty << std::endl;
    std::cout << "Write Policy:.........";
    if (this->write_allocate ==  true)
        std::cout << "Write Allocate" << std::endl;
    else
        std::cout << "No Write Allocate" << std::endl;
}

/**
 * Creates a new Block Identifier
 * @param config The configuration parameters for the simulation
 * @param address The requested address to be converted into a tag, set index, and block offset
 */
BlockIdentifier::BlockIdentifier(CacheConfig* config, unsigned address) {
	this->config = config;
	unsigned numOffsetBits = this->config->getOffsetBits();
	unsigned numSetBits = this->config->getSetBits();
	unsigned numTagBits = 32 - numOffsetBits - numSetBits; //Remainder of bits is used for tag

	//Create Bit Masks to extract information from address
	//This extracts each specific part of the address
	//Ex Offset bitmask: 000....0001111
	unsigned offsetMask = pow(2,numOffsetBits) - 1; //Create bitmask for Offset
	//Ex Set Index bitmask: 000...0011111000....00000
	unsigned setMask = pow(2, numSetBits + numOffsetBits) - 1; //Create bitmask for Set Index
	setMask = setMask ^ offsetMask;
	//Ex Tag bitmask: 11110000...00000
	unsigned tagMask = pow(2,32) - 1; //Create bitmask for tag
	tagMask = tagMask ^ (offsetMask | setMask);

	//Extract bits from address
	this->tag = tagMask & address;
	this->tag = this->tag >> (32 - numTagBits); //Right shift tag to represent actual value from mask
	this->setIndex = setMask & address;
	this->setIndex = this->setIndex >> (numOffsetBits); //Right shift index to represent actual value from mask
	this->blockOffset = offsetMask & address;
}

/**
 * Prints out a Block Identifier
 */
void BlockIdentifier::printBlock() {
	std::cout << "BLOCK" << std::endl;
	std::cout << "Tag:............" << "0x" << std::hex << this->tag << std::dec << std::endl;
	std::cout << "Set Index:......" << "0x" << std::hex << this->setIndex << std::dec << std::endl;
	std::cout << "Block Offset:..." << this->blockOffset << std::endl << std::endl;
}

int main(int argc, char** argv){
    if(argc != 3){
        std::cerr << "Usage: " << argv[0] << " config_file trace_file" << std::endl;
        exit(1);
    }
    //Setup Configuration
    CacheConfig simConfig = CacheConfig(argv[1]); //Load configuration file from command args
    CacheSimulator simulator = CacheSimulator(&simConfig); //Initialize Simulator with configurations
    //Perform simulation
    simulator.simulateTrace(argv[2]);
    simulator.writeResults(std::string(argv[2]) + ".out");
}
