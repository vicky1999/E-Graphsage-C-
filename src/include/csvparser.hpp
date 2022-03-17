#include <iostream>

using namespace std;
namespace CSVParser
{
    vector<map<string, any>> readFile(string filename, string basepath, map<string, string> schema, bool isnode);
    map<string, string> readSchema(string filename, string basepath, bool isnode);
}