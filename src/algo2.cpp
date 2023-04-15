#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <ranges>
#include <optional>
#include <span>
#include "graph.h"

std::vector<bool> breadth_first_print(const Graph& graph, int source) {
    std::queue<int> q;
    q.push(source);
    std::vector<bool> visited(graph.vertex_count(), false);
    visited[source] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        std::cout << current << " ";
        for (int j: graph.vertices()) {
            if (graph.are_connected({current, j}) && !visited[j]) {
                q.push(j);
                visited[j] = true;
            }
        }
    }
    std::cout << std::endl;
    return visited;
}

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

void connect_graph(Graph graph) {
    // Connect Graph
    std::vector<bool> subgraph = breadth_first_print(graph, 0);
    int last_connection = 0;
    for (int i: graph.vertices()) {
        if (subgraph[i] != 1) {
            graph.add_edge({i, last_connection});
            subgraph = breadth_first_print(graph, 0);
            last_connection = i;
        }
    }

    int stop;
    std::cin >> stop;
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

std::optional<std::unordered_set<int>> vertex_cover_of_size(const Graph& graph, unsigned int vertex_cover_size) {
    using std::ranges::all_of;

    const unsigned int vertex_count = graph.vertex_count();
    std::span<bool> vertex_mask = std::span(new bool[vertex_count], vertex_count);

    for (int k = 0; k <= vertex_cover_size; k++) {
        memset(vertex_mask.data(), 0, vertex_mask.size_bytes()); // set all to 0
        memset(vertex_mask.data(), 1, k * sizeof(bool)); // set k first to 1
        do {
            // range of all the vertices that are set in the mask
            auto selected_vertices = std::views::iota(0u, vertex_count)
                    | std::ranges::views::filter([&](int x) { return vertex_mask[x]; });

            // convert range to hashset
            std::unordered_set<int> vertex_cover_vertices(selected_vertices.begin(), selected_vertices.end());

            // ensure that all edges are being covered by at least 1 vertex
            if (all_of(graph.edges(), [&](const auto& edge) {
                auto [src, dst] = edge;
                return vertex_cover_vertices.contains(src) || vertex_cover_vertices.contains(dst);
            })) {
                return vertex_cover_vertices;
            }
        } while (std::prev_permutation(vertex_mask.begin(), vertex_mask.end()));
    }
    return std::nullopt;
}

//void per()
//{
//    int myints[3] = { 0, 0, 1 };
//
//    std::cout << "The 3! possible permutations with 3 elements:\n";
//    do {
//        std::cout << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n';
//    } while (std::next_permutation(myints, myints + 3));
//
//    std::cout << "After loop: " << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n';
//
//
//    for (int k = 0; k <= 3; k++)
//    {
//        memset(myints, 0, sizeof myints);
//        // add the size of the vertex cover for permutations
//        for (int i = 3 - 1; i >= 3-k; i--)
//        {
//            myints[i] = 1;
//        }
//        do {
//            for (int j = 0; j < 3; j++)
//            {
//                std::cout << myints[j] << ' ';
//            }
//            std::cout << '\n';
//        } while (std::next_permutation(myints, myints + 3));
//    }
//}

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