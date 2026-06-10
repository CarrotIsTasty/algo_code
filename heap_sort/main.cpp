#include "heap_sort.h"
#include <chrono>
#include <filesystem>

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

int main(int argc, char* argv[]){
        if (argc != 2) {
        cout << "Usage: " << argv[0] << " <dataset_file.csv>" << endl;
        cout << "Example: " << argv[0] << "dataset_1000.csv" << endl;
        return 1;
    }

    string filename = argv[1];

    //create fileoperation obj and also heapsort obj
    fileOperations fh;
    HeapSort sorter;


    // load csv file
    vector<Record> data;
    if (!fh.loadFile(filename, data)) {
        cerr << "Failed to load file: " << filename << endl;
        return 1;
    }

    cout<< "Heap sort algo" << endl;
    cout << "File name: " << filename << endl;
    cout << "Number of records: " << data.size() << endl;

    sorter.setData(data);

    // Record the time for the sort
    auto start = high_resolution_clock::now();
    sorter.sort();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Time taken to sort: " << duration.count() << " milliseconds" << endl;

    const auto& sorted = sorter.getData();

    //save sorted data to new file (keep directory if present)
    fs::path inPath(filename);
    fs::path outPath = inPath.parent_path() / (string("sorted_") + inPath.filename().string());
    string outputFilename = outPath.string();
    if (!fh.saveFile(outputFilename, sorted)) {
        cerr << "Failed to save sorted data to: " << outputFilename << endl;
        return 1;
    }
    cout << "Sorted data saved to: " << outputFilename << endl;

    return 0;

}