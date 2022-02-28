#include "include/csvparser.hpp"
#include "include/csv.hpp"
#include "include/datamodel.hpp"

#include<vector>

CSVParser::CSVParser() { }

void CSVParser::printData(vector<DataModel> data, int size) {
    size = (data.size() > size) ? size : data.size();
    cout<<"Source Address | Destination Address | "<<endl;
    for(int i=0;i<size;i++) {
        cout<<data[i].sourceIP<<':'<<data[i].sourcePort<<" | ";
        cout<<data[i].destinationIP<<':'<<data[i].destinationPort<<" | ";


        cout<<endl;
    }
}

vector<DataModel> CSVParser::readFile(string filename) {
    cout<<"Reading file: "<<filename<<endl;

    csv::Document doc(filename);

    vector<string> cols = doc.GetColumnNames();
    vector<DataModel> contents;

    int row_count = doc.GetRowCount();
    for(int i=0;i<row_count;i++) {
        vector<string> trow = doc.GetRow<string>(i);
        DataModel dm;
        dm.createDataModel(&dm, trow);
        contents.push_back(dm);
    }
    return contents;
}
