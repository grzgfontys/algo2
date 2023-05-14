## Week 1

### Generate graph

For the Task "Generate graph” we have programmed algorithm
that takes number of vertices _n_ and a probability _p_,
then creates _NxN_ adjacency matrix for the graph.
Next step is to loop through the matrix and for every
2 elements we are generation pseudo random value.
If the value is above or equal to the probability threshold _p_
we are making connection between two nodes.
```c++
auto should_assign = [&]() { return random() < connection_probability; };

for (int i = 0; i < vertex_count; i++) {
    for (int j = i; j < vertex_count; j++) {
        if (should_assign()) {
            graph.add_edge({i, j});
        } else {
            graph.remove_edge({i, j});
        }
    }
}
```

### Visualizing the graph

The adjacency matrix is converted into `.dot` file for GraphViz and saved as PDF.
We generate a `.dot` file by looping over all the vertices and edges and adding them to the file.
```c++
for (const auto vertex: graph.vertices()) {
    dot_file << "\t" << vertex << ";\n";
}
for (const auto [src, dst]: graph.edges()) {
    dot_file << "\t" << src << " -- " << dst << ";\n";
}
```

### Connect

For connecting a graph we initialize a set of vertices that initially contains all the graph vertices.
Then, we start a BFS algorithm from vertex 0.
On each visit of BFS, we erase that vertex from the set of disconnected vertices.
```c++
auto vertices = graph.vertices();
std::set disconnected_nodes(vertices.begin(), vertices.end());

std::set<int> visited{};
constexpr int source_vertex = 0;
breadth_first_search(graph, source_vertex, visited, [&](int v) {
    disconnected_nodes.erase(v);
});
```

After the initial pass, we check if the set of disconnected vertices is empty.
If it is not, we add an edge from 0 to the first disconnected vertex and repeat the BFS from the newly connected vertex.
To improve performance, we maintain the set of visited vertices between the calls to BFS.
We repeat this process until the set of disconnected vertices is empty.

```c++
while (!disconnected_nodes.empty()) {
    const int vertex = *disconnected_nodes.begin(); // take first disconnected edge
    graph.add_edge({source_vertex, vertex}); // add edge from source_vertex to disconnected edge

    // mark all newly connected vertices as connected
    breadth_first_search(graph, vertex, visited, [&](int v) {
        disconnected_nodes.erase(v); // if we encounter edge during BFS, it is connected
    });
}
```

After all this steps, the graph is guaranteed to be connected.

## Week 2

### Brute force search

For the brute force Vertex Cover search we are checking all the
possible permutations of the vertices' assignments starting with the 0 assigned vertex,
and we are validating the adjacency matrix if generated assignment covers all the edges.
If not, we are generating next assignment, either with different vertex assignment
of larger vertex amount.

Below is a simplified version of our implementation given in pseudocode:

```c++
for(int n = 0; n <= MAX_COVER_SIZE; n++) {
    for(each permutation of n vertices) {
        if (is valid vertex cover) {
            return permutation;
        }
    }
}
return NO SOLUTION;
```

## Week 3

### Finding pendants and tops

The first step was to implement degree of a graph vertex.
This is done very easily in an undirected graph in a form of adjacency matrix.
Only thing that needs to be done is to count the number of 1's in the row of the matrix, which can simply be done by
calculating the sum of that row.

Having implemented degree it is trivial to implement finding the isolated, pendant and tops vertices.

### Modifying the number of pendants and tops

#### Adding pendant vertices

To add a pendant vertex we must firstly find a vertex
that is not pendant and then remove all the edges except one.
However, we must firstly check that the edges are _removable_,
meaning that they can be removed without making the other
vertex pendant too.
If we were to do so, we could modify the number of pendants by more than we want to.

#### Removing pendant vertices

To remove pendant vertices we just make them disconnected.
If the adjacent vertex is also pendant, and we want to remove more than 1 pendants,
we can remove 2 pendants at once.

#### Adding tops vertices

To add a tops vertex we simply add edges connecting to other vertices.
However, we need to be careful that by doing so we don't make the other vertex tops too.
That is why when choosing the vertex to add connection to,
we skip those that are 1 edge from becoming tops.

#### Removing tops vertices

To remove tops vertices we simply remove edges from some tops vertex.
To avoid removing more than intended tops vertices,
we cannot remove an edge if the other edge will stop being tops as a result.

## Week 4

### Improved brute force

To improve the brute force we use kernelization as
described [here](http://en.wikipedia.org/wiki/Kernelization#Example:_vertex_cover).

We firstly create 2 sets: one for candidate vertices (i.e. those that will be considered for vertex cover), and one for
vertex cover itself.
The vertex cover set starts empty, and the candidate vertices are all vertices of the graph, except the isolated ones.
Then we copy the graph to be able to remove parts of it to reduce the problem size.

In the main part of the algorithm we initialize `k` to our vertex cover size, and loop until `k > 0`.

In each iteration we find the vertex with the greatest degree.
If this degree is greater than `k`, we add it to the vertex cover, remove from candidate vertices, and disconnect it
from the graph.
Finally, we decrement `k` by 1.

```c++
while (k > 0) {
    int max_degree_vertex = ...
    bool is_tops = graph_copy.degree(max_degree_vertex) > k;
    
    if (is_tops){
        vertex_cover.insert(max_degree_vertex);
        candidate_vertices.erase(max_degree_vertex);
        
        // disconnect from the graph
        for (int adj: graph_copy.adjacent(max_degree_vertex)) {
            graph_copy.remove_edge({max_degree_vertex, adj});
        }
        k--;
    }
}
```

If however, the vertex does not have sufficient degree, we check the number of the remaining candidate vertices.
If it is larger than `k^2`, there cannot be a solution, so we immediately return `std::nullopt`.
Otherwise, we break out of the loop.

```c++
while (k > 0) {
    int max_degree_vertex = ...
    bool is_tops = graph_copy.degree(max_degree_vertex) > k;
    
    if (!is_tops){
        if (candidate_vertices.size() > k * k) {
            // there cannot be a vertex cover
            return std::nullopt;
        }
        break;
    }
    ...
}
```

After the loop finishes, we check if the current vertex cover is in fact a valid cover (i.e. if it covers all the
edges).
If it does, we return the solution.
```c++
if (is_valid_vertex_cover(graph, vertex_cover)) {
    return vertex_cover;
}
```
If however, not all edges are covered, we try to cover the remaining candidate vertices using the naive brute force
implementation.
```c++
// perform naive search
auto remaining = try_vertex_cover_naive_impl(graph_copy, k);
```
If the brute force succeeds, we return a set union of the incomplete vertex cover, and the result from brute force.
If, however, the brute force does not succeed, there cannot exist a solution. 

```c++
if (remaining) {
    std::set<int> result{};
    std::set_union(vertex_cover.begin(),
                   vertex_cover.end(),
                   remaining->begin(),
                   remaining->end(),
                   std::inserter(result, result.begin()));

    return result;
}
return std::nullopt;
```

