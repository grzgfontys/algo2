#include "graph.h"
#include <stdexcept>
#include <random>

Graph::Graph(int vertex_count) {
    m_adjacency_matrix = span(new span<int>[vertex_count], vertex_count);
    for (auto& row: m_adjacency_matrix) {
        row = span<int>(new int[vertex_count], vertex_count);
    }
}

Graph::~Graph() {
    for (auto& row: m_adjacency_matrix) {
        delete[] row.data();
    }
    delete[] m_adjacency_matrix.data();
}

Graph Graph::randomized(int vertex_count, double connection_probability) {
    if (connection_probability < 0.0 || connection_probability > 1.0) {
        throw std::invalid_argument("Connection probability must be between 0 and 1");
    }

    Graph graph = Graph(vertex_count);
    span<span<int>>& adj_matrix = graph.m_adjacency_matrix;

    std::random_device rd{};
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0,1.0);
    auto should_assign = [&](){return dist(mt) >= connection_probability;};

    for (int i = 0; i < vertex_count; i++) {
        for (int j = i + 1; j < vertex_count; j++) {
            if (should_assign()) {
                adj_matrix[i][j] = 1;
                adj_matrix[j][i] = 1;
            } else {
                adj_matrix[i][j] = 0;
                adj_matrix[j][i] = 0;
            }
        }
    }

    return graph;
}
