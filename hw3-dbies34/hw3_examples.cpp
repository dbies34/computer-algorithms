//----------------------------------------------------------------------
// Name: S. Bowers
// File: hw3_examples.cpp
// Date: Spring 2021
// Desc: BFS algorithm tests for example graphs
//----------------------------------------------------------------------


#include <iostream>
#include <fstream>
#include <string>
#include <gtest/gtest.h>
#include "adjacency_list.h"
#include "adjacency_matrix.h"

using namespace std;


void get_stats(int& vertices, int& edges, const string& filename)
{
  ifstream f(filename);
  f >> vertices >> edges; 
  f.close();
}

void build_graph(Graph* g, const string& filename)
{
  ifstream f(filename);
  int vertices = 0;
  int edges = 0;
  f >> vertices >> edges; 
  for (int i = 0; i < edges; ++i) {
    int n1 = 0, w = 0, n2 = 0;
    f >> n1 >> w >> n2;
    g->set_edge(n1, w, n2);
  }
  f.close();
}

int connected_component_size(const Map& components, int component)
{
  int component_count = 0;
  for (pair<int,int> p : components) {
    if (p.second == component)
      component_count++;
  }
  return component_count;
}


//----------------------------------------------------------------------
// Adjacency Matrix Tests
//----------------------------------------------------------------------

