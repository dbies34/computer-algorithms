//----------------------------------------------------------------------
// Name: Drew Bies
// File: hw4_tests.cpp
// Date: Spring 2021
// Desc: Unit tests for DFS and related algorithms
//----------------------------------------------------------------------


#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "graph.h"
#include "adjacency_list.h"
#include "adjacency_matrix.h"

using namespace std;


//----------------------------------------------------------------------
// Helper functions for testing
//----------------------------------------------------------------------

void print_graph(const Graph& g)
{
  int n = g.vertex_count();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (g.has_edge(i,j)) {
        int edge_label;
        g.get_edge(i, j, edge_label);
        cout << "(" << i << "," << edge_label << "," << j << ")" << endl;
      }
    }
  }
}

void print_search_tree(const Map& search_tree)
{
  for (pair<int,int> p : search_tree)
    cout << "parent: " << p.second << ", child: " << p.first << endl;
}

void print_path(const list<int>& path)
{
  for (int v : path)
    cout << v << " ";
  cout << endl;
}

void print_ordering(const Map& ordering)
{
  for (pair<int,int> p : ordering)
    cout << p.first << " has order " << p.second << endl;
}


//----------------------------------------------------------------------
// Adjacency List Tests
//----------------------------------------------------------------------

// Basic DFS test over a directed graph
TEST(AdjacencyListTest, BasicDirectedDFS) {
  AdjacencyList g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(0, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(1, 0, 0);
  g.set_edge(3, 0, 0);
  Map tree;
  // directed dfs from 0
  g.dfs(true, 0, tree);
  // all nodes found
  ASSERT_EQ(g.vertex_count(), tree.size());
  // check parents
  ASSERT_EQ(-1, tree[0]);
  ASSERT_EQ(0, tree[1]);
  ASSERT_EQ(0, tree[2]);
  ASSERT_EQ(2, tree[3]);
  // directed dfs from 1
  tree.clear();
  g.dfs(true, 1, tree);
  ASSERT_EQ(4, tree.size());
  // all nodes found
  ASSERT_EQ(g.vertex_count(), tree.size());
  // check parents
  ASSERT_EQ(-1, tree[1]);
  ASSERT_EQ(1, tree[0]);
  ASSERT_EQ(0, tree[2]);
  ASSERT_EQ(2, tree[3]);
}


// 2nd DFS test over a directed graph
TEST(AdjacencyListTest, DisconnectedDirectedDFS) {
  AdjacencyList g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(0, 0, 2);
  g.set_edge(1, 0, 0);
  Map tree;
  // directed dfs from 0
  g.dfs(true, 0, tree);
  // 3 nodes found
  ASSERT_EQ(3, tree.size());
  // check parents
  ASSERT_EQ(-1, tree[0]);
  ASSERT_EQ(0, tree[1]);
  ASSERT_EQ(0, tree[2]);
}

// 3rd DFS test over a directed graph
TEST(AdjacencyListTest, BackwardsDirectedDFS) {
  AdjacencyList g(4);
  g.set_edge(3, 0, 2);
  g.set_edge(2, 0, 1);
  g.set_edge(1, 0, 0);
  Map tree;
  // directed dfs from 0
  g.dfs(true, 0, tree);
  // 1 nodes found
  ASSERT_EQ(1, tree.size());
  // check parents
  ASSERT_EQ(-1, tree[0]);
}

// Basic undirected graph DFS test
TEST(AdjacencyListTest, BasicUndirectedDFS) {
  AdjacencyList g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(0, 0, 3);
  g.set_edge(1, 0, 2);
  ASSERT_EQ(true, g.acyclic(false));
  g.set_edge(3, -1, 2);
  ASSERT_EQ(false, g.acyclic(false));
}

// 2nd undirected graph DFS tests
TEST(AdjacencyListTest, BackwardsUndirectedDFS) {
  AdjacencyList g(4);
  g.set_edge(3, 0, 2);
  g.set_edge(2, 0, 1);
  g.set_edge(1, 0, 0);
  Map tree;
  // directed dfs from 0
  g.dfs(false, 0, tree);
  // 1 nodes found
  ASSERT_EQ(4, tree.size());
  // check parents
  ASSERT_EQ(-1, tree[0]);
  ASSERT_EQ(0, tree[1]);
  ASSERT_EQ(1, tree[2]);
  ASSERT_EQ(2, tree[3]);

  ASSERT_EQ(true, g.acyclic(false));
}

// 3rd undirected graph DFS test
TEST(AdjacencyListTest, DisconnectedUndirectedDFS) {
  AdjacencyList g(8);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(4, 0, 5);
  g.set_edge(5, 0, 6);
  g.set_edge(4, 0, 7);
  Map tree;
  g.dfs(false, 0, tree);
  // 4 nodes found
  ASSERT_EQ(4, tree.size());
  ASSERT_EQ(-1, tree[0]);
  ASSERT_EQ(0, tree[1]);
  ASSERT_EQ(1, tree[2]);
  ASSERT_EQ(2, tree[3]);
  tree.clear();
  g.dfs(false, 4, tree);
  // 4 nodes found
  ASSERT_EQ(4, tree.size());
  ASSERT_EQ(-1, tree[4]);
  ASSERT_EQ(4, tree[5]);
  ASSERT_EQ(5, tree[6]);
  ASSERT_EQ(4, tree[7]);
}

// Basic directed cycle check test
TEST(AdjacencyListTest, BasicDFSDirectedCycleCheck) {
  AdjacencyList g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(0, 0, 3);
  ASSERT_EQ(true, g.acyclic(true));
  g.set_edge(2, 0, 0);
  ASSERT_EQ(false, g.acyclic(true));
}

// 2nd directed cycle check test, one graphs of two nodes
TEST(AdjacencyListTest, TwoNodeDFSDirectedCycleCheck) {
  AdjacencyList g(2);
  g.set_edge(0, 0, 1);
  ASSERT_EQ(true, g.acyclic(true));
  g.set_edge(1, 0, 0);
  ASSERT_EQ(false, g.acyclic(true));
}

// 3rd directed cycle check test, two disconnected graphs
TEST(AdjacencyListTest, DisconnectDFSDirectedCycleCheck) {
  AdjacencyList g(8);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(4, 0, 5);
  g.set_edge(5, 0, 6);
  g.set_edge(4, 0, 7);
  ASSERT_EQ(true, g.acyclic(true));
  g.set_edge(3, 0, 0);
  ASSERT_EQ(false, g.acyclic(true));
}

// Basic undirected cycle check test
TEST(AdjacencyListTest, BasicDFSUndirectedCycleCheck) {
  AdjacencyList g(3);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  ASSERT_EQ(true, g.acyclic(false));
  g.set_edge(2, 0, 0);
  ASSERT_EQ(false, g.acyclic(false));
}

// 2nd undirected cycle check test
TEST(AdjacencyListTest, LongDFSUndirectedCycleCheck) {
  AdjacencyList g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  ASSERT_EQ(true, g.acyclic(false));
  g.set_edge(3, 0, 1);
  ASSERT_EQ(false, g.acyclic(false));
}

// 3rd undirected cycle check test
TEST(AdjacencyListTest, DisconnectedDFSUndirectedCycleCheck) {
  AdjacencyList g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  ASSERT_EQ(true, g.acyclic(false));
  g.set_edge(4, 0, 2);
  ASSERT_EQ(false, g.acyclic(false));
}

// Basic directed transitive closure test
TEST(AdjacencyListTest, BasicDirectedUnweightedDFSTransClosure) {
  AdjacencyList g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(1, 0, 3);
  g.set_edge(3, 0, 4);
  AdjacencyList tc_g(g);
  g.unweighted_transitive_closure(true, tc_g);
  ASSERT_EQ(true, tc_g.has_edge(0, 2));
  ASSERT_EQ(true, tc_g.has_edge(0, 3));
  ASSERT_EQ(true, tc_g.has_edge(0, 4));
  ASSERT_EQ(true, tc_g.has_edge(1, 4));
}

// 2nd directed transitive closure test
TEST(AdjacencyListTest, DirectedUnweightedDFSTransClosure) {
  AdjacencyList g(6);
  g.set_edge(4, 0, 0);
  g.set_edge(5, 0, 0);
  g.set_edge(4, 0, 2);
  g.set_edge(5, 0, 1);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 1);
  AdjacencyList tc_g(g);
  g.unweighted_transitive_closure(true, tc_g);
  ASSERT_EQ(true, tc_g.has_edge(4, 3));
  ASSERT_EQ(true, tc_g.has_edge(4, 1));
}

