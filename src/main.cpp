#include <iostream>
#include "graph.h"
#include "vertex_cover.h"
#include "graph_connection.h"
#include "graph_visualization.h"

int main() {
    int vertex_count, probability;
    std::cout << "Input number of vertices: ";
    std::cin >> vertex_count;
    std::cout << "Input probability of connection: ";
    std::cin >> probability;

    Graph graph = Graph::randomized(vertex_count, static_cast<double>(probability) / 100.0);
    constexpr auto output_format = GraphOutputType::PNG;

    visualise_graph(graph, output_format);
    print_adjacency_matrix(graph);

    std::cout << "Do you want to connect the graph? [Y/n]? ";
    std::string input;
    std::cin >> input;
    if (!(input == "n" || input == "N" || input == "no" || input == "No")) {
        connect_graph(graph);
        visualise_graph(graph, output_format);
        print_adjacency_matrix(graph);
    }

    int vertex_cover_size;
    std::cout << "Input vertex cover size: ";
    std::cin >> vertex_cover_size;

    auto vertex_cover_result = vertex_cover_of_size(graph, vertex_cover_size);
    if (vertex_cover_result) {
        std::cout << "possible\n";
        for (auto vertex: vertex_cover_result.value()) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "not possible";
    }

    return 0;
}