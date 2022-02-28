// Built-in Headers
#include<iostream>
#include<unistd.h>
#include<vector>

using namespace std;

// User defined Headers
#include "include/csvparser.hpp"
#include "include/printer.hpp"


int main(int argc, char **argv) {
    print_success("E-GRAPHSAGE");

    /*
        Arguments: 
            1. -i ====> Input csv file
                -i flag is used to provide the input csv file
                Ex: 
                    ./EGraphSage -i input.csv
    */

    string filename = "";
    CSVParser *parser = new CSVParser();
    vector<DataModel> data;

    char ch;

    while((ch = getopt(argc, argv, "i:")) != EOF) {
        switch(ch) {
            case 'i':
                try {
                    filename = optarg;
                    data = parser->readFile(filename);
                    print_success("File Read completed Successfully");
                    parser->printData(data, 5);
                    break;
                }
                catch(...) {
                    print_error("Error in Reading file!");
                }

        }
    }

    if(filename == "") {
        print_error("File name is required!");
    }

    return 0;    
}