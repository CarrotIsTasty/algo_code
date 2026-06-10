/**
 * step_main.cpp
 * Main program for Heap Sort step demo
 * 
 * COMPILE: g++ -std=c++11 step_main.cpp FileHandling.cpp HeapStepDemo.cpp -o heap_sort_step
 * 
 * RUN: ./heap_sort_step dataset_1000.csv 1 7
 */

#include "fileHandling.h"
#include "heap_step.h"


bool loadRows(const string& filename, vector<Record>& data, int startRow, int endRow) {
    data.clear();
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "ERROR: Cannot open file: " << filename << endl;
        return false;
    }
    
    string line;
    int currentRow = 1;
    
    while (getline(file, line)) {
        if (currentRow >= startRow && currentRow <= endRow) {
            size_t commaPos = line.find(',');
            if (commaPos != string::npos) {
                string numStr = line.substr(0, commaPos);
                string textStr = line.substr(commaPos + 1);
                
                try {
                    long long number = stoll(numStr);
                    data.push_back(Record(number, textStr));
                } catch (...) {
                    // Skip invalid rows
                }
            }
        }
        currentRow++;
        if (currentRow > endRow) break;
    }
    
    file.close();
    
    if (data.empty()) {
        cerr << "ERROR: No data found in rows " << startRow << "-" << endRow << endl;
        return false;
    }
    
    cout << "Loaded " << data.size() << " records (rows " << startRow << "-" << endRow << ")" << endl;
    return true;
}

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc != 4) {
        cout << "HEAP SORT - STEP BY STEP DEMO" << endl;
        cout << "Usage: " << argv[0] << " <dataset.csv> <start_row> <end_row>" << endl;
        cout << "Example: " << argv[0] << " dataset_1000.csv 1 7" << endl;
        return 1;
    }
    
    string inputFile = argv[1];
    int startRow = atoi(argv[2]);
    int endRow = atoi(argv[3]);
    
    if (startRow < 1 || endRow < startRow) {
        cerr << "ERROR: start_row must be >= 1 and <= end_row" << endl;
        return 1;
    }
    
    cout << "Input file: " << inputFile << endl;
    cout << "Rows: " << startRow << " to " << endRow << endl;
    
    // Load the specified rows
    vector<Record> data;
    if (!loadRows(inputFile, data, startRow, endRow)) {
        return 1;
    }
    
    HeapStepDemo demo;
    
    if (!demo.createOutputFile(inputFile, startRow, endRow)) {
        return 1;
    }
    
    
    demo.runHeapSortWithSteps(data);
    
    demo.close();
    
    cout << "SUCCESS!" << endl;
    cout << "Output saved to: " << demo.getOutputFilename() << endl;
    
    cout << "\nOutput preview (first 10 lines):" << endl;
    
    ifstream previewFile(demo.getOutputFilename());
    string line;
    int lineCount = 0;
    while (getline(previewFile, line) && lineCount < 10) {
        cout << line << endl;
        lineCount++;
    }
    previewFile.close();
    
    if (lineCount >= 10) {
        cout << "... (full output in file)" << endl;
    }
    
    return 0;
}