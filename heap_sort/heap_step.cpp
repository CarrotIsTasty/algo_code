

#include "heap_step.h"
#include <filesystem>

namespace fs = std::filesystem;

HeapStepDemo::HeapStepDemo() {
    fileIsOpen = false;
    outputFilename = "";
}

HeapStepDemo::~HeapStepDemo() {
    close();
}

bool HeapStepDemo::createOutputFile(const string& inputFile, int startRow, int endRow) {
    // Close existing file if open
    close();
    
    // Build output path in same directory as input
    fs::path inPath(inputFile);
    string basename = inPath.stem().string(); // filename without extension
    fs::path outPath = inPath.parent_path() / (basename + "_heap_sorted_step_" + to_string(startRow) + "_" + to_string(endRow) + ".txt");
    outputFilename = outPath.string();

    outFile.open(outputFilename);
    if (!outFile.is_open()) {
        cerr << "ERROR: Cannot create file: " << outputFilename << endl;
        fileIsOpen = false;
        return false;
    }
    
    fileIsOpen = true;
    cout << "Output file: " << outputFilename << endl;
    return true;
}


void HeapStepDemo::close() {
    if (outFile.is_open()) {
        outFile.close();
    }
    fileIsOpen = false;
}


void HeapStepDemo::printArray(const vector<Record>& arr, const string& label) {
    if (!fileIsOpen) return;
    
    outFile << "[";
    for (size_t i = 0; i < arr.size(); i++) {
        outFile << arr[i].number << "/" << arr[i].str   ; 
        if (i < arr.size() - 1) {
            outFile << ", ";  
        }
    }
    outFile << "] " << label << "\n";  
}


void HeapStepDemo::heapifyStep(vector<Record>& arr, int n, int i, int stepNum) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && arr[left].number > arr[largest].number) {
        largest = left;
    }
    
    if (right < n && arr[right].number > arr[largest].number) {
        largest = right;
    }
    
    if (largest != i) {
        swap(arr[i], arr[largest]);
        
        if (stepNum > 0) {
            printArray(arr, "i = " + to_string(stepNum));
        }
        
        heapifyStep(arr, n, largest, stepNum);
    }
}


void HeapStepDemo::runHeapSortWithSteps(vector<Record>& arr) {
    int n = arr.size();
    
    if (n <= 1) {
        printArray(arr, "initial");
        return;
    }
    

    printArray(arr, "initial");
    

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyStep(arr, n, i, -1);  
    }
    
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        

        printArray(arr, "i = " + to_string(i));
        
        heapifyStep(arr, i, 0, i);
    }
}