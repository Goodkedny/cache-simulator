/*************************
 /
 /      filename: config.h
 /
 /      description: A class file for containing configuration settings
 /
 /      authors:    Fong, William
 /                  Guinard, Theresa
 /
 /      class:      CSE 331
 /      instructor: Zheng
 /      assignment: Lab Project #2
 /
 /      assigned:   Oct 2, 2014
 /      due:        Oct 21, 2014
 /
 ************************/
#ifndef CONFIG_H_
#define CONFIG_H_
#include <iostream>
#include <fstream>

using namespace std;

/*
 * This class contains the information within a configuration file for the cache simulator
 */

class config {
private:
    //Variables for the Cache Simulation Environment
    unsigned int lineSize, cacheAsso, dataSize, missPenalty;
    bool randomPlace, noWriteAllo;
public:
    config(string filename);
    ~config();
    void printConfig();

    //Getters for the variables
    unsigned int getLineSize();
    unsigned int getCacheAsso();
    unsigned int getDataSize();
    unsigned int getMissPenalty();
    bool isRandomPlace();
    bool isNoWriteAllo();
};

/**
 * This is the main constructor for the config class.
 * @Param filename This string contains the configuration filename
 */
config::config(string filename) {
    
    ifstream configFile;
    short int buf;
    configFile.open(filename);
    
    //Read in Line Size
    configFile >> lineSize;

    //Read in Cache Association
    configFile >> cacheAsso;

    //Read in Data Size
    configFile >> dataSize;

    //Read in Replacement Policy
    configFile >> buf;
    if (buf == 0) {
        //Config has specified that the Replacement Policy is Random Placement
        randomPlace = true;
    } else if (buf == 0) {
        //Config has specified that the Replacement Policy is FIFO
        randomPlace = false;
    }

    //Read in Data Size
    configFile >> dataSize;

    //Read in Write Policy
    configFile >> buf;
    if (buf == 0) {
        //Config has specified that the Write Policy is No Write Allocate
        noWriteAllo = true;
    } else if (buf == 1) {
        //Config has specified that the Write Policy is Write Allocate
        noWriteAllo = false;
    }

    //Clean up
    configFile.close();
}

/**
 * Destructor for Config file (not needed at this moment)
 */
config::~config() {
    
}

/**
 * This method is for printing out the configuration settings
 */
void config::printConfig() {
    cout << "Line Size: " << lineSize << "\n";
    cout << "Cache Association: " << cacheAsso << "\n";
    cout << "Data Size: " << dataSize << "\n";
    cout << "Miss Penalty: " << missPenalty << "\n";
    if (randomPlace == true)
        cout << "Replace Policy: Random Placement\n";
    else
        cout << "Replace Policy: FIFO\n";
    if (noWriteAllo ==  true)
        cout << "Write Policy: No Write Allocate\n";
    else
        cout << "Write Policy: Write Allocate\n";
}

/**
 * Returns the Line Size
 */
unsigned int config::getLineSize() {
    return lineSize;
}

/**
 * Returns the Cache Associativity
 */
unsigned int config::getCacheAsso() {
    return cacheAsso;
}

/**
 * Returns the Data Size of the Cache
 */
unsigned int config::getDataSize() {
    return dataSize;
}

/**
 * Returns the Miss Penalty for the simulation
 */
unsigned int config::getMissPenalty() {
    return missPenalty;
}

/**
 * Returns the Placement Policy: TRUE if Random Placement, FALSE if FIFO
 */
bool config::isRandomPlace() {
    return randomPlace;
}

/**
 * Returns the Write Policy: TRUE if No-Write-Allocate, FALSE if Write-Allocate
 */
bool config::isNoWriteAllo() {
    return noWriteAllo;
}

#endif
