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

    std::random_device rd{};
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    auto should_assign = [&]() { return dist(mt) < connection_probability; };

    for (int i = 0; i < vertex_count; i++) {
        for (int j = i + 1; j < vertex_count; j++) {
            if (should_assign()) {
                graph.add_edge({i, j});
            } else {
                graph.remove_edge({i, j});
            }
        }
    }

    return graph;
}

unsigned int Graph::vertex_count() const {
    return m_adjacency_matrix.size();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-make-member-function-const"

void Graph::add_edge(Graph::Edge edge) {
    auto [i, j] = edge;
    if (i == j) {
        return;
    }
    m_adjacency_matrix[i][j] = 1;
    m_adjacency_matrix[j][i] = 1;
}

void Graph::remove_edge(Graph::Edge edge) {
    auto [i, j] = edge;
    m_adjacency_matrix[i][j] = 0;
    m_adjacency_matrix[j][i] = 0;
}

#pragma clang diagnostic pop

bool Graph::are_connected(Graph::Edge edge) const {
    auto [i, j] = edge;
    if (m_adjacency_matrix[i][j] > 0 && m_adjacency_matrix[j][i] > 0) {
        return true;
    }
    if (m_adjacency_matrix[i][j] == 0 && m_adjacency_matrix[j][i] == 0) {
        return false;
    }
    throw std::logic_error("Unidirectional edge in non-directed graph");
}

vector<int> Graph::vertices() const {
    vector<int> vertices{};
    vertices.reserve(m_adjacency_matrix.size());
    for (int i = 0; i < m_adjacency_matrix.size(); ++i) {
        vertices.push_back(i);
    }
    return vertices;
}

vector<Graph::Edge> Graph::edges() const {
    vector<Edge> edges{};
    for (int i = 0; i < m_adjacency_matrix.size(); ++i) {
        for (int j = i + 1; j < m_adjacency_matrix.size(); ++j) {
            Edge edge = {i, j};
            if (are_connected(edge)) {
                edges.push_back(edge);
            }
        }
    }
    return edges;
}