// 3rd directed transitive closure test
TEST(AdjacencyListTest, LongDirectedUnweightedDFSTransClosure) {
  AdjacencyList g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  AdjacencyList tc_g(g);
  g.unweighted_transitive_closure(true, tc_g);
  ASSERT_EQ(true, tc_g.has_edge(0, 2));
  ASSERT_EQ(true, tc_g.has_edge(0, 3));
  ASSERT_EQ(true, tc_g.has_edge(0, 4));
  ASSERT_EQ(true, tc_g.has_edge(0, 5));
  ASSERT_EQ(true, tc_g.has_edge(1, 4));
  ASSERT_EQ(true, tc_g.has_edge(1, 2));
  ASSERT_EQ(true, tc_g.has_edge(1, 3));
  ASSERT_EQ(true, tc_g.has_edge(1, 4));
}

// Basic undirected transitive closure test
TEST(AdjacencyListTest, UndirectedUnweightedDFSTransClosure) {
  AdjacencyList g(3);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  AdjacencyList tc_g(g);
  g.unweighted_transitive_closure(false, tc_g);
  ASSERT_EQ(true, tc_g.has_edge(0, 2) || tc_g.has_edge(2, 0));
}

// 2nd undirected graph transitive closure tests
TEST(AdjacencyListTest, BasicUnirectedUnweightedDFSTransClosure) {
  AdjacencyList g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(1, 0, 3);
  g.set_edge(3, 0, 4);
  AdjacencyList tc_g(g);
  g.unweighted_transitive_closure(false, tc_g);
  ASSERT_EQ(true, tc_g.has_edge(0, 2));
  ASSERT_EQ(true, tc_g.has_edge(0, 3));
  ASSERT_EQ(true, tc_g.has_edge(0, 4));
  ASSERT_EQ(true, tc_g.has_edge(1, 4));
}

