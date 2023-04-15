#ifndef ALGORITHMS_2_GRAPH_H
#define ALGORITHMS_2_GRAPH_H

#include <span>
#include <vector>

using std::span;
using std::vector;

class Graph
{
public:
    struct Edge
    {
        int src, dst;
    };
public:
    span<span<int>> m_adjacency_matrix;
public:
    explicit Graph(int vertex_count);

    static Graph randomized(int vertex_count, double connection_probability);

    unsigned int vertex_count() const;

    void add_edge(Edge edge);

    void remove_edge(Edge edge);

    [[nodiscard]] bool are_connected(Graph::Edge edge) const;

    [[nodiscard]] vector<int> vertices() const;

    [[nodiscard]] vector<Edge> edges() const;

    virtual ~Graph();
};


#endif //ALGORITHMS_2_GRAPH_H
