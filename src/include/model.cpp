#include <iostream>
#include <torch/torch.h>

#include "model.hpp"

using namespace std;

class EGraphSAGE::MLPPredictor : torch::nn::Module
{
public:
    MLPPredictor()
    {
        cout << "MLPredictor!!!" << endl;
    }
    map<string, double> apply_edges()
    {
        cout << "Apply Edges!!!" << endl;
        map<string, double> score;
        score["score"] = 10.00;
        return score;
    }
};

class EGraphSAGE::SAGE : torch::nn::Module
{
public:
    SAGE()
    {
        cout << "Sage Constructor!!!" << endl;
    }
};

class EGraphSAGE::Model : torch::nn::Module
{
public:
    Model()
    {
        cout << "Model Constructor!!!" << endl;
    }
};