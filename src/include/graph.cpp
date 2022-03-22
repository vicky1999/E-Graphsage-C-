#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <typeinfo>

#include <torch/torch.h>
#include <torch/csrc/autograd/variable.h>
#include <torch/csrc/autograd/function.h>

#include "graph.hpp"

using namespace std;

string anyToString(any data)
{
    return std::any_cast<std::string&&>(std::move(data));
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

vector<double> anyMapToDoubleVector(map<string, any> data, vector<string> columns)
{
    vector<double> res;
    for (auto col : columns)
    {
        res.push_back(std::any_cast<double>(data[col]));
    }
    return res;  // .data();
}

set<string> Graph::getNodes()
{
    return Graph::nodes;
}

vector<int> Graph::getGraphNM(GTYPE graph) {
    int n, m;

    for (auto src : graph) {
        map<string, vector<vector<double>>> val = src.second;
        for (auto dst : val) {
            n++;
            m = dst.second[0].size(); // get the number of cols...
        }
    }
    vector<int> res = { n,m };
    return res;
}

torch::Tensor Graph::getMeanForEdges(GTYPE graph) {
    vector<int> sizes = getGraphNM(graph);
    int n = sizes[0], m = sizes[1];  // n -> number of rows, m -> number of cols...
    torch::Tensor res = torch::zeros({ n, m });
    auto options = torch::TensorOptions().dtype(torch::kFloat64); // .device(torch::kCUDA, 1);
    int ind = 0;
    for (auto src : graph) {
        map<string, vector<vector<double>>> val = src.second;
        bool canBreak = false;
        for (auto dst : val) {
            int size = dst.second.size();
            if (size <= 0) {
                break;
            }
            int cols = dst.second[0].size();
            torch::Tensor means = torch::zeros({ 1, cols });
            for (int i = 0;i < size; i++) {
                int sz = dst.second[i].size();

                torch::Tensor tmp = torch::from_blob(dst.second[i].data(), { sz }, options);
                means = torch::add(means, tmp);
            }
            res.slice(0, ind, ind + 1) = torch::div(means, size);
            if (size > 1 && !canBreak) {
                canBreak = true;
            }
            ind++;
        }
    }
    return res.view({ n, 1, m });
}

map<string, int> Graph::getSourceIndices(GTYPE graph) {
    // vector<string> ips;
    map<string, int> ip_map;
    int ind = 0;
    for (auto src : graph) {
        map<string, vector<vector<double>>> val = src.second;
        for (auto dst : val) {
            // ips.push_back(src.first);
            // ips.push_back(dst.first);
            if (ip_map.find(src.first) == ip_map.end()) {
                ip_map[src.first] = ind++;
            }
            if (ip_map.find(dst.first) == ip_map.end()) {
                ip_map[dst.first] = ind++;
            }
        }
    }
    return ip_map;
}

Graph::GraphModel Graph::createGraph(
    vector<map<string, std::any>> node_data,
    map<string, string> node_schema,
    vector<map<string, std::any>> ip_data,
    map<string, string> ip_schema,
    vector<string> node_columns,
    vector<string> ip_columns
)
{
    string source_key = "srcip";
    string destination_key = "dstip";

    cout << "Create a graph!!" << endl;
    unordered_map<string, map<string, vector<vector<double>>>> graph;

    auto options = torch::TensorOptions().dtype(torch::kFloat64); // .device(torch::kCUDA, 1);
    int schema_size = node_schema.size();
    int num_el = 0;

    for (int i = 0; i < ip_data.size(); i++)
    {
        // torch::Tensor tensor = torch::from_blob(anyMapToDoubleVector(node_data[i]), { schema_size }, options);
        string src = anyToString(ip_data[i]["srcip"]) + ":" + to_string(anyToInt(ip_data[i]["srcport"]));
        string dest = anyToString(ip_data[i]["dstip"]) + ":" + to_string(anyToInt(ip_data[i]["dstport"]));
        vector<double> vec = anyMapToDoubleVector(node_data[i], node_columns);
        graph[src][dest].push_back(vec);
        graph[dest][src].push_back(vec);

        // add to nodes set...
        Graph::nodes.insert(src);
        Graph::nodes.insert(dest);
        num_el += 2;
    }

    Graph::GraphModel res;
    res.graph = graph;
    int n = 2 * Graph::nodes.size(), m = node_schema.size();
    cout << "sizes: " << n << " - " << m << endl;

    auto tensor = torch::zeros({ num_el, m }, options);

    auto tensorOptions = torch::TensorOptions(); // .device(torch::kCUDA, 1);
    // cout << graph["10.113.0.201:2055"] << endl;
    bool isfirst = true;
    int ind = 0;
    for (auto src : graph) {
        map<string, vector<vector<double>>> val = src.second;
        for (auto dst : val) {
            int size = dst.second.size();
            for (int i = 0;i < size; i++) {
                int sz = dst.second[i].size();
                tensor.slice(0, ind, ind + 1) = torch::from_blob(dst.second[i].data(), { sz }, options);
                ind++;
            }
        }
    }
    torch::Tensor edata_size = torch::_shape_as_tensor(tensor);
    res.edata["h"] = tensor.view({ edata_size[0].item<int>(), 1, edata_size[1].item<int>() });
    return res;
}