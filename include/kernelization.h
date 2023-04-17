#ifndef ALGORITHMS_2_KERNELIZATION_H
#define ALGORITHMS_2_KERNELIZATION_H

#include <vector>
class Graph;

std::vector<int> isolated_vertices(const Graph& graph);

std::vector<int> pendant_vertices(const Graph& graph);

std::vector<int> tops_vertices(const Graph& graph, int edge_count);

#endif //ALGORITHMS_2_KERNELIZATION_H
