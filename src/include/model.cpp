#include <iostream>
#include <set>
#include <string>

#include <torch/torch.h>
#include <torch/csrc/autograd/variable.h>
#include <torch/csrc/autograd/function.h>

#include "model.hpp"
#include "graph.hpp"

using namespace std;

Model::EGraphSage::EGraphSage(int ndim_in, int ndim_out, int edim, double dropout)
{
    // this->nodes = nodes;
    cout << "E-GraphSage model creation!!!" << Model::nodes.size() << endl;
    this->gnn = std::make_shared<Model::SAGE>(ndim_in, ndim_out, edim, dropout);
    this->pred = std::make_shared<Model::MLPredictor>(ndim_out, 2);
}

torch::Tensor Model::EGraphSage::forward(map<string, map<string, vector<torch::Tensor>>> graph, torch::Tensor nfeats, torch::Tensor efeats)
{
    cout << "Model forward!!!" << endl;
    torch::Tensor h = this->gnn->forward(graph, nfeats, efeats);
    return this->pred->forward(graph, h);
}

struct SAGELayer : torch::nn::Module {
    torch::nn::Linear W_apply{ nullptr };
    SAGELayer(int ndim_in, int edim, int ndim_out) {
        W_apply = torch::nn::Linear(ndim_in + edim, ndim_out);
    }

    torch::Tensor forward(map<string, map<string, vector<torch::Tensor>>> graph, torch::Tensor nfeats, torch::Tensor efeats) {
        return nfeats;
    }
};

Model::SAGE::SAGE(int ndim_in, int ndim_out, int edim, double dropout) {
    vector<std::shared_ptr<SAGELayer>> modules = {
        std::make_shared<SAGELayer>(ndim_in, edim, 128),
        std::make_shared<SAGELayer>(128, edim, ndim_out)
    };

    for (auto& module : modules) {
        this->layers->push_back(module);
    }
    this->dropout = torch::nn::Dropout(dropout);
    cout << "Layers size: " << this->layers->size() << endl;;
}

torch::Tensor Model::SAGE::forward(map<string, map<string, vector<torch::Tensor>>> graph, torch::Tensor nfeats, torch::Tensor efeats) {
    cout << "Sage Forward!!" << endl;
    return torch::ones({ 1,3 });
}

Model::MLPredictor::MLPredictor(int in_features, int out_classes) {
    this->W = torch::nn::Linear(in_features * 2, out_classes);
    cout << "ML Predictor!!!" << endl;
}

torch::Tensor Model::MLPredictor::forward(map<string, map<string, vector<torch::Tensor>>> graph, torch::Tensor h) {
    cout << "ML Predictor forward!!!" << endl;
    return h;
}

std::shared_ptr<Model::EGraphSage> Model::createModel(set<string> nodes, int node_size, int edge_size) {
    Model::nodes = nodes;
    int num_nodes = nodes.size();
    Model::ndata["h"] = torch::ones({ num_nodes, edge_size });
    Model::edata["train_mask"] = torch::ones({ node_size * 2 });
    auto model = std::make_shared<Model::EGraphSage>(edge_size, 128, edge_size, 0.2);
    return model;
}