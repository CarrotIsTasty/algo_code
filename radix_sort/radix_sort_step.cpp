#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

using namespace std;
using namespace std::chrono;

struct Data {
    long long num;
    string id;
};

// Load only rows from startRow to endRow (1-indexed)
vector<Data> ReadDataRows(const string& filename, int startRow, int endRow) {
    vector<Data> dataset;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return dataset;
    }

    string line;
    int currentRow = 1;
    while (getline(file, line)) {
        if (currentRow >= startRow && currentRow <= endRow) {
            size_t comma_pos = line.find(',');
            if (comma_pos != string::npos) {
                try {
                    long long num = static_cast<long long>(stoull(line.substr(0, comma_pos)));
                    string id_str = line.substr(comma_pos + 1);
                    dataset.push_back({num, id_str});
                } catch (...) {
                    cerr << "Skipping invalid line at row " << currentRow << endl;
                }
            }
        }
        currentRow++;
        if (currentRow > endRow) break;
    }
    return dataset;
}

// Extract base filename without path and extension
string GetBaseName(const string& path) {
    size_t slash = path.find_last_of("/\\");
    size_t dot = path.find_last_of('.');
    string name = (slash != string::npos) ? path.substr(slash + 1) : path;
    if (dot != string::npos && dot > (slash != string::npos ? slash : 0)) {
        name = name.substr(0, dot - (slash != string::npos ? slash + 1 : 0));
    }
    return name;
}

// Write full array state
void WriteArrayState(ofstream& out, const vector<Data>& arr, const string& label) {
    out << "[" << label << "] ";
    for (size_t i = 0; i < arr.size(); i++) {
        out << arr[i].num << "/" << arr[i].id;
        if (i < arr.size() - 1) out << ", ";
    }
    out << "\n";
}

// Write temp array showing _ for unfilled slots
void WriteTempState(ofstream& out, const vector<Data>& temp, const vector<bool>& filled, const string& label) {
    out << "  " << label << ": [";
    for (size_t i = 0; i < temp.size(); i++) {
        if (filled[i]) {
            out << temp[i].num << "/" << temp[i].id;
        } else {
            out << "_";
        }
        if (i < temp.size() - 1) out << ", ";
    }
    out << "]\n";
}

// LSD (Least Significant Digit) Radix Sort — base 10, every single step
void RadixSortSteps(vector<Data>& arr, ofstream& out) {
    if (arr.empty()) return;

    // Print initial state
    WriteArrayState(out, arr, "initial");
    out << "\n";

    // Find the maximum number to know how many digits we need to process
    long long max_num = arr[0].num;
    for (const auto& d : arr) {
        if (d.num > max_num) max_num = d.num;
    }

    vector<Data> temp(arr.size());
    int pass = 0;

    // Process each decimal digit (LSD → MSD)
    for (long long exp = 1; max_num / exp > 0; exp *= 10) {
        pass++;

        // ---- Phase 1: Count digit occurrences ----
        int count[10] = {0};
        for (const auto& d : arr) {
            int digit = static_cast<int>((d.num / exp) % 10);
            count[digit]++;
        }

        // Print digit counts
        out << "--- pass " << pass << " (exp=" << exp << ") ---\n";
        out << "digit counts:  ";
        for (int d = 0; d < 10; d++) {
            out << d << ":" << count[d];
            if (d < 9) out << "  ";
        }
        out << "\n";

        // ---- Phase 2: Compute prefix sums ----
        int prefix[10];
        for (int d = 0; d < 10; d++) prefix[d] = count[d];
        for (int i = 1; i < 10; ++i) {
            prefix[i] += prefix[i - 1];
        }

        out << "prefix sums:  ";
        for (int d = 0; d < 10; d++) {
            out << d << ":" << prefix[d];
            if (d < 9) out << "  ";
        }
        out << "\n";

        // ---- Phase 3: Place elements backwards (show each placement) ----
        out << "placing elements (backwards):\n";

        vector<bool> filled(arr.size(), false);
        int step = 0;

        for (int i = static_cast<int>(arr.size()) - 1; i >= 0; --i) {
            int digit = static_cast<int>((arr[i].num / exp) % 10);
            int pos = --prefix[digit];
            temp[pos] = arr[i];
            filled[pos] = true;
            step++;

            string label = "step " + to_string(step) + ": arr[" + to_string(i) + "]="
                         + to_string(arr[i].num) + "/" + arr[i].id
                         + "  digit=" + to_string(digit) + " -> pos " + to_string(pos);
            WriteTempState(out, temp, filled, label);
        }

        arr.swap(temp);

        // Print array after this pass
        out << "\n";
        WriteArrayState(out, arr, "after pass " + to_string(pass));
        out << "\n";
    }
}

int main(int argc, char* argv[]) {
    // ---- 1. Check command line arguments ----
    if (argc != 4) {
        cout << "RADIX SORT - STEP BY STEP DEMO" << endl;
        cout << "Usage: " << argv[0] << " <dataset.csv> <start_row> <end_row>" << endl;
        cout << "Example: " << argv[0] << " dataset_1000.csv 1 7" << endl;
        return 1;
    }

    string filename = argv[1];
    int startRow = atoi(argv[2]);
    int endRow = atoi(argv[3]);

    if (startRow < 1 || endRow < startRow) {
        cerr << "ERROR: start_row must be >= 1 and <= end_row" << endl;
        return 1;
    }

    cout << "Input file: " << filename << endl;
    cout << "Rows: " << startRow << " to " << endRow << endl;

    // ---- 2. Load specified rows only ----
    vector<Data> dataset = ReadDataRows(filename, startRow, endRow);
    if (dataset.empty()) {
        cerr << "ERROR: No data found in rows " << startRow << "-" << endRow << endl;
        return 1;
    }
    cout << "Loaded " << dataset.size() << " records.\n";

    // ---- 3. Build output filename: dataset_<n>_radix_sorted_step_<startRow>_<endRow>.txt ----
    string base_name = GetBaseName(filename);
    string out_filename = base_name + "_radix_sorted_step_" + to_string(startRow) + "_" + to_string(endRow) + ".txt";

    ofstream out(out_filename);
    if (!out.is_open()) {
        cerr << "Failed to create output file: " << out_filename << endl;
        return 1;
    }

    // ---- 4. Write header ----
    out << "* Radix Sort Step-by-Step\n";
    out << "* Dataset File: " << filename << "\n";
    out << "* Rows: " << startRow << " to " << endRow << "\n";
    out << "* Records: " << dataset.size() << "\n";
    out << "\n";

    // ---- 5. Run Radix Sort with step-by-step output (timed) ----
    auto start = high_resolution_clock::now();
    RadixSortSteps(dataset, out);
    auto end = high_resolution_clock::now();
    double elapsed = duration<double>(end - start).count();

    out << "\n";
    out << "* Execution time: " << elapsed << " seconds\n";

    out.close();

    // ---- 6. Done ----
    cout << "SUCCESS!" << endl;
    cout << "Output saved to: " << out_filename << endl;

    // Preview first 10 lines of output
    cout << "\nOutput preview (first 10 lines):" << endl;
    ifstream previewFile(out_filename);
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
