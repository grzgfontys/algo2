#include "kernelization.h"
#include "graph.h"
#include <ranges>
#include <algorithm>
#include <stdexcept>

namespace views = std::views;
namespace ranges = std::ranges;

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

void add_pendants(Graph& graph, unsigned int count) {
    auto is_pendant = pendant_checker(graph);
    auto is_not_pendant = [&](int v) { return !is_pendant(v); };

    auto non_pendant_vertices = graph.vertices() | views::filter(is_not_pendant);

    for (auto vertex: non_pendant_vertices) {
        if (count == 0) {
            break;
        }
        if (graph.degree(vertex) > 1) {
            auto adjacent = graph.adjacent(vertex);
            auto is_removable = [&](int v) { return graph.degree(v) > 2; };
            if (!ranges::all_of(adjacent.begin(), adjacent.end(), is_removable)) {
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

void remove_pendants(Graph& graph, unsigned int count) {
    auto is_pendant = pendant_checker(graph);

    for (auto vertex: graph.vertices() | views::filter(is_pendant)) {
        if (count == 0) {
            break;
        }
        auto adjacent = graph.adjacent(vertex).front();
        if (is_pendant(adjacent)) {
            if (count >= 2) {
                graph.remove_edge({vertex, adjacent}); // make vertex disconnected
                count -= 2;
            }
        } else if (graph.degree(adjacent) > 2) {
            graph.remove_edge({vertex, adjacent}); // make vertex disconnected
            count--;
        }
    }

    if (count > 0) {
        throw std::runtime_error("Cannot remove any more pendants");
    }
}

void add_tops(Graph& graph, int edge_count, unsigned int count) {
    auto is_tops = tops_checker(graph, edge_count);
    auto is_not_tops = [&](int v) { return !is_tops(v); };
    auto is_not_almost_tops = [&](int v) -> bool { return graph.degree(v) < edge_count; };

    auto vertices = graph.vertices();
    for (auto vertex: vertices | views::filter(is_not_tops)) {
        if (count == 0) {
            break;
        }
        auto degree = graph.degree(vertex);

        auto available_neighbors = vertices | views::filter(is_not_almost_tops)
                | views::filter([&](int v) { return !graph.are_connected({vertex, v}); });

        std::vector<int> available_neighbors_vec = {available_neighbors.begin(), available_neighbors.end()};

        auto to_add = edge_count - degree + 1;
        if (available_neighbors_vec.size() < to_add) {
            throw std::runtime_error("Cannot add any more tops vertices");
        }
        for (int new_neighbor: available_neighbors_vec | ranges::views::take(to_add)) {
            graph.add_edge({vertex, new_neighbor});
        }
        count--;
    }

    if (count > 0) {
        throw std::runtime_error("Cannot add any more tops vertices");
    }
}

void remove_tops(Graph& graph, int edge_count, unsigned int count) {
    auto is_tops = tops_checker(graph, edge_count);
    auto safe_to_disconnect = [&](int v) -> bool { return graph.degree(v) != edge_count + 1; };

    for (auto vertex: graph.vertices() | views::filter(is_tops)) {
        if (count == 0) {
            break;
        }

        auto removable = graph.adjacent(vertex) | views::filter(safe_to_disconnect);
        std::vector<int> removable_vec = {removable.begin(), removable.end()};

        auto to_remove = graph.degree(vertex) - edge_count;
        if (removable_vec.size() < to_remove) {
            continue;
        }
        for (int adjacent: removable_vec | ranges::views::take(to_remove)) {
            graph.remove_edge({vertex, adjacent});
        }
        count--;
    }

    if (count > 0){
        throw std::runtime_error("Cannot remove any more tops vertices");
    }
}
