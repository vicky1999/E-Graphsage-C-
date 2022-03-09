// Built-in Headers
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;

// User defined Headers
#include "include/printer.hpp"
#include "include/featherparser.hpp"

#include <arrow/api.h>

int main(int argc, char **argv)
{
    print_success("E-GRAPHSAGE");

    /*
        Arguments:
            1. -i ====> Input csv file
                -i flag is used to provide the input csv file
                Ex:
                    ./EGraphSage -i input.csv
    */

    string filename = "";

    char ch;

    while ((ch = getopt(argc, argv, "i:")) != EOF)
    {
        switch (ch)
        {
        case 'i':
            try
            {
                filename = optarg;
                std::shared_ptr<arrow::Table> table = FeatherParser::readFile(filename);
                print_success("File Read completed Successfully");
                print_success("No. of Rows: " + std::to_string(table->num_rows()));
                print_success("No. of columns: " + std::to_string(table->num_columns()));
                break;
            }
            catch (...)
            {
                print_error("Error in Reading file!");
            }
        }
    }

    if (filename == "")
    {
        print_error("File name is required!");
    }

    return 0;
}