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

vector<Data> ReadData(const string& filename) {
    vector<Data> dataset;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return dataset;
    }

    string line;
    while (getline(file, line)) {
        size_t comma_pos = line.find(',');
        if (comma_pos != string::npos) {
            try {
                long long num = static_cast<long long>(stoull(line.substr(0, comma_pos)));
                string id_str = line.substr(comma_pos + 1);
                dataset.push_back({num, id_str});
            } catch (const invalid_argument& e) {
                cerr << "Skipping line (invalid number): " << line << endl;
            } catch (const out_of_range& e) {
                cerr << "Skipping line (number out of range): " << line << endl;
            }
        } else {
            cerr << "Skipping line (no comma): " << line << endl;
        }
    }
    cout << "Dataset file has been read\n";
    return dataset;
}

// LSD (Least Significant Digit) Radix Sort — base 10
void RadixSort(vector<Data>& arr) {
    if (arr.empty()) return;

    // Find the maximum number to know how many digits we need to process
    long long max_num = arr[0].num;
    for (const auto& d : arr) {
        if (d.num > max_num) max_num = d.num;
    }

    vector<Data> temp(arr.size());

    // Process each decimal digit (LSD → MSD)
    for (long long exp = 1; max_num / exp > 0; exp *= 10) {
        int count[10] = {0};

        // Count occurrences of each digit
        for (const auto& d : arr) {
            int digit = static_cast<int>((d.num / exp) % 10);
            count[digit]++;
        }

        // Prefix sums → starting positions
        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }

        // Build sorted output (iterate backwards for stability)
        for (int i = static_cast<int>(arr.size()) - 1; i >= 0; --i) {
            int digit = static_cast<int>((arr[i].num / exp) % 10);
            temp[--count[digit]] = arr[i];
        }

        arr.swap(temp);
    }
}

// Human-readable timestamp for log output
string GetTimestamp() {
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
    return string(buf);
}

// Filename-safe timestamp (no spaces or colons)
string GetTimestampFile() {
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", local);
    return string(buf);
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

// Write all sorted records to a CSV file
void WriteSortedCSV(const vector<Data>& dataset, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Failed to create sorted CSV: " << filename << endl;
        return;
    }
    for (const auto& d : dataset) {
        out << d.num << "," << d.id << "\n";
    }
    out.close();
    cout << "Sorted CSV written to " << filename << "\n";
}

int main(int argc, char* argv[]) {
    // ---- 1. Determine dataset filename ----
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        cout << "Enter dataset filename: ";
        getline(cin, filename);
    }

    // ---- 2. Read data (NOT timed) ----
    vector<Data> dataset = ReadData(filename);
    if (dataset.empty()) {
        cerr << "No data loaded. Exiting." << endl;
        return 1;
    }
    cout << "Loaded " << dataset.size() << " records.\n";

    // ---- 3. Radix Sort (timed) ----
    auto start = high_resolution_clock::now();
    RadixSort(dataset);
    auto end = high_resolution_clock::now();
    double elapsed = duration<double>(end - start).count();

    cout << "Sort completed in " << elapsed << " seconds.\n";

    // ---- 4. Write results to output file ----
    string base_name = GetBaseName(filename);
    string out_filename = "radix_sort_step_" + GetTimestampFile() + "_" + base_name + ".txt";
    ofstream out(out_filename);
    if (!out.is_open()) {
        cerr << "Failed to create output file." << endl;
        return 1;
    }

    out << "* Radix Sort " << GetTimestamp() << "\n";
    out << "* Execution time: " << elapsed << " seconds\n";
    out << "* Dataset File: " << filename << "\n";
    out << "* Records sorted: " << dataset.size() << "\n";
    out << "\n";
    out << "First 10 sorted records:\n";
    int first_n = (dataset.size() < 10) ? (int)dataset.size() : 10;
    for (int i = 0; i < first_n; ++i) {
        out << "  " << dataset[i].num << "," << dataset[i].id << "\n";
    }
    out << "...\n";
    out << "Last 10 sorted records:\n";
    int start_i = ((int)dataset.size() > 10) ? (int)dataset.size() - 10 : 0;
    for (int i = start_i; i < (int)dataset.size(); ++i) {
        out << "  " << dataset[i].num << "," << dataset[i].id << "\n";
    }

    out.close();
    cout << "Results written to " << out_filename << "\n";

    // ---- 5. Write sorted CSV ----
    string csv_filename = "sorted_radix_dataset_" + to_string(dataset.size()) + ".csv";
    WriteSortedCSV(dataset, csv_filename);

    return 0;
}
