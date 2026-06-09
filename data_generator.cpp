/**
 * data_generator.cpp
 * Dataset generator that creates a CSV file with random number + random string pairs.
 *
 * - User specifies dataset size.
 * - Uses mt19937_64 seeded with 2431324789 for reproducibility.
 * - Output: dataset_<size>.csv, each row formatted as "number,string".
 */

#include <iostream>
#include <fstream>
#include <random>
#include <string>

using namespace std;

int main() {
    // ---- 1. Get dataset size from user ----
    cout << "Enter the number of records to generate: ";
    size_t size = 0;
    cin >> size;
    if (size == 0) {
        cerr << "Size must be greater than 0." << endl;
        return 1;
    }

    // ---- 2. Seed the RNG with the fixed seed ----
    const unsigned long long SEED = 2431324789ULL;
    mt19937_64 rng(SEED);

    // ---- 3. Prepare distributions ----
    // 10-digit numbers [1000000000, 9999999999]
    uniform_int_distribution<unsigned long long> num_dist(1000000000ULL, 9999999999ULL);

    // 5 lowercase letters a-z
    uniform_int_distribution<int> char_dist(0, 25);

    // ---- 4. Build output filename ----
    string filename = "dataset_" + to_string(size) + ".csv";
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return 1;
    }

    cout << "Generating " << size << " records into " << filename << " ..." << endl;

    // ---- 5. Generate and write data ----
    for (size_t i = 0; i < size; ++i) {
        // Random 10-digit number
        unsigned long long num = num_dist(rng);

        // Random 5-character string (a-z)
        string str(5, 'a');
        for (int j = 0; j < 5; ++j) {
            str[j] = static_cast<char>('a' + char_dist(rng));
        }

        outfile << num << "," << str << "\n";
    }

    outfile.close();
    cout << "Done. File saved as " << filename << endl;
    return 0;
}