// BFS test
TEST(AdjacencyMatrixTest, BFSMediumSizedRandomGraph) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map search_tree;
  // directed (largest and smallest search tree)
  g->bfs(true, 1918, search_tree);
  ASSERT_EQ(347, search_tree.size());
  search_tree.clear();
  g->bfs(true, 0, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->bfs(false, 2, search_tree);
  ASSERT_EQ(1948, search_tree.size());
  search_tree.clear();
  g->bfs(false, 0, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  delete g;
}

// BFS test
TEST(AdjacencyMatrixTest, BFSMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map search_tree;
  // directed (largest and smallest search tree)
  g->bfs(true, 0, search_tree);
  ASSERT_EQ(2048, search_tree.size());
  search_tree.clear();
  g->bfs(true, 2047, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->bfs(false, 0, search_tree);
  ASSERT_EQ(2048, search_tree.size());
  delete g;
}

// BFS test
TEST(AdjacencyMatrixTest, BFSMediumSized2dGridGraph) {
  int size = 2500;
  string filename = "examples/grid-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map search_tree;
  // directed (largest and smallest search tree)
  g->bfs(true, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->bfs(false, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  delete g;
}

// BFS test
TEST(AdjacencyMatrixTest, BFSMediumSizedBarbellGraph) {
  int size = 2500;
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map search_tree;
  // directed (largest and smallest search tree)
  g->bfs(true, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  search_tree.clear();  
  g->bfs(true, 2499, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->bfs(false, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  delete g;
}


// shortest-path test
TEST(AdjacencyMatrixTest, ShortestPathMediumSizedRandomGraph) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  list<int> path;
  g->shortest_path_length(true, 1918, 2487, path);
  ASSERT_EQ(5, path.size());
  ASSERT_EQ(1918, path.front());
  ASSERT_EQ(2487, path.back());
  path.clear();
  g->shortest_path_length(false, 1918, 2499, path);
  ASSERT_EQ(10, path.size());
  ASSERT_EQ(1918, path.front());
  ASSERT_EQ(2499, path.back());
  delete g;
}

// shortest-path test
TEST(AdjacencyMatrixTest, ShortestPathMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  list<int> path;
  g->shortest_path_length(true, 0, 2047, path);
  ASSERT_EQ(12, path.size());
  ASSERT_EQ(0, path.front());
  ASSERT_EQ(2047, path.back());
  path.clear();
  g->shortest_path_length(false, 0, 2047, path);
  ASSERT_EQ(12, path.size());
  ASSERT_EQ(0, path.front());
  ASSERT_EQ(2047, path.back());
  delete g;
}

// shortest-path test
TEST(AdjacencyMatrixTest, ShortestPathMediumSized2dGridGraph) {
  int size = 2500;
  string filename = "examples/grid-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  list<int> path;
  g->shortest_path_length(true, 0, 2499, path);
  ASSERT_EQ(99, path.size());
  ASSERT_EQ(0, path.front());
  ASSERT_EQ(2499, path.back());
  path.clear();
  g->shortest_path_length(false, 0, 2499, path);
  ASSERT_EQ(99, path.size());
  ASSERT_EQ(0, path.front());
  ASSERT_EQ(2499, path.back());
  delete g;
}

// shortest-path test
TEST(AdjacencyMatrixTest, ShortestPathMediumSizedBarbellGraph) {
  int size = 2500;
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  list<int> path;
  g->shortest_path_length(true, 0, 2499, path);
  ASSERT_EQ(6, path.size());
  ASSERT_EQ(0, path.front());
  ASSERT_EQ(2499, path.back());
  path.clear();
  g->shortest_path_length(false, 0, 2499, path);
  ASSERT_EQ(6, path.size());
  ASSERT_EQ(0, path.front());
  ASSERT_EQ(2499, path.back());
  delete g;
}


// connected components test
TEST(AdjacencyMatrixTest, ConnectedComponentsMediumSizedRandomGraph) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map components;
  g->bfs_connected_components(components);
  ASSERT_EQ(vertices, components.size());
  ASSERT_EQ(1948, connected_component_size(components, 2));
  delete g;
}

// connected components test
TEST(AdjacencyMatrixTest, ConnectedComponentsMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map components;
  g->bfs_connected_components(components);
  ASSERT_EQ(vertices, components.size());
  ASSERT_EQ(2048, connected_component_size(components, 0));
  delete g;
}

// connected components test
TEST(AdjacencyMatrixTest, ConnectedComponentsMediumSized2dGridGraph) {
  int size = 2500;
  string filename = "examples/grid-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map components;
  g->bfs_connected_components(components);
  ASSERT_EQ(vertices, components.size());
  ASSERT_EQ(2500, connected_component_size(components, 0));
  delete g;
}

// connected components test
TEST(AdjacencyMatrixTest, ConnectedComponentsMediumSizedBarbellGraph) {
  int size = 2500;
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map components;
  g->bfs_connected_components(components);
  ASSERT_EQ(vertices, components.size());
  ASSERT_EQ(2500, connected_component_size(components, 0));
  delete g;
}


// bipartite test
TEST(AdjacencyMatrixTest, BipartiteMediumSizedRandomGraph) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map coloring;
  ASSERT_EQ(false, g->bipartite_graph(coloring));
  delete g;
}

// bipartite test
TEST(AdjacencyMatrixTest, BipartiteMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map coloring;
  ASSERT_EQ(true, g->bipartite_graph(coloring));
  delete g;
}

// bipartite test
TEST(AdjacencyMatrixTest, BipartiteMediumSized2dGridGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map coloring;
  ASSERT_EQ(true, g->bipartite_graph(coloring));
  delete g;
}

// bipartite test
TEST(AdjacencyMatrixTest, BipartiteMediumSizedBarbellGraph) {
  int size = 2500;
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map coloring;
  ASSERT_EQ(false, g->bipartite_graph(coloring));
  delete g;
}



//----------------------------------------------------------------------
// Adjacency List Tests
//----------------------------------------------------------------------

// BFS test
TEST(AdjacencyListTest, BFSMediumSizedRandomGraph) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map search_tree;
  // directed (largest and smallest search tree)
  g->bfs(true, 1918, search_tree);
  ASSERT_EQ(347, search_tree.size());
  search_tree.clear();
  g->bfs(true, 0, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->bfs(false, 2, search_tree);
  ASSERT_EQ(1948, search_tree.size());
  search_tree.clear();
  g->bfs(false, 0, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  delete g;
}

// BFS test
TEST(AdjacencyListTest, BFSMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map search_tree;
  // directed (largest and smallest search tree)
  g->bfs(true, 0, search_tree);
  ASSERT_EQ(2048, search_tree.size());
  search_tree.clear();
  g->bfs(true, 2047, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->bfs(false, 0, search_tree);
  ASSERT_EQ(2048, search_tree.size());
  delete g;
}

// BFS test
TEST(AdjacencyListTest, BFSMediumSized2dGridGraph) {
  int size = 2500;
  string filename = "examples/grid-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map search_tree;
  // directed (largest and smallest search tree)
  g->bfs(true, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->bfs(false, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  delete g;
}

// BFS test
TEST(AdjacencyListTest, BFSMediumSizedBarbellGraph) {
  int size = 2500;
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map search_tree;
  // directed (largest and smallest search tree)
  g->bfs(true, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  search_tree.clear();  
  g->bfs(true, 2499, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->bfs(false, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  delete g;
}


// shortest-path test
TEST(AdjacencyListTest, ShortestPathMediumSizedRandomGraph) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  list<int> path;
  g->shortest_path_length(true, 1918, 2487, path);
  ASSERT_EQ(5, path.size());
  ASSERT_EQ(1918, path.front());
  ASSERT_EQ(2487, path.back());
  path.clear();
  g->shortest_path_length(false, 1918, 2499, path);
  ASSERT_EQ(10, path.size());
  ASSERT_EQ(1918, path.front());
  ASSERT_EQ(2499, path.back());
  delete g;
}

// shortest-path test
TEST(AdjacencyListTest, ShortestPathMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  list<int> path;
  g->shortest_path_length(true, 0, 2047, path);
  ASSERT_EQ(12, path.size());
  ASSERT_EQ(0, path.front());
  ASSERT_EQ(2047, path.back());
  path.clear();
  g->shortest_path_length(false, 0, 2047, path);
  ASSERT_EQ(12, path.size());
  ASSERT_EQ(0, path.front());
  ASSERT_EQ(2047, path.back());
  delete g;
}

// shortest-path test
TEST(AdjacencyListTest, ShortestPathMediumSized2dGridGraph) {
  int size = 2500;
  string filename = "examples/grid-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  list<int> path;
  g->shortest_path_length(true, 0, 2499, path);
  ASSERT_EQ(99, path.size());
  ASSERT_EQ(0, path.front());
  ASSERT_EQ(2499, path.back());
  path.clear();
  g->shortest_path_length(false, 0, 2499, path);
  ASSERT_EQ(99, path.size());
  ASSERT_EQ(0, path.front());
  ASSERT_EQ(2499, path.back());
  delete g;
}

// shortest-path test
TEST(AdjacencyListTest, ShortestPathMediumSizedBarbellGraph) {
  int size = 2500;
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  list<int> path;
  g->shortest_path_length(true, 0, 2499, path);
  ASSERT_EQ(6, path.size());
  ASSERT_EQ(0, path.front());
  ASSERT_EQ(2499, path.back());
  path.clear();
  g->shortest_path_length(false, 0, 2499, path);
  ASSERT_EQ(6, path.size());
  ASSERT_EQ(0, path.front());
  ASSERT_EQ(2499, path.back());
  delete g;
}


// connected components test
TEST(AdjacencyListTest, ConnectedComponentsMediumSizedRandomGraph) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map components;
  g->bfs_connected_components(components);
  ASSERT_EQ(vertices, components.size());
  ASSERT_EQ(1948, connected_component_size(components, 2));
  delete g;
}

// connected components test
TEST(AdjacencyListTest, ConnectedComponentsMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map components;
  g->bfs_connected_components(components);
  ASSERT_EQ(vertices, components.size());
  ASSERT_EQ(2048, connected_component_size(components, 0));
  delete g;
}

// connected components test
TEST(AdjacencyListTest, ConnectedComponentsMediumSized2dGridGraph) {
  int size = 2500;
  string filename = "examples/grid-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map components;
  g->bfs_connected_components(components);
  ASSERT_EQ(vertices, components.size());
  ASSERT_EQ(2500, connected_component_size(components, 0));
  delete g;
}

// connected components test
TEST(AdjacencyListTest, ConnectedComponentsMediumSizedBarbellGraph) {
  int size = 2500;
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map components;
  g->bfs_connected_components(components);
  ASSERT_EQ(vertices, components.size());
  ASSERT_EQ(2500, connected_component_size(components, 0));
  delete g;
}


// bipartite test
TEST(AdjacencyListTest, BipartiteMediumSizedRandomGraph) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map coloring;
  ASSERT_EQ(false, g->bipartite_graph(coloring));
  delete g;
}

// bipartite test
TEST(AdjacencyListTest, BipartiteMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map coloring;
  ASSERT_EQ(true, g->bipartite_graph(coloring));
  delete g;
}

// bipartite test
TEST(AdjacencyListTest, BipartiteMediumSized2dGridGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map coloring;
  ASSERT_EQ(true, g->bipartite_graph(coloring));
  delete g;
}

// bipartite test
TEST(AdjacencyListTest, BipartiteMediumSizedBarbellGraph) {
  int size = 2500;
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  Map coloring;
  ASSERT_EQ(false, g->bipartite_graph(coloring));
  delete g;
}



int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

