#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include "fileHandling.h"

#include <vector>
#include <string>
#include <fstream> // always need this for file handling
#include <iostream>


class HeapSort {
    private:
        std::vector<Record> data;

        //because we want to use this swap
        void swap(Record& a, Record& b);

        void heapify (int n, int i);

    public:
        
        HeapSort();
        void maxHeapBuild();
        void sort();
        bool isSorted();
        int getSize();
        void sortWithSteps(std::string outputFile);

        const std::vector<Record>& getData() const {
            return data;
        }
        void setData(const std::vector<Record>& newData) {
            data = newData;
        }
};

#endif
