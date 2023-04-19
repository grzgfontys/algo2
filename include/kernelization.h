#ifndef ALGORITHMS_2_KERNELIZATION_H
#define ALGORITHMS_2_KERNELIZATION_H

#include <vector>

class Graph;

std::vector<int> isolated_vertices(const Graph& graph);

std::vector<int> pendant_vertices(const Graph& graph);

std::vector<int> tops_vertices(const Graph& graph, int edge_count);

void add_pendants(Graph& graph, unsigned int count);

void remove_pendants(Graph& graph, unsigned int count);

void add_tops(Graph& graph, int edge_count, unsigned int count);

void remove_tops(Graph& graph, int edge_count, unsigned int count);

#endif //ALGORITHMS_2_KERNELIZATION_H
