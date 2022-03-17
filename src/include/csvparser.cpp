#include <iostream>
#include <stdlib.h>
#include <vector>
#include <map>
#include <any>
#include <string.h>
#include <fstream>
#include <sstream>

// User defined Header files
#include "csvparser.hpp"
// #include "schema.hpp"
#include "commons.h"

using namespace std;

std::any getDataWithType(string value, string column, string type)
{
    any res;
    if (type == "int64")
        res = atoi(value.c_str());
    else if (type == "string")
        res = value;
    return res;
}

vector<map<string, any>> CSVParser::readFile(string filename, string basepath, map<string, string> schema, bool isnode)
{
    vector<map<string, any>> res;
    string filepath = basepath + "/" + filename;
    fstream fs;
    fs.open(filepath, fstream::in);
    string line, word, temp;
    int col = 0;
    std::any value;
    vector<string> columns;
    bool isHeader = true;
    cout << "Schema size: " << schema.size() << endl;

    while (fs >> temp)
    {
        stringstream s(temp);
        col = 0;
        map<string, any> row;
        while (getline(s, word, ','))
        {
            if (isHeader)
            {
                columns.push_back(word);
            }
            else
            {
                std::any value = getDataWithType(word, columns[col], schema[columns[col]]);
                row[columns[col]] = value;
            }
            col++;
        }
        if (!isHeader)
        {
            res.push_back(row);
        }
        isHeader = false;
    }

    return res;
}

map<string, string> CSVParser::readSchema(string filename, string basepath, bool isnode)
{
    map<string, string> schema;
    string filepath = basepath + "/" + filename;
    fstream fs;
    fs.open(filepath, fstream::in);
    string line, word, temp;
    int cnt = 0;
    string key, value;
    while (fs >> temp)
    {
        stringstream s(temp);
        cnt = 0;
        key = "", value = "";
        while (getline(s, word, ','))
        {
            if (!cnt)
            {
                key = word;
            }
            else
            {
                value = word;
            }
            cnt++;
        }
        schema[key] = value;
    }

    return schema;
}