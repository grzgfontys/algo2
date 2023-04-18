#include "kernelization.h"
#include "graph.h"
#include <ranges>
#include <stdexcept>

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

void add_pendants(Graph& graph, int count) {
    auto is_pendant = pendant_checker(graph);
    auto is_not_pendant = [&](int v) { return !is_pendant(v); };

    auto non_pendant_vertices = graph.vertices() | views::filter(is_not_pendant);

    for (auto vertex: non_pendant_vertices) {
        if (count == 0) {
            break;
        }
        if (graph.degree(vertex) > 1) {
            auto adjacent = graph.adjacent(vertex);
            if (!std::ranges::all_of(adjacent.begin(), adjacent.end(), is_not_pendant)) {
                continue;
            }
            for (int adjacent_vertex: adjacent | views::drop(1)) {
                graph.remove_edge({vertex, adjacent_vertex});
            }
            count--;
        }
    }
    if (count > 0) {
        throw std::runtime_error("Cannot add any more pendants");
    }
}
