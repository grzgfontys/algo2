#include <sstream>
#include <iostream>
#include <fstream>
#include "graph_visualization.h"

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

std::string adjacency_matrix_to_string(const Graph& graph) {
    std::stringstream ss;

    for (int i: graph.vertices()) {
        for (int j: graph.vertices()) {
            ss << (graph.are_connected({i, j}) ? 1 : 0) << " ";
        }
        ss << "\n";
    }

    return ss.str();
}

void print_adjacency_matrix(const Graph& graph){
    std::cout << adjacency_matrix_to_string(graph);
    std::cout.flush();
}