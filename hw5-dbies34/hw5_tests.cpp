//----------------------------------------------------------------------
// Name: 
// File: hw5_tests.cpp
// Date: Spring 2021
// Desc: Unit tests for graph functions related to HW-5
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

void print_path(const list<int>& path)
{
  for (int v : path)
    cout << v << " ";
  cout << endl;
}

void print_map(Map &map)
{
  for (Map::iterator it = map.begin(); it != map.end(); ++it)
    cout << it -> first << " : " << it -> second << endl;
}


//----------------------------------------------------------------------
// Adjacency List Tests
//----------------------------------------------------------------------

TEST(AdjacencyListTest, BasicRemoveEdge) {
  AdjacencyList g(5);
  g.set_edge(0, 0, 2);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(2, 0, 4);
  ASSERT_EQ(4, g.edge_count());
  g.remove_edge(2, 3);
  ASSERT_EQ(3, g.edge_count());  
  g.remove_edge(2, 4);
  ASSERT_EQ(2, g.edge_count());  
  std::list<int> vertices;
  g.connected_to(2, vertices);
  ASSERT_EQ(0, vertices.size());
  g.connected_from(2, vertices);
  ASSERT_EQ(2, vertices.size());
  g.remove_edge(0, 2);
  ASSERT_EQ(1, g.edge_count());
  g.remove_edge(1, 2);
  ASSERT_EQ(0, g.edge_count());
}


// test of removing edges that dont exist
TEST(AdjacencyListTest, RemoveNonExistentEdges) {
  AdjacencyList g(5);
  g.set_edge(0, 0, 2);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(2, 0, 4);
  ASSERT_EQ(4, g.edge_count());
  // vertex 4 has no out edges 
  g.remove_edge(4, 0);
  // edge count should remain the same
  ASSERT_EQ(4, g.edge_count());
  // vertex 2 has two out edges not including 0  
  g.remove_edge(2, 0);
  // edge count should remain the same
  ASSERT_EQ(4, g.edge_count());  
}

// remove edge test with a complex graph
TEST(AdjacencyListTest, RemoveEdgeComplexGraph) {
  AdjacencyList g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(0, 0, 4);
  g.set_edge(2, 0, 5);
  g.set_edge(3, 0, 0);
  g.set_edge(4, 0, 3); 
  g.set_edge(4, 0, 2);
  g.set_edge(5, 0, 4); 
  g.set_edge(5, 0, 2);
  g.set_edge(5, 0, 0);
  g.set_edge(5, 0, 3);
  g.set_edge(5, 0, 1);    
  g.set_edge(3, 0, 5);
  g.set_edge(0, 0, 5);
  g.set_edge(1, 0, 5);
  ASSERT_EQ(15, g.edge_count());

  // remove edge from vertex with a high degree
  g.remove_edge(5,4);
  // one less edge count
  ASSERT_EQ(14, g.edge_count());

  // try to remove the same edge
  g.remove_edge(5,4);
  // same edge count
  ASSERT_EQ(14, g.edge_count());
}


TEST(AdjacencyListTest, BasicStronglyConnectedComponents) {
  AdjacencyList g(12);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 3);
  g.set_edge(1, 0, 4);
  g.set_edge(4, 0, 1);
  g.set_edge(2, 0, 5);
  g.set_edge(5, 0, 2);
  g.set_edge(1, 0, 2);
  g.set_edge(4, 0, 5);
  g.set_edge(4, 0, 6);
  g.set_edge(5, 0, 7);
  g.set_edge(6, 0, 7);
  g.set_edge(7, 0, 10);
  g.set_edge(10, 0, 11);
  g.set_edge(11, 0, 9);
  g.set_edge(9, 0, 8);
  g.set_edge(8, 0, 6);
  g.set_edge(6, 0, 9);
  Map components;
  g.strongly_connected_components(components);
  ASSERT_EQ(12, components.size());
  int counts[5];
  counts[0] = components[0];
  counts[1] = components[3];
  counts[2] = components[1];
  counts[3] = components[2];
  counts[4] = components[6];
  // each count should be unique
  for (int i = 0; i < 4; ++i)
    for (int j = i + 1; j < 5; ++j)
      ASSERT_NE(counts[i], counts[j]);
  // check that the components are correct
  ASSERT_EQ(counts[2], components[4]);
  ASSERT_EQ(counts[3], components[5]);
  ASSERT_EQ(counts[4], components[7]);
  ASSERT_EQ(counts[4], components[8]);
  ASSERT_EQ(counts[4], components[9]);
  ASSERT_EQ(counts[4], components[10]);
  ASSERT_EQ(counts[4], components[11]);  
}


