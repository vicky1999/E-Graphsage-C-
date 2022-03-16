// Built-in Headers
#include <iostream>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <filesystem>

// #include <boost/graph/graph_traits.hpp>

// User defined Headers
#include "include/printer.hpp"
#include "include/featherparser.hpp"
#include "include/graph.hpp"
#include "include/model.hpp"

#include <arrow/api.h>

using namespace std;

int main(int argc, char **argv)
{
    print_success("E-GRAPHSAGE");

    /*
        Arguments:
            1. -i ====> Input csv file
                -i flag is used to provide the input csv file
                Ex:
                    ./EGraphSage -i input_file.ftr -n node_file.ftr
    */

    string filename = "";

    char ch;
    std::shared_ptr<arrow::Table> node_table, ip_table;

    while ((ch = getopt(argc, argv, "i:n:")) != -1)
    {
        switch (ch)
        {
        case 'i':
            try
            {
                filename = optarg;
                char buff[1000];
                getcwd(buff, 1000);
                string path(buff);
                string str = path.substr(0, path.find_last_of("/\\"));
                string file = "file://" + str + "/" + filename;
                ip_table = FeatherParser::readFile(file);
                print_success("File Read completed Successfully");
                print_success("No. of Rows: " + std::to_string(ip_table->num_rows()));
                print_success("No. of columns: " + std::to_string(ip_table->num_columns()));

                break;
            }
            catch (...)
            {
                print_error("Error in Reading file!");
            }
        case 'n':
            try
            {
                filename = optarg;
                char buff[1000];
                getcwd(buff, 1000);
                string path(buff);
                string str = path.substr(0, path.find_last_of("/\\"));
                string file = "file://" + str + "/" + filename;
                node_table = FeatherParser::readFile(file);
                print_success("File Read completed Successfully");
                print_success("No. of Rows: " + std::to_string(node_table->num_rows()));
                print_success("No. of columns: " + std::to_string(node_table->num_columns()));
                break;
            }
            catch (...)
            {
                print_error("Error in reading node file!");
            }
        }
    }

    if (!node_table || !ip_table)
    {
        print_error("Node or ip table missing");
        return 0;
    }
    // Time to create a graph!!!
    // Graph::createGraph(node_table, ip_table);
    // EGraphSAGE::Model model = EGraphSAGE::Model();
    // EGraphSAGE::MLPPredictor();

    return 0;
}