// 3rd undirected graph transitive closure tests
TEST(AdjacencyListTest, DisconnectedUnirectedUnweightedDFSTransClosure) {
  AdjacencyList g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(1, 0, 3);
  AdjacencyList tc_g(g);
  g.unweighted_transitive_closure(false, tc_g);
  ASSERT_EQ(true, tc_g.has_edge(0, 2));
  ASSERT_EQ(true, tc_g.has_edge(0, 3));
  ASSERT_EQ(false, tc_g.has_edge(0, 4));
  ASSERT_EQ(false, tc_g.has_edge(1, 4));
}

// Basic topological sort test
TEST(AdjacencyListTest, BasicDFSTopologicalSort) {
  AdjacencyList g1(5);
  g1.set_edge(0, 0, 2);
  g1.set_edge(1, 0, 2);
  g1.set_edge(2, 0, 3);
  g1.set_edge(2, 0, 4);
  g1.set_edge(3, 0, 4);
  Map vertex_ordering;
  g1.dfs_topological_sort(vertex_ordering);
  ASSERT_LE(vertex_ordering[3], vertex_ordering[4]);
  ASSERT_LE(vertex_ordering[2], vertex_ordering[3]);
  ASSERT_LE(vertex_ordering[1], vertex_ordering[2]);
  ASSERT_LE(vertex_ordering[0], vertex_ordering[2]);
  AdjacencyList g2(5);
  g2.set_edge(0, 0, 2);
  g2.set_edge(1, 0, 2);
  g2.set_edge(2, 0, 3);
  g2.set_edge(2, 0, 4);
  g2.set_edge(4, 0, 3);
  vertex_ordering.clear();
  g2.dfs_topological_sort(vertex_ordering);
  ASSERT_LE(vertex_ordering[4], vertex_ordering[3]);
  ASSERT_LE(vertex_ordering[2], vertex_ordering[3]);
  ASSERT_LE(vertex_ordering[1], vertex_ordering[2]);
  ASSERT_LE(vertex_ordering[0], vertex_ordering[2]);
  
}