// test over a big SCC
TEST(AdjacencyListTest, BigStronglyConnectedComponents) {
  AdjacencyList g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 0);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 1);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 2);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 3); 
  g.set_edge(4, 0, 0);
  g.set_edge(0, 0, 4); 
  Map components;
  g.strongly_connected_components(components);
  ASSERT_EQ(5, components.size());

  // every vertex should be in the same component
  for (Map::iterator it = components.begin(); it != components.end(); ++it)
    ASSERT_EQ(0, it -> second);
}

// test with a graph having a high edge count
TEST(AdjacencyListTest, ComplexStronglyConnectedComponents) {
  AdjacencyList g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(0, 0, 4);
  g.set_edge(2, 0, 5);
  g.set_edge(3, 0, 0);
  g.set_edge(4, 0, 3); 
  g.set_edge(4, 0, 2);
  g.set_edge(5, 0, 4); 
  g.set_edge(5, 0, 2);
  g.set_edge(5, 0, 0);
  g.set_edge(5, 0, 3);
  g.set_edge(5, 0, 1);    
  g.set_edge(3, 0, 5);
  g.set_edge(0, 0, 5);
  g.set_edge(1, 0, 5);

  Map components;
  g.strongly_connected_components(components);

  // should return only two components
  for (Map::iterator it = components.begin(); it != components.end(); ++it)
    ASSERT_GE(1, it -> second);
}

TEST(AdjacencyListTest, BasicTransitiveReduction) {
  AdjacencyList g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 0);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 1);
  g.set_edge(2, 0, 0);
  g.set_edge(0, 0, 2);
  g.set_edge(1, 0, 3);
  g.set_edge(1, 0, 4);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 3);
  AdjacencyList tr_g(5);
  g.transitive_reduction(tr_g);
  ASSERT_EQ(6, tr_g.edge_count());
  ASSERT_EQ(true, tr_g.has_edge(0, 1) xor tr_g.has_edge(1, 0));
  ASSERT_EQ(true, tr_g.has_edge(1, 2) xor tr_g.has_edge(2, 1));
  ASSERT_EQ(true, tr_g.has_edge(2, 0) xor tr_g.has_edge(0, 2));  
  ASSERT_EQ(true, tr_g.has_edge(3, 4) and tr_g.has_edge(4, 3));
}

// this tests having a weakly connected lone vertex
TEST(AdjacencyListTest, LoneSCCTransitiveReduction) {
  AdjacencyList g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 0);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 1);
  g.set_edge(2, 0, 0);
  g.set_edge(0, 0, 2);
  g.set_edge(1, 0, 3);
  g.set_edge(1, 0, 4);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 3);
  g.set_edge(4, 0, 5);
  AdjacencyList tr_g(6);
  g.transitive_reduction(tr_g);
  // should have only one more edge than the previous test
  ASSERT_EQ(7, tr_g.edge_count());
  ASSERT_EQ(true, tr_g.has_edge(0, 1) xor tr_g.has_edge(1, 0));
  ASSERT_EQ(true, tr_g.has_edge(1, 2) xor tr_g.has_edge(2, 1));
  ASSERT_EQ(true, tr_g.has_edge(2, 0) xor tr_g.has_edge(0, 2));  
  ASSERT_EQ(true, tr_g.has_edge(3, 4) and tr_g.has_edge(4, 3));
}

