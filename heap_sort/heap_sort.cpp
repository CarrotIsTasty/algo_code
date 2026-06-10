#include "heap_sort.h"

using namespace std;

void HeapSort::swap(Record& a, Record& b) {
    Record temp = a;
    a = b;
    b = temp;
}

HeapSort::HeapSort() {
   // because we need to create empty array
}


void HeapSort::maxHeapBuild() {
    int n = data.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(n, i);
    }
}


//This is from lecture and lab
void HeapSort::heapify(int n, int i) {
    int largest = i; 
    int left = 2 * i + 1; 
    int right = 2 * i + 2; 

    // If left child is larger than root
    if (left < n && data[left].number > data[largest].number)
        largest = left;

    // If right child is larger than largest so far
    if (right < n && data[right].number > data[largest].number)
        largest = right;

    // If largest is not root
    if (largest != i) {
        swap(data[i], data[largest]);

        // Recursively heapify 
        heapify(n, largest);
    }
}


void HeapSort::sort() {
    int n = data.size();

    maxHeapBuild();

    for (int i = n - 1; i > 0; i--) {
        swap(data[0], data[i]);
        heapify(i, 0); 
    }
}


bool HeapSort::isSorted() {
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i - 1].number > data[i].number) {
            return false;
        }
    }
    return true;
}

int HeapSort::getSize() {
    return data.size();
}

void HeapSort::sortWithSteps(string outputFile) {
    ofstream outfile(outputFile);
    if (!outfile.is_open()) {
        cerr << "Failed to open file: " << outputFile << endl;
        return;
    }

    int n = data.size();
    maxHeapBuild();

    for (int i = n - 1; i > 0; i--) {
        swap(data[0], data[i]);
        heapify(i, 0); 

        // Write current state to file
        for (const auto& record : data) {
            outfile << record.number << "," << record.str << "\n";
        }
        outfile << "---- Step " << (n - i) << " ----\n";
    }

    outfile.close();
}