// 2nd topological sort test, where 1 and 0 are not the lowest values
TEST(AdjacencyListTest, DFSTopologicalSort) {
  AdjacencyList g(6);
  g.set_edge(4, 0, 0);
  g.set_edge(5, 0, 0);
  g.set_edge(4, 0, 2);
  g.set_edge(5, 0, 1);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 1);
  Map vertex_ordering;
  g.dfs_topological_sort(vertex_ordering);
  ASSERT_LE(vertex_ordering[4], vertex_ordering[2]);
  ASSERT_LE(vertex_ordering[5], vertex_ordering[3]);
  ASSERT_LE(vertex_ordering[3], vertex_ordering[1]);
  ASSERT_LE(vertex_ordering[2], vertex_ordering[0]);
}


// 3rd topological sorting test, where the graph is split
TEST(AdjacencyListTest, DisconnectedDFSTopologicalSort) {
  AdjacencyList g(6);
  g.set_edge(3, 0, 4);
  g.set_edge(5, 0, 4);
  g.set_edge(0, 0, 1);
  g.set_edge(2, 0, 0);
  Map vertex_ordering;
  g.dfs_topological_sort(vertex_ordering);
  ASSERT_LE(vertex_ordering[3], vertex_ordering[4]);
  ASSERT_LE(vertex_ordering[5], vertex_ordering[3]);
  ASSERT_LE(vertex_ordering[0], vertex_ordering[1]);
  ASSERT_LE(vertex_ordering[2], vertex_ordering[0]);
}

//----------------------------------------------------------------------
// Adjacency Matrix Tests
//----------------------------------------------------------------------

