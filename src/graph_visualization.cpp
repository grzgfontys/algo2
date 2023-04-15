#include <sstream>
#include <iostream>
#include <fstream>
#include <format>
#include "graph_visualization.h"

std::string get_format_string(GraphOutputType format) {
    switch (format) {
        case BMP:
            return "bmp";
        case PNG:
            return "png";
        case JPEG:
            return "jpeg";
        case SVG:
            return "svg";
        case PDF:
            return "pdf";
    }
}

void visualise_graph(const Graph& graph, GraphOutputType format) {

    const std::string DOT_FILENAME = "graph.dot";
    // Create a DOT file that describes the graph
    std::ofstream dot_file(DOT_FILENAME);
    dot_file << "graph G {\n";
    for (const auto vertex: graph.vertices()) {
        dot_file << "\t" << vertex << ";\n";
    }
    for (const auto [src, dst]: graph.edges()) {
        dot_file << "\t" << src << " -- " << dst << ";\n";
    }
    dot_file << "}" << std::endl;
    dot_file.close();

    std::stringstream command;
    auto format_string = get_format_string(format);
    command << "dot " << "-T" << format_string << " " << DOT_FILENAME << " -o graph." << format_string;

    // Use Graphviz to visualize the graph
    system(command.str().c_str()); // Change the output format as needed

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

void print_adjacency_matrix(const Graph& graph) {
    std::cout << adjacency_matrix_to_string(graph);
    std::cout.flush();
}