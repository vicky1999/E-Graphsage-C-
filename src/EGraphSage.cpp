// Built-in Headers
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <string.h>

// User defined Headers
#include "include/printer.hpp"

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

    string node_filename = "", ip_filename = "";
    string node_schema = "", ip_schema = "";
    char buff[1000];
    char ch;

    while ((ch = getopt(argc, argv, "i:n:v:e:")) != -1)
    {
        switch (ch)
        {
        case 'i':
            node_filename = optarg;
            break;
        case 'n':
            ip_filename = optarg;
            break;
        case 'v':
            node_schema = optarg;
            break;
        case 'e':
            ip_schema = optarg;
            break;
        }
    }

    if (node_filename == "" || ip_filename == "" || node_schema == "" || ip_schema == "")
    {
        print_error("Node file, ip file, node schema and ip schema are required!");
        return 0;
    }

    getcwd(buff, 1000);
    string path(buff);
    string basepath = path.substr(0, path.find_last_of("/\\"));
    string command_string = "python3 " + basepath + "/python/featherparser.py -i " + ip_filename + " -n " + node_filename;
    const char *command = command_string.c_str();
    system(command);

    return 0;
}