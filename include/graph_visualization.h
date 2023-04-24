#ifndef ALGORITHMS_2_GRAPH_VISUALIZATION_H
#define ALGORITHMS_2_GRAPH_VISUALIZATION_H

#include "graph.h"
#include <string>

enum class GraphOutputType
{
    BMP,
    PNG,
    JPEG,
    SVG,
    PDF
};

void visualise_graph(const Graph& graph, GraphOutputType format);

std::string adjacency_matrix_to_string(const Graph& graph);

void print_adjacency_matrix(const Graph& graph);

#endif //ALGORITHMS_2_GRAPH_VISUALIZATION_H
