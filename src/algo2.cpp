#include <iostream>
#include <fstream>
#include "graph.h"
#include "vertex_cover.h"
#include "graph_connection.h"

void visualise_graph(const Graph& graph) {
    // Create a DOT file that describes the graph
    std::ofstream dot_file("graph.dot");
    dot_file << "graph G {\n";
    for (const auto vertex: graph.vertices()) {
        dot_file << "\t" << vertex << ";\n";
    }
    for (const auto [src, dst]: graph.edges()) {
        dot_file << "\t" << src << " -- " << dst << ";\n";
    }
    dot_file << "}" << std::endl;
    dot_file.close();

    // Use Graphviz to visualize the graph
    system("dot -Tpdf graph.dot -o graph.pdf"); // Change the output format as needed

}

void print_matrix(const Graph& graph) {

    for (int i: graph.vertices()) {
        for (int j: graph.vertices()) {
            std::cout << (graph.are_connected({i, j}) ? 1 : 0) << " ";
        }
        std::cout << "\n";
    }
    std::cout.flush();
}

int main() {
    int vertex_count, probability;
    std::cout << "Input number of vertices: ";
    std::cin >> vertex_count;
    std::cout << "Input probability of connection: ";
    std::cin >> probability;

    Graph graph = Graph::randomized(vertex_count, static_cast<double>(probability) / 100.0);

    visualise_graph(graph);
    print_matrix(graph);

    std::cout << "Do you want to connect the graph? [Y/n]? ";
    std::string input;
    std::cin >> input;
    if (!(input == "n" || input == "N" || input == "no" || input == "No")) {
        connect_graph(graph);
        visualise_graph(graph);
        print_matrix(graph);
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