// Basic DFS test over a directed graph
TEST(AdjacencyMatrixTest, BasicDirectedDFS) {
  AdjacencyMatrix g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(0, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(1, 0, 0);
  g.set_edge(3, 0, 0);
  Map tree;
  // directed dfs from 0
  g.dfs(true, 0, tree);
  // all nodes found
  ASSERT_EQ(g.vertex_count(), tree.size());
  // check parents
  ASSERT_EQ(-1, tree[0]);
  ASSERT_EQ(0, tree[1]);
  ASSERT_EQ(0, tree[2]);
  ASSERT_EQ(2, tree[3]);
  // directed dfs from 1
  tree.clear();
  g.dfs(true, 1, tree);
  ASSERT_EQ(4, tree.size());
  // all nodes found
  ASSERT_EQ(g.vertex_count(), tree.size());
  // check parents
  ASSERT_EQ(-1, tree[1]);
  ASSERT_EQ(1, tree[0]);
  ASSERT_EQ(0, tree[2]);
  ASSERT_EQ(2, tree[3]);
}


// 2nd DFS test over a directed graph
TEST(AdjacencyMatrixTest, DisconnectedDirectedDFS) {
  AdjacencyMatrix g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(0, 0, 2);
  g.set_edge(1, 0, 0);
  Map tree;
  // directed dfs from 0
  g.dfs(true, 0, tree);
  // 3 nodes found
  ASSERT_EQ(3, tree.size());
  // check parents
  ASSERT_EQ(-1, tree[0]);
  ASSERT_EQ(0, tree[1]);
  ASSERT_EQ(0, tree[2]);
}

// 3rd DFS test over a directed graph
TEST(AdjacencyMatrixTest, BackwardsDirectedDFS) {
  AdjacencyMatrix g(4);
  g.set_edge(3, 0, 2);
  g.set_edge(2, 0, 1);
  g.set_edge(1, 0, 0);
  Map tree;
  // directed dfs from 0
  g.dfs(true, 0, tree);
  // 1 nodes found
  ASSERT_EQ(1, tree.size());
  // check parents
  ASSERT_EQ(-1, tree[0]);
}

// Basic undirected graph DFS test
TEST(AdjacencyMatrixTest, BasicUndirectedDFS) {
  AdjacencyMatrix g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(0, 0, 3);
  g.set_edge(1, 0, 2);
  ASSERT_EQ(true, g.acyclic(false));
  g.set_edge(3, -1, 2);
  ASSERT_EQ(false, g.acyclic(false));
}

// 2nd undirected graph DFS tests
TEST(AdjacencyMatrixTest, BackwardsUndirectedDFS) {
  AdjacencyMatrix g(4);
  g.set_edge(3, 0, 2);
  g.set_edge(2, 0, 1);
  g.set_edge(1, 0, 0);
  Map tree;
  // directed dfs from 0
  g.dfs(false, 0, tree);
  // 1 nodes found
  ASSERT_EQ(4, tree.size());
  // check parents
  ASSERT_EQ(-1, tree[0]);
  ASSERT_EQ(0, tree[1]);
  ASSERT_EQ(1, tree[2]);
  ASSERT_EQ(2, tree[3]);

  ASSERT_EQ(true, g.acyclic(false));
}

// 3rd undirected graph DFS test
TEST(AdjacencyMatrixTest, DisconnectedUndirectedDFS) {
  AdjacencyMatrix g(8);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(4, 0, 5);
  g.set_edge(5, 0, 6);
  g.set_edge(4, 0, 7);
  Map tree;
  g.dfs(false, 0, tree);
  // 4 nodes found
  ASSERT_EQ(4, tree.size());
  ASSERT_EQ(-1, tree[0]);
  ASSERT_EQ(0, tree[1]);
  ASSERT_EQ(1, tree[2]);
  ASSERT_EQ(2, tree[3]);
  tree.clear();
  g.dfs(false, 4, tree);
  // 4 nodes found
  ASSERT_EQ(4, tree.size());
  ASSERT_EQ(-1, tree[4]);
  ASSERT_EQ(4, tree[5]);
  ASSERT_EQ(5, tree[6]);
  ASSERT_EQ(4, tree[7]);
}

// Basic directed cycle check test
TEST(AdjacencyMatrixTest, BasicDFSDirectedCycleCheck) {
  AdjacencyMatrix g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(0, 0, 3);
  ASSERT_EQ(true, g.acyclic(true));
  g.set_edge(2, 0, 0);
  ASSERT_EQ(false, g.acyclic(true));
}

// 2nd directed cycle check test, one graphs of two nodes
TEST(AdjacencyMatrixTest, TwoNodeDFSDirectedCycleCheck) {
  AdjacencyMatrix g(2);
  g.set_edge(0, 0, 1);
  ASSERT_EQ(true, g.acyclic(true));
  g.set_edge(1, 0, 0);
  ASSERT_EQ(false, g.acyclic(true));
}

// 3rd directed cycle check test, two disconnected graphs
TEST(AdjacencyMatrixTest, DisconnectDFSDirectedCycleCheck) {
  AdjacencyMatrix g(8);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(4, 0, 5);
  g.set_edge(5, 0, 6);
  g.set_edge(4, 0, 7);
  ASSERT_EQ(true, g.acyclic(true));
  g.set_edge(3, 0, 0);
  ASSERT_EQ(false, g.acyclic(true));
}

// Basic undirected cycle check test
TEST(AdjacencyMatrixTest, BasicDFSUndirectedCycleCheck) {
  AdjacencyMatrix g(3);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  ASSERT_EQ(true, g.acyclic(false));
  g.set_edge(2, 0, 0);
  ASSERT_EQ(false, g.acyclic(false));
}

// 2nd undirected cycle check test
TEST(AdjacencyMatrixTest, LongDFSUndirectedCycleCheck) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  ASSERT_EQ(true, g.acyclic(false));
  g.set_edge(3, 0, 1);
  ASSERT_EQ(false, g.acyclic(false));
}

