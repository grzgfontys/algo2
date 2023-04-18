#include <unordered_set>
#include <queue>
#include <functional>
#include "graph_connection.h"

void breadth_first_search(const Graph& graph,
                          int source,
                          std::unordered_set<int>& visited,
                          const std::function<void(int)>& on_visit) {
    std::queue<int> q;
    q.push(source);
    visited.insert(source);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        on_visit(current);
        for (int vertex: graph.adjacent(current)) {
            if (!visited.contains(vertex)) {
                q.push(vertex);
                visited.insert(vertex);
            }
        }
    }
}


void connect_graph(Graph& graph) {
    auto vertices = graph.vertices();
    std::unordered_set disconnected_nodes(vertices.begin(), vertices.end());

    std::unordered_set<int> visited{};
    constexpr int source_vertex = 0;
    breadth_first_search(graph, source_vertex, visited, [&](int v) {
        disconnected_nodes.erase(v);
    });

    while (!disconnected_nodes.empty()) {
        const int vertex = *disconnected_nodes.begin(); // take first disconnected edge
        graph.add_edge({source_vertex, vertex}); // add edge from source_vertex to disconnected edge

        // mark all newly connected vertices as connected
        breadth_first_search(graph, vertex, visited, [&](int v) {
            disconnected_nodes.erase(v); // if we encounter edge during BFS, it is connected
        });
    }
}