#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <arrow/api.h>

using namespace std;

namespace Graph
{
    void createGraph(std::shared_ptr<arrow::Table> node_table, std::shared_ptr<arrow::Table> ip_table);
}

#endif