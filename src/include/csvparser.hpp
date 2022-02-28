#ifndef CSVPARSER_HPP
#define CSVPARSER_HPP

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<math.h>

#include "datamodel.hpp"

using namespace std;

class CSVParser {
    public:
        CSVParser();
        vector<DataModel> readFile(string file);
        void printData(vector<DataModel> data, int size = 5);
};

#endif