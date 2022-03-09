#ifndef FEATHERPARSER_HPP
#define FEATHERPARSER_HPP

#include <iostream>
#include <vector>
#include <arrow/api.h>

using namespace std;

namespace FeatherParser
{
        void FeatherParser();
        std::shared_ptr<arrow::Table> readFile(string filename);

};

#endif