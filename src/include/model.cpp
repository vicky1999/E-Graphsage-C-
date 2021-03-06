#include <iostream>
#include <set>
#include <string>
#include <stdexcept>

#include <torch/torch.h>
#include <torch/csrc/autograd/variable.h>
#include <torch/csrc/autograd/function.h>

#include "model.hpp"
#include "graph.hpp"

using namespace std;

Model::EGraphSage::EGraphSage(int ndim_in, int ndim_out, int edim, double dropout)
{
    this->gnn = std::make_shared<Model::SAGE>(ndim_in, ndim_out, edim, dropout);
    this->pred = std::make_shared<Model::MLPredictor>(ndim_out, 2);
}

torch::Tensor Model::EGraphSage::forward(Graph::GraphModel graph, torch::Tensor nfeats, torch::Tensor efeats)
{
    torch::Tensor h = this->gnn->forward(graph, nfeats, efeats);
    return this->pred->forward(graph, h);
}

struct SAGELayer : torch::nn::Module {
    torch::nn::Linear W_apply{ nullptr };
    SAGELayer(int ndim_in, int edim, int ndim_out) {
        W_apply = torch::nn::Linear(ndim_in + edim, ndim_out);
    }

    torch::Tensor forward(Graph::GraphModel graph, torch::Tensor nfeats, torch::Tensor efeats) {
        Graph::GraphModel g = graph;
        g.ndata["h"] = nfeats;
        g.edata["h"] = efeats;
        g.ndata["h_neigh"] = Graph::getMeanForEdges(g.graph);
        g.ndata["h"] = torch::relu(W_apply->forward(torch::cat({ g.ndata["h"], g.ndata["h_neigh"] }, 2)));
        return g.ndata["h"];
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
    // cout << "Layers size: " << this->layers->size() << endl;;
}

torch::Tensor Model::SAGE::forward(Graph::GraphModel graph, torch::Tensor nfeats, torch::Tensor efeats) {
    // cout << "Sage Forward!!" << endl;
    for (int i = 0;i < this->layers->size();i++) {
        if (i != 0) {
            nfeats = this->dropout(nfeats);
        }
        nfeats = this->layers->at<SAGELayer>(i).forward(graph, nfeats, efeats);
    }
    return nfeats.sum(1);
}

Model::MLPredictor::MLPredictor(int in_features, int out_classes) {
    this->W = torch::nn::Linear(in_features * 2, out_classes);
    // cout << "ML Predictor!!!" << endl;
}

torch::Tensor Model::MLPredictor::forward(Graph::GraphModel graph, torch::Tensor h) {
    // cout << "ML Predictor forward!!!" << endl;
    vector<torch::Tensor> src_maps, dest_maps;
    Graph::GraphModel g = graph;
    g.ndata["h"] = h;

    map<string, int> ip_map = Graph::getSourceIndices(graph.graph);
    for (auto src : graph.graph) {
        map<string, vector<vector<double>>> val = src.second;
        for (auto dst : val) {
            int dest_size = dst.second.size();
            for (int i = 0;i < dest_size;i++) {
                int src_ind = ip_map[src.first], dst_ind = ip_map[dst.first];
                auto src_val = h[src_ind];
                auto dst_val = h[dst_ind];
                src_maps.push_back(src_val);
                dest_maps.push_back(dst_val);
            }
        }
    }

    auto options = torch::TensorOptions(); // .device(torch::kCUDA, 1);
    torch::Tensor src, dest;
    int src_size = src_maps.size(), dst_size = dest_maps.size(), h_size = torch::_shape_as_tensor(h)[1].item<int>();
    src = torch::from_blob(src_maps.data(), { src_size, h_size }, options);
    dest = torch::from_blob(dest_maps.data(), { dst_size, h_size }, options);

    torch::Tensor combined = torch::cat({ src, dest }, 1);  // not used since it is giving segmentation error!!!


    auto score = this->W->forward(combined);
    graph.edata["score"] = score;
    return graph.edata["score"];
}

float computeAccuracy(torch::Tensor pred, torch::Tensor labels) {
    auto vals = pred.argmax(1);
    auto x = (vals == labels).to(torch::kFloat);
    float mean = x.mean().item<float>();
    return mean;
}

std::shared_ptr<Model::EGraphSage> Model::createModel(Graph::GraphModel graph, int node_size, int edge_size, int epochs) {
    vector<int> sizes = Graph::getGraphNM(graph.graph);
    int n = sizes[0], m = sizes[1];
    graph.ndata["h"] = torch::ones({ n, 1, m });
    graph.edata["train_mask"] = torch::ones({ n });
    auto model = std::make_shared<Model::EGraphSage>(m, 128, m, 0.2);

    torch::Tensor node_features = graph.ndata["h"];
    torch::Tensor edge_features = graph.edata["h"];
    torch::Tensor edge_label = graph.edata["Label"];
    torch::Tensor train_mask = graph.edata["train_mask"];

    torch::optim::Adam opt(model->parameters());

    torch::nn::CrossEntropyLoss criterion = torch::nn::CrossEntropyLoss(torch::nn::CrossEntropyLossOptions().weight(torch::ones({ 2 })).reduction(torch::kMean));

    for (int epoch = 1;epoch <= epochs;epoch++) {
        opt.zero_grad();
        try {
            auto pred = model->forward(graph, node_features, edge_features);
            // cout << "Label: " << edge_label << endl;
            auto loss = criterion(pred, edge_label);
            loss.backward();
            opt.step();
            cout << "Epoch: " << epoch << "/" << epochs << " =====> ACC: " << computeAccuracy(pred, edge_label) << endl;
        }
        catch (const exception& ex) {
            cout << "Error occurred!!!" << endl;
            continue;
        }
    }


    // torch::Tensor pred = model->forward(graph, node_features, edge_features);
    // auto predicted = pred.argmax(1);
    return model;
}
