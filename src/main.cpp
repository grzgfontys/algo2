#include <iostream>
#include "graph.h"
#include "vertex_cover.h"
#include "graph_connection.h"
#include "graph_visualization.h"
#include "kernelization.h"

void display_graph_stats(const Graph& graph, int tops_threshold) {
    std::cout << "Number of vertices: " << graph.vertices().size() << "\nIsolated vertices: "
              << isolated_vertices(graph).size() << "\nPendant vertices: " << pendant_vertices(graph).size()
              << "\nTops(" << tops_threshold << ") vertices: " << tops_vertices(graph, tops_threshold).size()
              << std::endl;
}

enum class CommandResult
{
    OK,
    Exit,
    Undefined,
    Error
};

CommandResult handle_command(Graph& graph, const std::string& command, int tops_threshold) {
    if (command == "connect") {
        connect_graph(graph);
        return CommandResult::OK;
    } else if (command == "cover") {
        int vertex_cover_size;
        std::cout << "Input vertex cover size: ";
        std::cin >> vertex_cover_size;

        auto vertex_cover_result = vertex_cover_of_size(graph, vertex_cover_size);
        if (vertex_cover_result) {
            std::cout << "possible\n";
            for (auto vertex: vertex_cover_result.value()) {
                std::cout << vertex << " ";
            }
            std::cout << "\n";
        } else {
            std::cout << "not possible\n";
        }
        std::cout.flush();
        return CommandResult::OK;
    }
    if (command == "stats") {
        display_graph_stats(graph, tops_threshold);
        return CommandResult::OK;
    }
    if (command == "p++") {
        add_pendants(graph, 1);
        return CommandResult::OK;
    }
    if (command == "p--") {
        remove_pendants(graph, 1);
        return CommandResult::OK;
    }
    if (command == "t++") {
        add_tops(graph, tops_threshold, 1);
        return CommandResult::OK;
    }
    if (command == "t--") {
        remove_tops(graph, tops_threshold, 1);
        return CommandResult::OK;
    }
    if (command == "exit") {
        return CommandResult::Exit;
    }
    return CommandResult::Undefined;
}

int main() {
    constexpr auto OUTPUT_FORMAT = GraphOutputType::PNG;

    int vertex_count, probability;
    std::cout << "Input number of vertices: ";
    std::cin >> vertex_count;
    std::cout << "Input probability of connection: ";
    std::cin >> probability;

    Graph graph = Graph::randomized(vertex_count, static_cast<double>(probability) / 100.0);

    visualise_graph(graph, OUTPUT_FORMAT);
    print_adjacency_matrix(graph);

    int tops_threshold;
    std::cout << "Enter tops threshold: ";
    std::cin >> tops_threshold;

    CommandResult result;
    do {
        std::string input;
        std::cout << "> ";
        std::cin >> input;

        try {
            result = handle_command(graph, input, tops_threshold);
        } catch (std::runtime_error& err) {
            std::cerr << "An error occurred while processing the command: " << err.what() << std::endl;
            result = CommandResult::Error;
        }

        if (result == CommandResult::Undefined) {
            std::cerr << "No such command, type \'exit\' to exit." << std::endl;
        }

        if (result == CommandResult::OK) {
            visualise_graph(graph, OUTPUT_FORMAT);
        }

    } while (result != CommandResult::Exit);

    return 0;
}