#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>

std::vector<bool> breadth_first_print(int** matrix, int vertices, int source) {
    std::queue<int> q;
    q.push(source);
    std::vector<bool> visited(vertices, false);
    visited[source] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        std::cout << current << " ";
        for (int j = 0; j < vertices; j++) {
            if (matrix[current][j] == 1 && !visited[j]) {
                q.push(j);
                visited[j] = true;
            }
        }
    }
    std::cout << std::endl;
    return visited;
}

void visualise_graph(int** matrix, int vertices) {
    // Create a DOT file that describes the graph
    std::ofstream dotFile("graph.dot");
    dotFile << "graph G {\n";
    bool single;
    for (int i = 0; i < vertices; i++) {
        single = true;
        for (int j = i + 1; j < vertices; j++) {
            // To avoid duplicate edges
            if (matrix[i][j] == 1) {
                dotFile << "  " << i << " -- " << j << ";\n";
                single = false;
            }
        }
        if (single) {
            dotFile << "  " << i << ";\n";
        }
    }
    dotFile << "}\n";
    dotFile.close();

    // Use Graphviz to visualize the graph
    system("dot -Tpdf graph.dot -o graph.pdf"); // Change the output format as needed

}

void connectGraph(int** matrix, int vertices) {
    // Connect Graph
    std::vector<bool> subGraph = breadth_first_print(matrix, vertices, 0);
    int lastConnection = 0;
    for (int i = 0; i < vertices; i++) {
        if (subGraph[i] != 1) {
            matrix[lastConnection][i] = 1;
            matrix[i][lastConnection] = 1;
            subGraph = breadth_first_print(matrix, vertices, 0);
            lastConnection = i;
        }
    }

    int stop;
    std::cin >> stop;
}

void print_matrix(int** matrix, int vertices) {

    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            std::cout << matrix[i][j];
        }
        std::cout << "\n";
    }
}

bool should_assign(int probability) {
    int outcome = rand() % 100 + 1;
    if (outcome >= probability)
        return true;
    else
        return false;
}

bool validate_vertex_cover(const int* vertex_cover_vertices, int** vertexCoverMatrix, int vertices) {
    for (int i = 0; i < vertices; i++) {
        for (int j = i + 1; j < vertices; j++) {
            if (vertexCoverMatrix[i][j] == 1 && vertex_cover_vertices[i] == 0 && vertex_cover_vertices[j] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool vertex_cover_of_size(int** matrix, int vertices) {
    int minimum_vertex_cover;
    std::cin >> minimum_vertex_cover;

    int vertex_cover_vertices[vertices];
    memset(vertex_cover_vertices, 0, sizeof vertex_cover_vertices);
    int** vertex_cover_matrix;
    vertex_cover_matrix = new int* [vertices];
    memcpy(vertex_cover_matrix, matrix, vertices * vertices * sizeof(int));

    for (int k = 0; k <= minimum_vertex_cover; k++) {
        memset(vertex_cover_vertices, 0, sizeof vertex_cover_vertices);
        // add the size of the vertex cover for permutations
        for (int i = vertices - 1; i >= vertices - k; i--) {
            vertex_cover_vertices[i] = 1;
        }
        do {
            //for (int j = 0; j < vertices; j++)
            //{
            //    std::cout << vertex_cover_vertices[j] << ' ';
            //}
            //std::cout << '\n';
            if (validate_vertex_cover(vertex_cover_vertices, vertex_cover_matrix, vertices)) {
                return true;
            }
        } while (std::next_permutation(vertex_cover_vertices, vertex_cover_vertices + vertices));
    }
    return false;
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
    srand((unsigned) time(nullptr));
    std::cout << "Hello World!\n";
    int verts, probability;
    std::cin >> verts;
    std::cin >> probability;

    probability = (probability - 100) * -1;

    /*   int matrix[verts][verts];
       memset(matrix, 0, sizeof matrix);*/

    int** matrix;
    matrix = new int* [verts];
    for (int i = 0; i < verts; i++)
        matrix[i] = new int[verts];

    for (int i = 0; i < verts; i++) {
        for (int j = i + 1; j < verts; j++) {
            if (should_assign(probability)) {
                matrix[i][j] = 1;
                matrix[j][i] = 1;
            } else {
                matrix[i][j] = 0;
                matrix[j][i] = 0;
            }
        }
    }

    visualise_graph(matrix, verts);
    print_matrix(matrix, verts);

    //connectGraph(matrix, verts);

    //visualise_graph(matrix, verts);
    //print_matrix(matrix, verts);

    //per();

    if (vertex_cover_of_size(matrix, verts)) {
        std::cout << "possible";
    } else {
        std::cout << "not possible";
    }

    return 0;
}