// this tests having a graph with no SCCs
TEST(AdjacencyListTest, NoSCCTransitiveReduction) {
  AdjacencyList g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(0, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  ASSERT_EQ(5, g.edge_count());
  AdjacencyList tr_g(6);
  g.transitive_reduction(tr_g);

  // no edges should be removed
  ASSERT_EQ(true, tr_g.has_edge(0, 1));
  ASSERT_EQ(true, tr_g.has_edge(1, 2));
  ASSERT_EQ(true, tr_g.has_edge(0, 3));
  ASSERT_EQ(true, tr_g.has_edge(3, 4));
  ASSERT_EQ(true, tr_g.has_edge(4, 5));
}

// TODO: Add at least one additional transitive reduction tests

TEST(AdjacencyListTest, BasicEulerianPath) {
  AdjacencyList g(5);
  g.set_edge(0, 0, 2);
  g.set_edge(1, 0, 0);
  g.set_edge(1, 0, 3);
  g.set_edge(2, 0, 1);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 1);
  g.set_edge(3, 0, 4);  
  list<int> path;
  ASSERT_EQ(true, g.directed_eulerian_path(path));
  ASSERT_EQ(g.edge_count() + 1, path.size());
  // check for actual edges
  vector<int> vpath(path.begin(), path.end());
  for (int i = 0; i < g.edge_count(); ++i) {
    ASSERT_EQ(true, g.has_edge(vpath[i], vpath[i+1]));
  }
  // check for duplicate edges
  for (int i = 0; i < g.edge_count() - 1; ++i) {
    int u = vpath[i];
    int v = vpath[i + 1];
    for (int j = i + 1; j < g.edge_count(); ++j) {
      if (vpath[j] == u)
        ASSERT_NE(v, vpath[j + 1]);
    }
  }
  g.set_edge(2, 0, 4);
  path.clear();
  ASSERT_EQ(false, g.directed_eulerian_path(path));
}


// test over a graph that has the sink and source as the same vertex
TEST(AdjacencyListTest, BasicEulerianPathCycle) {
  AdjacencyList g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 0);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 1);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 2);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 3); 
  g.set_edge(4, 0, 0);
  g.set_edge(0, 0, 4); 
  list<int> path;
  ASSERT_EQ(true, g.directed_eulerian_path(path));
  ASSERT_EQ(g.edge_count() + 1, path.size());
}

// test over a more complex graph that has the sink and source as the same vertex
TEST(AdjacencyListTest, ComplexEulerianPathCycle) {
  AdjacencyList g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(0, 0, 4);
  g.set_edge(2, 0, 5);
  g.set_edge(3, 0, 0);
  g.set_edge(4, 0, 3); 
  g.set_edge(4, 0, 2);
  g.set_edge(5, 0, 4); 
  list<int> path;
  ASSERT_EQ(true, g.directed_eulerian_path(path));
  ASSERT_EQ(g.edge_count() + 1, path.size());

  g.set_edge(5, 0, 3); 
  ASSERT_EQ(false, g.directed_eulerian_path(path));
}

//----------------------------------------------------------------------
// Adjacency Matrix Tests
//----------------------------------------------------------------------

TEST(AdjacencyMatrixTest, BasicRemoveEdge) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 0, 2);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(2, 0, 4);
  ASSERT_EQ(4, g.edge_count());
  g.remove_edge(2, 3);
  ASSERT_EQ(3, g.edge_count());  
  g.remove_edge(2, 4);
  ASSERT_EQ(2, g.edge_count());  
  std::list<int> vertices;
  g.connected_to(2, vertices);
  ASSERT_EQ(0, vertices.size());
  g.connected_from(2, vertices);
  ASSERT_EQ(2, vertices.size());
  g.remove_edge(0, 2);
  ASSERT_EQ(1, g.edge_count());
  g.remove_edge(1, 2);
  ASSERT_EQ(0, g.edge_count());
}


// TODO: Add your tests above but use your adjacency matrix instead of
//       your adjacency list
// test of removing edges that dont exist
TEST(AdjacencyMatrixTest, RemoveNonExistentEdges) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 0, 2);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(2, 0, 4);
  ASSERT_EQ(4, g.edge_count());
  // vertex 4 has no out edges 
  g.remove_edge(4, 0);
  // edge count should remain the same
  ASSERT_EQ(4, g.edge_count());
  // vertex 2 has two out edges not including 0  
  g.remove_edge(2, 0);
  // edge count should remain the same
  ASSERT_EQ(4, g.edge_count());  
}

