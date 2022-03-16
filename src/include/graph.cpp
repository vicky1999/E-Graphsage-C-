#include <iostream>
#include <fstream>
#include <vector>
#include <any>
#include <map>

#include <arrow/api.h>

#include "graph.hpp"

using namespace std;

void Graph::createGraph(std::shared_ptr<arrow::Table> node_table, std::shared_ptr<arrow::Table> ip_table)
{
    cout << "Created Graph1!!" << endl;

    vector<map<string, int>> v;
    map<string, int> m;
    m["test"] = 0;
    m["test1"] = 1;
    v.push_back(m);

    cout << v[0]["test1"] << endl;

    // map<string, std::any> mp;
    // mp["text"] = string("test");
    // std::any val = mp["text"];
    // auto x = std::any_cast<std::string &&>(std::move(val));
    // cout << x << endl;

    // std::shared_ptr<arrow::Array> row_data = node_table->column(0)->chunk(0);
    // auto arrow_data = std::static_pointer_cast<arrow::StringArray>(row_data);
    // const uint8_t *str_data = arrow_data->raw_data();
    // for (int64_t i = 0; i < 5; ++i)
    // {
    //     printf("%s", str_data);
    // }
}