#include<iostream>
#include<vector>
#include<map>
#include<any>

using namespace std;
namespace StandardScaler {
    vector<map<string, any>> fit(vector<map<string, any>> data, map<string, string> schema, vector<string> columns);
    vector<double> getColumnMeans(vector<map<string, any>> data, map<string, string> schema, vector<string> columns);
    vector<double> getSTD(vector<map<string, any>> data, map<string, string> schema, vector<string> columns, vector<double> means);
}