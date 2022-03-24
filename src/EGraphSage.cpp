// Built-in Headers
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <string.h>
#include <any>

#include <torch/torch.h>

// User defined Headers
#include "include/printer.hpp"
#include "include/csvparser.hpp"
#include "include/commons.h"
#include "include/graph.hpp"
#include "include/model.hpp"

using namespace std;

char buff[1000];

string getBasePath()
{
    getcwd(buff, 1000);
    string path(buff);
    string basepath = path.substr(0, path.find_last_of("/\\"));
    return basepath;
}

void printSchema(map<string, string> schema)
{
    for (auto x : schema)
    {
        cout << x.first << " => " << x.second << endl;
    }
}

void createSchema(string filename, bool isnode)
{
    string basepath = getBasePath();
    map<string, string> schema;
    if (isnode)
    {
        schema = CSVParser::readSchema(filename, basepath, isnode);
        Commons::node_schema = schema;
        // printSchema(Schema::node_schema);
    }
    else
    {
        schema = CSVParser::readSchema(filename, basepath, isnode);
        Commons::ip_schema = schema;
    }
}

void convertToCSV(string ip_filename, string node_filename)
{
    string basepath = getBasePath();
    string command_string = "python3 " + basepath + "/python/featherparser.py -i " + ip_filename + " -n " + node_filename;
    const char* command = command_string.c_str();
    system(command);
}

void parseCSV(string filename, map<string, string> schema, bool isnode)
{
    string basepath = getBasePath();
    vector<map<string, any>> res = CSVParser::readFile(filename, basepath, schema, isnode);
    if (isnode)
    {
        Commons::node_data = res;
    }
    else
    {
        Commons::ip_data = res;
    }
}

int main(int argc, char** argv)
{
    print_success("E-GRAPHSAGE");

    /*
        Arguments:
            1. -i ====> Input csv file
                -i flag is used to provide the input csv file
                Ex:
                    ./EGraphSage -i input_file.ftr -n node_file.ftr
    */

    string node_filename = "", ip_filename = "";
    string node_schema_name = "", ip_schema_name = "";
    string label = "";
    char buff[1000];
    char ch;
    int epochs = 100;

    while ((ch = getopt(argc, argv, "i:n:v:e:l:s:")) != -1)
    {
        switch (ch)
        {
        case 'i':
            ip_filename = optarg;
            break;
        case 'n':
            node_filename = optarg;
            break;
        case 'v':
            node_schema_name = optarg;
            break;
        case 'e':
            ip_schema_name = optarg;
            break;
        case 'l':
            label = optarg;
            break;
        case 's':
            epochs = stoi(optarg);
            break;

        }

    }

    if (node_filename == "" || ip_filename == "" || node_schema_name == "" || ip_schema_name == "")
    {
        print_error("Node file, ip file, node schema and ip schema are required!");
        return 0;
    }

    // Call python script to create csv data from feather file.
    convertToCSV(ip_filename, node_filename);

    // get csv filename.
    char* node_basename = strtok(const_cast<char*>(node_filename.c_str()), ".");
    char* ip_basename = strtok(const_cast<char*>(ip_filename.c_str()), ".");
    string node_csv = "data/" + string(node_basename) + ".csv";
    string ip_csv = "data/" + string(ip_basename) + ".csv";

    createSchema(node_schema_name, true);
    createSchema(ip_schema_name, false);

    // Get Columns
    vector<string> node_columns = CSVParser::getColumns(node_csv, getBasePath());
    vector<string> ip_columns = CSVParser::getColumns(ip_csv, getBasePath());

    // Parse csv data
    parseCSV(node_csv, Commons::node_schema, true);
    parseCSV(ip_csv, Commons::ip_schema, false);

    auto it = std::find(node_columns.begin(), node_columns.end(), label);
    if (it == node_columns.end()) {
        print_error("Please enter a valid Label Column");
        return 0;
    }
    int label_index = it - node_columns.begin();
    cout << "Label Index: " << label_index << endl;

    cout << "IP data size: " << Commons::ip_data.size() << endl;
    cout << "Node data size: " << Commons::node_data.size() << endl;

    Graph::GraphModel graph = Graph::createGraph(
        Commons::node_data, Commons::node_schema, Commons::ip_data, Commons::ip_schema, node_columns, ip_columns, label_index);

    // set<string> nodes = Graph::getNodes();
    std::shared_ptr<Model::EGraphSage> model = Model::createModel(graph, Commons::node_data.size(), Commons::node_schema.size(), epochs);

    return 0;
}