// remove edge test with a complex graph
TEST(AdjacencyMatrixTest, RemoveEdgeComplexGraph) {
  AdjacencyMatrix g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(0, 0, 4);
  g.set_edge(2, 0, 5);
  g.set_edge(3, 0, 0);
  g.set_edge(4, 0, 3); 
  g.set_edge(4, 0, 2);
  g.set_edge(5, 0, 4); 
  g.set_edge(5, 0, 2);
  g.set_edge(5, 0, 0);
  g.set_edge(5, 0, 3);
  g.set_edge(5, 0, 1);    
  g.set_edge(3, 0, 5);
  g.set_edge(0, 0, 5);
  g.set_edge(1, 0, 5);
  ASSERT_EQ(15, g.edge_count());

  // remove edge from vertex with a high degree
  g.remove_edge(5,4);
  // one less edge count
  ASSERT_EQ(14, g.edge_count());

  // try to remove the same edge
  g.remove_edge(5,4);
  // same edge count
  ASSERT_EQ(14, g.edge_count());
}


TEST(AdjacencyMatrixTest, BasicStronglyConnectedComponents) {
  AdjacencyMatrix g(12);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 3);
  g.set_edge(1, 0, 4);
  g.set_edge(4, 0, 1);
  g.set_edge(2, 0, 5);
  g.set_edge(5, 0, 2);
  g.set_edge(1, 0, 2);
  g.set_edge(4, 0, 5);
  g.set_edge(4, 0, 6);
  g.set_edge(5, 0, 7);
  g.set_edge(6, 0, 7);
  g.set_edge(7, 0, 10);
  g.set_edge(10, 0, 11);
  g.set_edge(11, 0, 9);
  g.set_edge(9, 0, 8);
  g.set_edge(8, 0, 6);
  g.set_edge(6, 0, 9);
  Map components;
  g.strongly_connected_components(components);
  ASSERT_EQ(12, components.size());
  int counts[5];
  counts[0] = components[0];
  counts[1] = components[3];
  counts[2] = components[1];
  counts[3] = components[2];
  counts[4] = components[6];
  // each count should be unique
  for (int i = 0; i < 4; ++i)
    for (int j = i + 1; j < 5; ++j)
      ASSERT_NE(counts[i], counts[j]);
  // check that the components are correct
  ASSERT_EQ(counts[2], components[4]);
  ASSERT_EQ(counts[3], components[5]);
  ASSERT_EQ(counts[4], components[7]);
  ASSERT_EQ(counts[4], components[8]);
  ASSERT_EQ(counts[4], components[9]);
  ASSERT_EQ(counts[4], components[10]);
  ASSERT_EQ(counts[4], components[11]);  
}


// test over a big SCC
TEST(AdjacencyMatrixTest, BigStronglyConnectedComponents) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 0);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 1);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 2);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 3); 
  g.set_edge(4, 0, 0);
  g.set_edge(0, 0, 4); 
  Map components;
  g.strongly_connected_components(components);
  ASSERT_EQ(5, components.size());

  // every vertex should be in the same component
  for (Map::iterator it = components.begin(); it != components.end(); ++it)
    ASSERT_EQ(0, it -> second);
}

// test with a graph having a high edge count
TEST(AdjacencyMatrixTest, ComplexStronglyConnectedComponents) {
  AdjacencyMatrix g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(0, 0, 4);
  g.set_edge(2, 0, 5);
  g.set_edge(3, 0, 0);
  g.set_edge(4, 0, 3); 
  g.set_edge(4, 0, 2);
  g.set_edge(5, 0, 4); 
  g.set_edge(5, 0, 2);
  g.set_edge(5, 0, 0);
  g.set_edge(5, 0, 3);
  g.set_edge(5, 0, 1);    
  g.set_edge(3, 0, 5);
  g.set_edge(0, 0, 5);
  g.set_edge(1, 0, 5);

  Map components;
  g.strongly_connected_components(components);

  // should return only two components
  for (Map::iterator it = components.begin(); it != components.end(); ++it)
    ASSERT_GE(1, it -> second);
}

TEST(AdjacencyMatrixTest, BasicTransitiveReduction) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 0);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 1);
  g.set_edge(2, 0, 0);
  g.set_edge(0, 0, 2);
  g.set_edge(1, 0, 3);
  g.set_edge(1, 0, 4);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 3);
  AdjacencyMatrix tr_g(5);
  g.transitive_reduction(tr_g);
  ASSERT_EQ(6, tr_g.edge_count());
  ASSERT_EQ(true, tr_g.has_edge(0, 1) xor tr_g.has_edge(1, 0));
  ASSERT_EQ(true, tr_g.has_edge(1, 2) xor tr_g.has_edge(2, 1));
  ASSERT_EQ(true, tr_g.has_edge(2, 0) xor tr_g.has_edge(0, 2));  
  ASSERT_EQ(true, tr_g.has_edge(3, 4) and tr_g.has_edge(4, 3));
}

