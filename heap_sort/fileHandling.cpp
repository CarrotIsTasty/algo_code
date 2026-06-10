#include "fileHandling.h"

using namespace std;

fileOperations::fileOperations() {
}

fileOperations::~fileOperations() {
}

bool fileOperations::loadFile(const string& filename, vector<Record>& data) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return false;
    }

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string numStr, str;

        if (!getline(ss, numStr, ',')) {
            cerr << "Malformed line (missing number): " << line << endl;
            continue; 
        }
        if (!getline(ss, str)) {
            cerr << "Malformed line (missing string): " << line << endl;
            continue; 
        }

        try {
            unsigned long long num = stoull(numStr);
            data.emplace_back(num, str);
        } catch (const invalid_argument& e) {
            cerr << "Invalid number in line: " << line << endl;
            continue; 
        } catch (const out_of_range& e) {
            cerr << "Number out of range in line: " << line << endl;
            continue; 
        }
    }

    infile.close();
    return true;
}

bool fileOperations::saveFile(const string& filename, const vector<Record>& data) {
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return false;
    }

    for (const auto& record : data) {
        outfile << record.number << "," << record.str << "\n";
    }

    outfile.close();
    return true;
}



