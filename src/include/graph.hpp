#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <set>
#include <any>
#include <torch/torch.h>
#include <torch/csrc/autograd/variable.h>
#include <torch/csrc/autograd/function.h>

using namespace std;

namespace Graph
{
    static set<string> nodes;
    set<string> getNodes();
    map<string, map<string, vector<torch::Tensor>>> createGraph(
        vector<map<string, std::any>> node_data,
        map<string, string> node_schema,
        vector<map<string, std::any>> ip_data,
        map<string, string> ip_schema);
}

#endif