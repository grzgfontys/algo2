#ifndef ALGORITHMS_2_GRAPH_H
#define ALGORITHMS_2_GRAPH_H

#include <span>
#include <initializer_list>

using std::span;

class Graph
{
public:
    struct Edge
    {
        int src, dst;
    };
private:
    span<span<int>> m_adjacency_matrix;
public:
    explicit Graph(int vertex_count);

    static Graph randomized(int vertex_count, double connection_probability);

    void add_edge(Edge edge);

    void remove_edge(Edge edge);

    bool are_connected(Graph::Edge edge);

    virtual ~Graph();
};


#endif //ALGORITHMS_2_GRAPH_H
