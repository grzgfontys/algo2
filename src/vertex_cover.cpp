#include <span>
#include <cstring>
#include <ranges>
#include <functional>
#include <algorithm>
#include "vertex_cover.h"

bool is_valid_vertex_cover(const Graph& graph, const std::unordered_set<int>& vertices) {
    return std::ranges::all_of(graph.edges(), [&](const auto& edge) {
        auto [src, dst] = edge;
        return vertices.contains(src) || vertices.contains(dst);
    });
}

std::optional<std::unordered_set<int>> vertex_cover_of_size(const Graph& graph, unsigned int vertex_cover_size) {
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
            const std::unordered_set<int> vertex_set(selected_vertices.begin(), selected_vertices.end());

            if (is_valid_vertex_cover(graph, vertex_set)) {
                delete[] vertex_mask.data();
                return vertex_set;
            }
        } while (std::prev_permutation(vertex_mask.begin(), vertex_mask.end()));
    }

    delete[] vertex_mask.data();
    return std::nullopt;
}