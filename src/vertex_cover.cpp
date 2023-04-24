#include <span>
#include <cstring>
#include <ranges>
#include <algorithm>
#include "vertex_cover.h"

bool is_valid_vertex_cover(const Graph& graph, const std::set<int>& vertex_cover) {
    return std::ranges::all_of(graph.edges(), [&](const auto& edge) {
        auto [src, dst] = edge;
        return vertex_cover.contains(src) || vertex_cover.contains(dst);
    });
}

std::optional<std::set<int>> try_vertex_cover_naive_impl(const Graph& graph, unsigned int vertex_cover_size) {
    using std::views::iota;
    using std::ranges::views::filter;

    const unsigned int vertex_count = graph.vertex_count();
    const std::span<bool> vertex_mask = std::span(new bool[vertex_count], vertex_count);

    for (int k = 0; k <= vertex_cover_size; k++) {
        memset(vertex_mask.data(), 0, vertex_mask.size_bytes()); // set all to 0
        memset(vertex_mask.data(), 1, k * sizeof(bool)); // set k first to 1
        do {
            // range of all the vertices that are set in the mask
            auto selected_vertices = iota(0u, vertex_count) | filter([&](int x) { return vertex_mask[x]; });

            // convert range to hashset
            const std::set<int> vertex_set(selected_vertices.begin(), selected_vertices.end());

            if (is_valid_vertex_cover(graph, vertex_set)) {
                delete[] vertex_mask.data();
                return vertex_set;
            }
        } while (std::prev_permutation(vertex_mask.begin(), vertex_mask.end()));
    }

    delete[] vertex_mask.data();
    return std::nullopt;
}

std::optional<std::set<int>> vertex_cover_of_size(const Graph& graph, unsigned int vertex_cover_size) {
    auto all_vertices = graph.vertices();
    std::set<int> candidate_vertices{all_vertices.begin(), all_vertices.end()};

    return try_vertex_cover_naive_impl(graph, vertex_cover_size);
}

std::optional<std::set<int>> vertex_cover_kernelized(const Graph& graph, unsigned int vertex_cover_size) {

    auto all_vertices = graph.vertices();
    std::set<int> candidate_vertices{all_vertices.begin(), all_vertices.end()};
    std::set<int> vertex_cover{};

    // remove isolated vertices
    std::erase_if(candidate_vertices, [&](int v) { return graph.degree(v) == 0; });

    Graph graph_copy = graph;
    int k = static_cast<int>(vertex_cover_size);
    while (k > 0) {
        auto degree_comparer = [&graph_copy](int a, int b) -> bool {
            return graph_copy.degree(a) < graph_copy.degree(b);
        };
        int max_degree_vertex = *std::max_element(candidate_vertices.begin(),
                                                  candidate_vertices.end(),
                                                  degree_comparer);
        bool is_tops = graph_copy.degree(max_degree_vertex) > k;
        if (!is_tops) {
            if (candidate_vertices.size() > k * k) {
                // there cannot be a vertex cover
                return std::nullopt;
            }
            break;
        }
        vertex_cover.insert(max_degree_vertex);
        candidate_vertices.erase(max_degree_vertex);
        // disconnect from the graph
        for (int adj: graph_copy.adjacent(max_degree_vertex)) {
            graph_copy.remove_edge({max_degree_vertex, adj});
        }
        k--;
    }

    if (is_valid_vertex_cover(graph, vertex_cover)) {
        return vertex_cover;
    }

    // perform naive search
    auto remaining = try_vertex_cover_naive_impl(graph_copy, k);
    if (remaining) {
        std::set<int> result{};
        std::set_union(vertex_cover.begin(),
                       vertex_cover.end(),
                       remaining->begin(),
                       remaining->end(),
                       std::inserter(result, result.begin()));
        if (result.size() > vertex_cover_size) [[unlikely]] {
            throw std::logic_error("Vertex cover size greater than allowed");
        }
        if (!is_valid_vertex_cover(graph, result)) [[unlikely]] {
            throw std::logic_error("Returning invalid vertex cover");
        }
        return result;
    }
    return std::nullopt;
}