// 3rd undirected cycle check test
TEST(AdjacencyMatrixTest, DisconnectedDFSUndirectedCycleCheck) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  ASSERT_EQ(true, g.acyclic(false));
  g.set_edge(4, 0, 2);
  ASSERT_EQ(false, g.acyclic(false));
}

// Basic directed transitive closure test
TEST(AdjacencyMatrixTest, BasicDirectedUnweightedDFSTransClosure) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(1, 0, 3);
  g.set_edge(3, 0, 4);
  AdjacencyMatrix tc_g(g);
  g.unweighted_transitive_closure(true, tc_g);
  ASSERT_EQ(true, tc_g.has_edge(0, 2));
  ASSERT_EQ(true, tc_g.has_edge(0, 3));
  ASSERT_EQ(true, tc_g.has_edge(0, 4));
  ASSERT_EQ(true, tc_g.has_edge(1, 4));
}

// 2nd directed transitive closure test
TEST(AdjacencyMatrixTest, DirectedUnweightedDFSTransClosure) {
  AdjacencyMatrix g(6);
  g.set_edge(4, 0, 0);
  g.set_edge(5, 0, 0);
  g.set_edge(4, 0, 2);
  g.set_edge(5, 0, 1);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 1);
  AdjacencyMatrix tc_g(g);
  g.unweighted_transitive_closure(true, tc_g);
  ASSERT_EQ(true, tc_g.has_edge(4, 3));
  ASSERT_EQ(true, tc_g.has_edge(4, 1));
}

// 3rd directed transitive closure test
TEST(AdjacencyMatrixTest, LongDirectedUnweightedDFSTransClosure) {
  AdjacencyMatrix g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  AdjacencyMatrix tc_g(g);
  g.unweighted_transitive_closure(true, tc_g);
  ASSERT_EQ(true, tc_g.has_edge(0, 2));
  ASSERT_EQ(true, tc_g.has_edge(0, 3));
  ASSERT_EQ(true, tc_g.has_edge(0, 4));
  ASSERT_EQ(true, tc_g.has_edge(0, 5));
  ASSERT_EQ(true, tc_g.has_edge(1, 4));
  ASSERT_EQ(true, tc_g.has_edge(1, 2));
  ASSERT_EQ(true, tc_g.has_edge(1, 3));
  ASSERT_EQ(true, tc_g.has_edge(1, 4));
}

// Basic undirected transitive closure test
TEST(AdjacencyMatrixTest, UndirectedUnweightedDFSTransClosure) {
  AdjacencyMatrix g(3);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  AdjacencyMatrix tc_g(g);
  g.unweighted_transitive_closure(false, tc_g);
  ASSERT_EQ(true, tc_g.has_edge(0, 2) || tc_g.has_edge(2, 0));
}

// 2nd undirected graph transitive closure tests
TEST(AdjacencyMatrixTest, BasicUnirectedUnweightedDFSTransClosure) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(1, 0, 3);
  g.set_edge(3, 0, 4);
  AdjacencyMatrix tc_g(g);
  g.unweighted_transitive_closure(false, tc_g);
  ASSERT_EQ(true, tc_g.has_edge(0, 2));
  ASSERT_EQ(true, tc_g.has_edge(0, 3));
  ASSERT_EQ(true, tc_g.has_edge(0, 4));
  ASSERT_EQ(true, tc_g.has_edge(1, 4));
}

