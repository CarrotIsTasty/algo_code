#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


struct Record {
    unsigned long long number;
    std::string str;

    Record(unsigned long long num, const std::string& s) : number(num), str(s) {}
};

class fileOperations{
    public:
        fileOperations();
        ~fileOperations();
        bool loadFile(const std::string& filename, std::vector<Record>& data);
        bool saveFile(const std::string& filename, const std::vector<Record>& data);
};

#endif