// this tests having a weakly connected lone vertex
TEST(AdjacencyMatrixTest, LoneSCCTransitiveReduction) {
  AdjacencyMatrix g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 0);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 1);
  g.set_edge(2, 0, 0);
  g.set_edge(0, 0, 2);
  g.set_edge(1, 0, 3);
  g.set_edge(1, 0, 4);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 3);
  g.set_edge(4, 0, 5);
  AdjacencyMatrix tr_g(6);
  g.transitive_reduction(tr_g);
  // should have only one more edge than the previous test
  ASSERT_EQ(7, tr_g.edge_count());
  ASSERT_EQ(true, tr_g.has_edge(0, 1) xor tr_g.has_edge(1, 0));
  ASSERT_EQ(true, tr_g.has_edge(1, 2) xor tr_g.has_edge(2, 1));
  ASSERT_EQ(true, tr_g.has_edge(2, 0) xor tr_g.has_edge(0, 2));  
  ASSERT_EQ(true, tr_g.has_edge(3, 4) and tr_g.has_edge(4, 3));
}

// this tests having a graph with no SCCs
TEST(AdjacencyMatrixTest, NoSCCTransitiveReduction) {
  AdjacencyMatrix g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(0, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  ASSERT_EQ(5, g.edge_count());
  AdjacencyMatrix tr_g(6);
  g.transitive_reduction(tr_g);
  
  // no edges should be removed
  ASSERT_EQ(true, tr_g.has_edge(0, 1));
  ASSERT_EQ(true, tr_g.has_edge(1, 2));
  ASSERT_EQ(true, tr_g.has_edge(0, 3));
  ASSERT_EQ(true, tr_g.has_edge(3, 4));
  ASSERT_EQ(true, tr_g.has_edge(4, 5));
}

// TODO: Add at least one additional transitive reduction tests

TEST(AdjacencyMatrixTest, BasicEulerianPath) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 0, 2);
  g.set_edge(1, 0, 0);
  g.set_edge(1, 0, 3);
  g.set_edge(2, 0, 1);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 1);
  g.set_edge(3, 0, 4);  
  list<int> path;
  ASSERT_EQ(true, g.directed_eulerian_path(path));
  ASSERT_EQ(g.edge_count() + 1, path.size());
  // check for actual edges
  vector<int> vpath(path.begin(), path.end());
  for (int i = 0; i < g.edge_count(); ++i) {
    ASSERT_EQ(true, g.has_edge(vpath[i], vpath[i+1]));
  }
  // check for duplicate edges
  for (int i = 0; i < g.edge_count() - 1; ++i) {
    int u = vpath[i];
    int v = vpath[i + 1];
    for (int j = i + 1; j < g.edge_count(); ++j) {
      if (vpath[j] == u)
        ASSERT_NE(v, vpath[j + 1]);
    }
  }
  g.set_edge(2, 0, 4);
  path.clear();
  ASSERT_EQ(false, g.directed_eulerian_path(path));
}


// test over a graph that has the sink and source as the same vertex
TEST(AdjacencyMatrixTest, BasicEulerianPathCycle) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 0);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 1);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 2);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 3); 
  g.set_edge(4, 0, 0);
  g.set_edge(0, 0, 4); 
  list<int> path;
  ASSERT_EQ(true, g.directed_eulerian_path(path));
  ASSERT_EQ(g.edge_count() + 1, path.size());
}

// test over a more complex graph that has the sink and source as the same vertex
TEST(AdjacencyMatrixTest, ComplexEulerianPathCycle) {
  AdjacencyMatrix g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(0, 0, 4);
  g.set_edge(2, 0, 5);
  g.set_edge(3, 0, 0);
  g.set_edge(4, 0, 3); 
  g.set_edge(4, 0, 2);
  g.set_edge(5, 0, 4); 
  list<int> path;
  ASSERT_EQ(true, g.directed_eulerian_path(path));
  ASSERT_EQ(g.edge_count() + 1, path.size());

  g.set_edge(5, 0, 3); 
  ASSERT_EQ(false, g.directed_eulerian_path(path));
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