// 3rd undirected graph transitive closure tests
TEST(AdjacencyMatrixTest, DisconnectedUnirectedUnweightedDFSTransClosure) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(1, 0, 3);
  AdjacencyMatrix tc_g(g);
  g.unweighted_transitive_closure(false, tc_g);
  ASSERT_EQ(true, tc_g.has_edge(0, 2));
  ASSERT_EQ(true, tc_g.has_edge(0, 3));
  ASSERT_EQ(false, tc_g.has_edge(0, 4));
  ASSERT_EQ(false, tc_g.has_edge(1, 4));
}

// Basic topological sort test
TEST(AdjacencyMatrixTest, BasicDFSTopologicalSort) {
  AdjacencyMatrix g1(5);
  g1.set_edge(0, 0, 2);
  g1.set_edge(1, 0, 2);
  g1.set_edge(2, 0, 3);
  g1.set_edge(2, 0, 4);
  g1.set_edge(3, 0, 4);
  Map vertex_ordering;
  g1.dfs_topological_sort(vertex_ordering);
  ASSERT_LE(vertex_ordering[3], vertex_ordering[4]);
  ASSERT_LE(vertex_ordering[2], vertex_ordering[3]);
  ASSERT_LE(vertex_ordering[1], vertex_ordering[2]);
  ASSERT_LE(vertex_ordering[0], vertex_ordering[2]);
  AdjacencyMatrix g2(5);
  g2.set_edge(0, 0, 2);
  g2.set_edge(1, 0, 2);
  g2.set_edge(2, 0, 3);
  g2.set_edge(2, 0, 4);
  g2.set_edge(4, 0, 3);
  vertex_ordering.clear();
  g2.dfs_topological_sort(vertex_ordering);
  ASSERT_LE(vertex_ordering[4], vertex_ordering[3]);
  ASSERT_LE(vertex_ordering[2], vertex_ordering[3]);
  ASSERT_LE(vertex_ordering[1], vertex_ordering[2]);
  ASSERT_LE(vertex_ordering[0], vertex_ordering[2]);
  
}

// 2nd topological sort test, where 1 and 0 are not the lowest values
TEST(AdjacencyMatrixTest, DFSTopologicalSort) {
  AdjacencyMatrix g(6);
  g.set_edge(4, 0, 0);
  g.set_edge(5, 0, 0);
  g.set_edge(4, 0, 2);
  g.set_edge(5, 0, 1);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 1);
  Map vertex_ordering;
  g.dfs_topological_sort(vertex_ordering);
  ASSERT_LE(vertex_ordering[4], vertex_ordering[2]);
  ASSERT_LE(vertex_ordering[5], vertex_ordering[3]);
  ASSERT_LE(vertex_ordering[3], vertex_ordering[1]);
  ASSERT_LE(vertex_ordering[2], vertex_ordering[0]);
}


// 3rd topological sorting test, where the graph is split
TEST(AdjacencyMatrixTest, DisconnectedDFSTopologicalSort) {
  AdjacencyMatrix g(6);
  g.set_edge(3, 0, 4);
  g.set_edge(5, 0, 4);
  g.set_edge(0, 0, 1);
  g.set_edge(2, 0, 0);
  Map vertex_ordering;
  g.dfs_topological_sort(vertex_ordering);
  ASSERT_LE(vertex_ordering[3], vertex_ordering[4]);
  ASSERT_LE(vertex_ordering[5], vertex_ordering[3]);
  ASSERT_LE(vertex_ordering[0], vertex_ordering[1]);
  ASSERT_LE(vertex_ordering[2], vertex_ordering[0]);
}


int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

