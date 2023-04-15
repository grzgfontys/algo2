#include <vector>
#include <queue>
#include "graph_connection.h"

std::vector<bool> breadth_first_print(const Graph& graph, int source) {
    std::queue<int> q;
    q.push(source);
    std::vector<bool> visited(graph.vertex_count(), false);
    visited[source] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        for (int j: graph.vertices()) {
            if (graph.are_connected({current, j}) && !visited[j]) {
                q.push(j);
                visited[j] = true;
            }
        }
    }
    return visited;
}

void connect_graph(Graph& graph) {
    std::vector<bool> subgraph = breadth_first_print(graph, 0);
    int last_connection = 0;
    for (int i: graph.vertices()) {
        if (subgraph[i] != 1) {
            graph.add_edge({i, last_connection});
            subgraph = breadth_first_print(graph, 0);
            last_connection = i;
        }
    }
}