#include "kernelization.h"
#include "graph.h"
#include <ranges>

namespace views = std::views;

auto isolation_checker(const Graph& graph) {
    return [&](int vertex) { return graph.degree(vertex) == 0; };
}

auto pendant_checker(const Graph& graph) {
    return [&](int vertex) { return graph.degree(vertex) == 1; };
}

auto tops_checker(const Graph& graph, int edge_count) {
    return [&graph, edge_count](int vertex) { return graph.degree(vertex) > edge_count; };
}

std::vector<int> isolated_vertices(const Graph& graph) {
    auto isolated_vertices = graph.vertices() | views::filter(isolation_checker(graph));
    return {isolated_vertices.begin(), isolated_vertices.end()};
}

std::vector<int> pendant_vertices(const Graph& graph) {
    auto pendant_vertices = graph.vertices() | views::filter(pendant_checker(graph));
    return {pendant_vertices.begin(), pendant_vertices.end()};
}

std::vector<int> tops_vertices(const Graph& graph, int edge_count) {
    auto tops_vertices = graph.vertices() | views::filter(tops_checker(graph, edge_count));
    return {tops_vertices.begin(), tops_vertices.end()};
}