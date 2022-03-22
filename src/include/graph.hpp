#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <set>
#include <any>
#include <unordered_map>
#include <torch/torch.h>
#include <torch/csrc/autograd/variable.h>
#include <torch/csrc/autograd/function.h>

#define GTYPE unordered_map<string, map<string, vector<vector<double>>>>

using namespace std;

namespace Graph
{
    struct GraphModel {
        map<string, torch::Tensor> ndata;
        map<string, torch::Tensor> edata;
        GTYPE graph;
    };

    static set<string> nodes;
    set<string> getNodes();
    GraphModel createGraph(
        vector<map<string, std::any>> node_data,
        map<string, string> node_schema,
        vector<map<string, std::any>> ip_data,
        map<string, string> ip_schema,
        vector<string> node_columns,
        vector<string> ip_columns
    );

    torch::Tensor getMeanForEdges(GTYPE graph);
    vector<int> getGraphNM(GTYPE graph); // to get the size of graph
    map<string, int> getSourceIndices(GTYPE graph);
}

#endif