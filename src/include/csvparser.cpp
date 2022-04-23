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
#include "scaler.hpp"
#include "commons.h"

using namespace std;

std::any getDataWithType(string value, string column, string type)
{
    any res;
    if (type == "string")
        res = string(value);
    else if (type == "int64" && (column == "srcport" || column == "dstport"))
        res = stoi(value);
    else
        res = stod(value);
    return res;
}

vector<string> CSVParser::getColumns(string filename, string basepath) {
    vector<string> columns;

    string filepath = basepath + "/" + filename;
    fstream fs;
    fs.open(filepath, fstream::in);
    string line, word, temp;

    while (fs >> temp)
    {
        stringstream s(temp);
        while (getline(s, word, ','))
        {
            columns.push_back(word);
        }
        break;
    }
    return columns;
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
    // cout << "Schema size: " << schema.size() << endl;

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
                // if (col == 0) {
                //     cout << "Word: " << word << endl;
                // }
                std::any value = getDataWithType(word, columns[col], schema[columns[col]]);
                row[columns[col]] = value;
            }
            col++;
        }
        if (!isHeader)
        {
            res.push_back(row);
            // cout << "Row Size: " << row.size() << endl;
        }
        isHeader = false;
    }
    if (isnode) {
        res = StandardScaler::fit(res, schema, CSVParser::getColumns(filename, basepath));
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