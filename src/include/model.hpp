#ifndef MODEL_HPP
#define MODEL_HPP

#include <iostream>
#include <set>
#include <string>
#include <map>
#include <torch/torch.h>
#include <torch/csrc/autograd/variable.h>
#include <torch/csrc/autograd/function.h>

#include "graph.hpp"

using namespace std;
namespace Model
{
    static set<string> nodes;
    static map<string, torch::Tensor> ndata, edata;

    // class SageLayer : torch::nn::Module
    // {
    // public:
    //     torch::nn::Linear W_apply{nullptr};
    //     SageLayer(int ndim_in, int edim, int ndim_out);
    //     torch::Tensor forward(torch::Tensor x);
    // };

    class SAGE : torch::nn::Module
    {
    public:
        torch::nn::ModuleList layers;
        torch::nn::Dropout dropout;
        SAGE(int ndim_in, int ndim_out, int edim, double dropout);
        torch::Tensor forward(Graph::GraphModel graph, torch::Tensor nfeats, torch::Tensor efeats);
    };

    class MLPredictor : torch::nn::Module {
    public:
        torch::nn::Linear W{ nullptr };
        MLPredictor(int in_features, int out_features);
        torch::Tensor forward(Graph::GraphModel graph, torch::Tensor h);
    };

    class EGraphSage : torch::nn::Module
    {
    public:
        std::shared_ptr<SAGE> gnn;
        std::shared_ptr<MLPredictor> pred;

        EGraphSage(int ndim_in, int ndim_out, int edim, double dropout);
        torch::Tensor forward(Graph::GraphModel graph, torch::Tensor nfeats, torch::Tensor efeats);
    };

    std::shared_ptr<EGraphSage> createModel(Graph::GraphModel graph, int node_size, int edge_size);
}

#endif