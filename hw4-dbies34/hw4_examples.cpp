//----------------------------------------------------------------------
// Name: S. Bowers
// File: hw4_examples.cpp
// Date: Spring 2021
// Desc: Tests for example graphs
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

bool subset(const list<int>& s1, const list<int>& s2)
{
  for (int x : s1) {
    bool contained_in = false;
    for (int y : s2) {
      if (x == y)
        contained_in = true;
    }
    if (!contained_in)
      return false;
  }
  return true;
}

bool valid_topological_sort(const Graph& g, const Map& ordering_map)
{
  int n = ordering_map.size();
  // create list with ordering
  int node_order[n];
  for (int i = 0; i < n; ++i)
    node_order[ordering_map.at(i)-1] = i;
  // ...
  list<int> checked;
  for (int i = n-1; i >= 0; i--) {
    int v = node_order[i];
    list<int> vertices;
    g.connected_to(v, vertices);
    if (!subset(vertices, checked))
      return false;
    checked.push_back(v);
  }
  return true;
}



//----------------------------------------------------------------------
// Adjacency Matrix Tests
//----------------------------------------------------------------------

// DFS test
TEST(AdjacencyMatrixTest, DFSMediumSizedRandomGraph) {
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
  g->dfs(true, 1918, search_tree);
  ASSERT_EQ(347, search_tree.size());
  search_tree.clear();
  g->dfs(true, 0, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->dfs(false, 2, search_tree);
  ASSERT_EQ(1948, search_tree.size());
  search_tree.clear();
  g->dfs(false, 0, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  delete g;
}


// DFS test
TEST(AdjacencyMatrixTest, DFSMediumSizedHypercubeGraph) {
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
  g->dfs(true, 0, search_tree);
  ASSERT_EQ(2048, search_tree.size());
  search_tree.clear();
  g->dfs(true, 2047, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  search_tree.clear();
  // undirected (largest search tree)
  g->dfs(false, 0, search_tree);
  ASSERT_EQ(2048, search_tree.size());
  delete g;
}

// DFS test
TEST(AdjacencyMatrixTest, DFSMediumSized2dGridGraph) {
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
  g->dfs(true, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->dfs(false, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  delete g;
}


// BFS test
TEST(AdjacencyMatrixTest, DFSMediumSizedBarbellGraph) {
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
  g->dfs(true, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  search_tree.clear();  
  g->dfs(true, 2499, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->dfs(false, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  delete g;
}


// cycle check
TEST(AdjacencyMatrixTest, CycleCheckMediumSizedRandomGraph) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // directly cycle check
  ASSERT_EQ(false, g->acyclic(true));
  // undirected cycle check
  ASSERT_EQ(false, g->acyclic(false));  
  delete g;
}


// cycle check
TEST(AdjacencyMatrixTest, CycleCheckMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // directly cycle check
  ASSERT_EQ(true, g->acyclic(true));
  // undirected cycle check
  ASSERT_EQ(false, g->acyclic(false));  
  delete g;
}

// cycle check
TEST(AdjacencyMatrixTest, CycleCheckMediumSized2dGridGraph) {
  int size = 2500;
  string filename = "examples/grid-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // directly cycle check
  ASSERT_EQ(false, g->acyclic(true));
  // undirected cycle check
  ASSERT_EQ(false, g->acyclic(false));  
  delete g;
}

// cycle check
TEST(AdjacencyMatrixTest, CycleCheckMediumSizedBarbellGraph) {
  int size = 2500;
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // directly cycle check
  ASSERT_EQ(true, g->acyclic(true));
  // undirected cycle check
  ASSERT_EQ(false, g->acyclic(false));  
  delete g;
}


// transitive closure
TEST(AdjacencyMatrixTest, TransitiveClosureMediumSizedRandomGraph) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  AdjacencyMatrix* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // copy g and call tc as directed graph
  AdjacencyMatrix* dir_tc_g = new AdjacencyMatrix(*g);
  g->unweighted_transitive_closure(true, *dir_tc_g);
  list<int> tc_vertices;
  dir_tc_g->connected_to(1918, tc_vertices);
  ASSERT_EQ(346, tc_vertices.size());
  ASSERT_EQ(true, dir_tc_g->has_edge(1918, 2487));
  delete dir_tc_g;
  delete g;
}

// transitive closure
TEST(AdjacencyMatrixTest, TransitiveClosureMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  AdjacencyMatrix* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // copy g and call tc as directed graph
  AdjacencyMatrix* dir_tc_g = new AdjacencyMatrix(*g);
  g->unweighted_transitive_closure(true, *dir_tc_g);
  list<int> tc_vertices;
  dir_tc_g->connected_to(0, tc_vertices);
  ASSERT_EQ(2047, tc_vertices.size());
  ASSERT_EQ(true, dir_tc_g->has_edge(0, 2047));
  delete dir_tc_g;
  delete g;
}

// transitive closure
TEST(AdjacencyMatrixTest, TransitiveClosureMediumSized2dGridGraph) {
  int size = 1000; // 2500 takes too long!
  string filename = "examples/grid-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  AdjacencyMatrix* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // copy g and call tc as directed graph
  AdjacencyMatrix* dir_tc_g = new AdjacencyMatrix(*g);
  g->unweighted_transitive_closure(true, *dir_tc_g);
  list<int> tc_vertices;
  dir_tc_g->connected_to(0, tc_vertices);
  ASSERT_EQ(960, tc_vertices.size());
  ASSERT_EQ(true, dir_tc_g->has_edge(0, 960));
  delete dir_tc_g;
  delete g;
}

// transitive closure
TEST(AdjacencyMatrixTest, TransitiveClosureMediumSizedBarbellGraph) {
  int size = 500; // 2500 takes too long!
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  AdjacencyMatrix* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // copy g and call tc as directed graph
  AdjacencyMatrix* dir_tc_g = new AdjacencyMatrix(*g);
  g->unweighted_transitive_closure(true, *dir_tc_g);
  list<int> tc_vertices;
  dir_tc_g->connected_to(0, tc_vertices);
  ASSERT_EQ(499, tc_vertices.size());
  ASSERT_EQ(true, dir_tc_g->has_edge(0, 499));
  delete dir_tc_g;
  delete g;
}


// topological sort
TEST(AdjacencyMatrixTest, TopologicalSortMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // computer ordering and check if valid
  Map ordering;
  g->dfs_topological_sort(ordering);
  ASSERT_EQ(vertices, ordering.size());
  ASSERT_EQ(true, valid_topological_sort(*g, ordering));
  delete g;
}

// topological sort
TEST(AdjacencyMatrixTest, TopologicalSortMediumSizedBarbellGraph) {
  int size = 500; // 2500 takes too long!
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  AdjacencyMatrix* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // computer ordering and check if valid
  Map ordering;
  g->dfs_topological_sort(ordering);
  ASSERT_EQ(vertices, ordering.size());
  ASSERT_EQ(true, valid_topological_sort(*g, ordering));
  delete g;
}




//----------------------------------------------------------------------
// Adjacency List Tests
//----------------------------------------------------------------------

// DFS test
TEST(AdjacencyListTest, DFSMediumSizedRandomGraph) {
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
  g->dfs(true, 1918, search_tree);
  ASSERT_EQ(347, search_tree.size());
  search_tree.clear();
  g->dfs(true, 0, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->dfs(false, 2, search_tree);
  ASSERT_EQ(1948, search_tree.size());
  search_tree.clear();
  g->dfs(false, 0, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  delete g;
}

// DFS test
TEST(AdjacencyListTest, DFSMediumSizedHypercubeGraph) {
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
  g->dfs(true, 0, search_tree);
  ASSERT_EQ(2048, search_tree.size());
  search_tree.clear();
  g->dfs(true, 2047, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->dfs(false, 0, search_tree);
  ASSERT_EQ(2048, search_tree.size());
  delete g;
}

// DFS test
TEST(AdjacencyListTest, DFSMediumSized2dGridGraph) {
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
  g->dfs(true, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->dfs(false, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  delete g;
}

// DFS test
TEST(AdjacencyListTest, DFSMediumSizedBarbellGraph) {
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
  g->dfs(true, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  search_tree.clear();  
  g->dfs(true, 2499, search_tree);
  ASSERT_EQ(1, search_tree.size());  
  search_tree.clear();
  // undirected (largest and smallest search tree)
  g->dfs(false, 0, search_tree);
  ASSERT_EQ(2500, search_tree.size());
  delete g;
}


// cycle check
TEST(AdjacencyListTest, CycleCheckMediumSizedRandomGraph) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // directly cycle check
  ASSERT_EQ(false, g->acyclic(true));
  // undirected cycle check
  ASSERT_EQ(false, g->acyclic(false));  
  delete g;
}

// cycle check
TEST(AdjacencyListTest, CycleCheckMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // directly cycle check
  ASSERT_EQ(true, g->acyclic(true));
  // undirected cycle check
  ASSERT_EQ(false, g->acyclic(false));  
  delete g;
}

// cycle check
TEST(AdjacencyListTest, CycleCheckMediumSized2dGridGraph) {
  int size = 2500;
  string filename = "examples/grid-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // directly cycle check
  ASSERT_EQ(false, g->acyclic(true));
  // undirected cycle check
  ASSERT_EQ(false, g->acyclic(false));  
  delete g;
}

// cycle check
TEST(AdjacencyListTest, CycleCheckMediumSizedBarbellGraph) {
  int size = 2500;
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // directly cycle check
  ASSERT_EQ(true, g->acyclic(true));
  // undirected cycle check
  ASSERT_EQ(false, g->acyclic(false));  
  delete g;
}


// transitive closure
TEST(AdjacencyListTest, TransitiveClosureMediumSizedRandomGraph) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  AdjacencyList* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // copy g and call tc as directed graph
  AdjacencyList* dir_tc_g = new AdjacencyList(*g);
  g->unweighted_transitive_closure(true, *dir_tc_g);
  list<int> tc_vertices;
  dir_tc_g->connected_to(1918, tc_vertices);
  ASSERT_EQ(346, tc_vertices.size());
  ASSERT_EQ(true, dir_tc_g->has_edge(1918, 2487));
  delete dir_tc_g;
  delete g;
}

// transitive closure
TEST(AdjacencyListTest, TransitiveClosureMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  AdjacencyList* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // copy g and call tc as directed graph
  AdjacencyList* dir_tc_g = new AdjacencyList(*g);
  g->unweighted_transitive_closure(true, *dir_tc_g);
  list<int> tc_vertices;
  dir_tc_g->connected_to(0, tc_vertices);
  ASSERT_EQ(2047, tc_vertices.size());
  ASSERT_EQ(true, dir_tc_g->has_edge(0, 2047));
  delete dir_tc_g;
  delete g;
}

// transitive closure
TEST(AdjacencyListTest, TransitiveClosureMediumSized2dGridGraph) {
  int size = 1000; // 2500 takes too long!
  string filename = "examples/grid-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  AdjacencyList* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // copy g and call tc as directed graph
  AdjacencyList* dir_tc_g = new AdjacencyList(*g);
  g->unweighted_transitive_closure(true, *dir_tc_g);
  list<int> tc_vertices;
  dir_tc_g->connected_to(0, tc_vertices);
  ASSERT_EQ(960, tc_vertices.size());
  ASSERT_EQ(true, dir_tc_g->has_edge(0, 960));
  delete dir_tc_g;
  delete g;
}

// transitive closure
TEST(AdjacencyListTest, TransitiveClosureMediumSizedBarbellGraph) {
  int size = 500; // 2500 takes too long!
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  AdjacencyList* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // copy g and call tc as directed graph
  AdjacencyList* dir_tc_g = new AdjacencyList(*g);
  g->unweighted_transitive_closure(true, *dir_tc_g);
  list<int> tc_vertices;
  dir_tc_g->connected_to(0, tc_vertices);
  ASSERT_EQ(499, tc_vertices.size());
  ASSERT_EQ(true, dir_tc_g->has_edge(0, 499));
  delete dir_tc_g;
  delete g;
}


// topological sort
TEST(AdjacencyListTest, TopologicalSortMediumSizedHypercubeGraph) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // computer ordering and check if valid
  Map ordering;
  g->dfs_topological_sort(ordering);
  ASSERT_EQ(vertices, ordering.size());
  ASSERT_EQ(true, valid_topological_sort(*g, ordering));
  delete g;
}

// topological sort
TEST(AdjacencyListTest, TopologicalSortMediumSizedBarbellGraph) {
  int size = 500; // 2500 takes too long!
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  AdjacencyList* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_LT(0, g->vertex_count());
  ASSERT_LT(0, g->edge_count());
  ASSERT_EQ(edges, g->edge_count());
  // computer ordering and check if valid
  Map ordering;
  g->dfs_topological_sort(ordering);
  ASSERT_EQ(vertices, ordering.size());
  ASSERT_EQ(true, valid_topological_sort(*g, ordering));
  delete g;
}



int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

