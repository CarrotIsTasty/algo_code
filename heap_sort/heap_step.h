
#ifndef HEAP_STEP_DEMO_H
#define HEAP_STEP_DEMO_H

#include "fileHandling.h"
#include <fstream>

using namespace std;

class HeapStepDemo {
private:
    ofstream outFile;        
    string outputFilename;   
    bool fileIsOpen;         
    
    void printArray(const vector<Record>& arr, const string& label);
    
    /**
     * 
     * 
     * @param arr - Array to heapify
     * @param n - Size of heap
     * @param i - Index to heapify
     * @param stepNum - Step number for label (or -1 to skip printing)
     */
    void heapifyStep(vector<Record>& arr, int n, int i, int stepNum);
    
public:

    HeapStepDemo();
    ~HeapStepDemo();
    
    /**
     * Create output file for step demo
     * 
     * @param inputFile - Original CSV file (e.g., "dataset_1000.csv")
     * @param startRow - Start row number
     * @param endRow - End row number
     * @return true if successful
     */
    bool createOutputFile(const string& inputFile, int startRow, int endRow);
    
    void close();
    
    /**
     * Run full Heap Sort with step-by-step output
     * 
     * @param arr - Array to sort (will be modified)
     */
    void runHeapSortWithSteps(vector<Record>& arr);
    string getOutputFilename() const {
        return outputFilename;
    }
};

#endif