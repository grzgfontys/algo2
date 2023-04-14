#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;

vector<bool> breadthFirstPrint(int **matrix, int verts, int source)
{
    std::queue<int> q;
    q.push(source);
    vector<bool> visited(verts, 0);
    visited[source] = true;

    while (q.size() > 0)
    {
        int current = q.front();
        q.pop();
        cout << current << " ";
        for (int j = 0; j < verts; j++)
        {
            if (matrix[current][j]==1 && !visited[j])
            {
                q.push(j);
                visited[j] = true;
            }
        }
    }
    cout << endl;
    return visited;
}

void visualizeGraph(int** matrix, int verts)
{
    // Create a DOT file that describes the graph
    ofstream dotFile("graph.dot");
    dotFile << "graph G {\n";
    bool single;
    for (int i = 0; i < verts; i++)
    {
        single = true;
        for (int j = i + 1; j < verts; j++)
        {
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

void connectGraph(int** matrix, int verts)
{
    // Connect Graph
    vector<bool> subGraph = breadthFirstPrint(matrix, verts, 0);
    int lastConnection = 0;
    for (int i = 0; i < verts; i++)
    {
        if (subGraph[i] != 1)
        {
            matrix[lastConnection][i] = 1;
            matrix[i][lastConnection] = 1;
            subGraph = breadthFirstPrint(matrix, verts, 0);
            lastConnection = i;
        }
    }

    int stop;
    std::cin >> stop;
}

void printMatrix(int** matrix, int verts)
{

    for (int i = 0; i < verts; i++)
    {
        for (int j = 0; j < verts; j++)
        {
            std::cout << matrix[i][j];
        }
        std::cout << "\n";
    }
}

bool shouldAssign(int probability)
{
    int outcome = rand() % 100 + 1;
    if (outcome >= probability)
        return true;
    else
        return false;
}

bool validateVertexCover(int* vertexCoverVerts, int** vertexCoverMatrix, int verts)
{
    for (int i = 0; i < verts; i++)
    {
        for (int j = i + 1; j < verts; j++)
        {
            if (vertexCoverMatrix[i][j] == 1 && vertexCoverVerts[i] == 0 && vertexCoverVerts[j] == 0)
            {
                return false;
            }
        }
    }
    return true;
}

bool vertexCoverOfSize(int** matrix, int verts)
{
    int minVetexCover;
    std::cin >> minVetexCover;

    int vertexCoverVerts[verts];
    memset(vertexCoverVerts, 0, sizeof vertexCoverVerts);
    int** vertexCoverMatrix;
    vertexCoverMatrix = new int* [verts];
    memcpy(vertexCoverMatrix, matrix, verts * verts * sizeof(int));

    for (int k = 0; k <= minVetexCover; k++)
    {
        memset(vertexCoverVerts, 0, sizeof vertexCoverVerts);
        // add the size of the vertex cover for permutations
        for (int i = verts-1; i >= verts-k; i--)
        {
            vertexCoverVerts[i] = 1;
        }
        do {
            //for (int j = 0; j < verts; j++)
            //{
            //    std::cout << vertexCoverVerts[j] << ' ';
            //}
            //std::cout << '\n';
            if (validateVertexCover(vertexCoverVerts, vertexCoverMatrix, verts))
            {
                return true;
            }
        } while (std::next_permutation(vertexCoverVerts, vertexCoverVerts + verts));
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

int main()
{
    srand((unsigned)time(NULL));
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
        matrix[i] = new int[verts] {0};

    for (int i = 0; i < verts; i++)
    {
        for (int j = i+1; j < verts; j++)
        {
            if (shouldAssign(probability))
            {
                matrix[i][j] = 1;
                matrix[j][i] = 1;
            }
            else
            {
                matrix[i][j] = 0;
                matrix[j][i] = 0;
            }
        }
    }

    visualizeGraph(matrix, verts);
    printMatrix(matrix, verts);

    //connectGraph(matrix, verts);

    //visualizeGraph(matrix, verts);
    //printMatrix(matrix, verts);

    //per();

    if (vertexCoverOfSize(matrix, verts))
    {
        std::cout << "possible";
    }
    else {
        std::cout << "not possible";
    }

    return 0;
}