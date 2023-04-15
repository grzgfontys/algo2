#ifndef ALGORITHMS_2_VERTEX_COVER_H
#define ALGORITHMS_2_VERTEX_COVER_H

#include <optional>
#include <unordered_set>
#include "graph.h"

std::optional<std::unordered_set<int>> vertex_cover_of_size(const Graph& graph, unsigned int vertex_cover_size);

#endif //ALGORITHMS_2_VERTEX_COVER_H
