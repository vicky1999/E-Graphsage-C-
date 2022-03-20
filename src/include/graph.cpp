#include <iostream>
#include <map>
#include <vector>
#include <typeinfo>

#include <torch/torch.h>
#include <torch/csrc/autograd/variable.h>
#include <torch/csrc/autograd/function.h>

#include "graph.hpp"

using namespace std;

string anyToString(any data)
{
    return std::any_cast<std::string &&>(std::move(data));
}

int anyToInt(any data)
{
    return std::any_cast<int>(data);
}

map<string, double> anyMapToDoubleMap(map<string, any> data)
{
    map<string, double> res;
    for (auto d : data)
    {
        res[d.first] = std::any_cast<double>(d.second);
    }
    return res;
}

double *anyMapToDoubleVector(map<string, any> data)
{
    vector<double> res;
    for (auto d : data)
    {
        res.push_back(std::any_cast<double>(d.second));
    }
    return res.data();
}

set<string> Graph::getNodes()
{
    return Graph::nodes;
}

map<string, map<string, vector<torch::Tensor>>> Graph::createGraph(
    vector<map<string, std::any>> node_data,
    map<string, string> node_schema,
    vector<map<string, std::any>> ip_data,
    map<string, string> ip_schema)
{
    string source_key = "srcip";
    string destination_key = "dstip";

    cout << "Create a graph!!" << endl;
    map<string, map<string, vector<torch::Tensor>>> graph;

    auto options = torch::TensorOptions().dtype(torch::kFloat64); // .device(torch::kCUDA, 1);
    int schema_size = node_schema.size();

    for (int i = 0; i < ip_data.size(); i++)
    {
        torch::Tensor tensor = torch::from_blob(anyMapToDoubleVector(node_data[i]), {schema_size}, options);
        string src = anyToString(ip_data[i]["srcip"]) + ":" + to_string(anyToInt(ip_data[i]["srcport"]));
        string dest = anyToString(ip_data[i]["dstip"]) + ":" + to_string(anyToInt(ip_data[i]["dstport"]));
        graph[src][dest].push_back(tensor);

        // add to nodes set...
        Graph::nodes.insert(src);
        Graph::nodes.insert(dest);
    }
    cout << "Graph has : " << graph.size() << " Nodes!" << Graph::nodes.size() << endl;
    // for (auto v : graph)
    // {
    //     cout << "src: " << v.first << endl;
    // }

    return graph;
}