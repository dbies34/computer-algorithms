//----------------------------------------------------------------------
// Name: 
// File: hw8_tests.cpp
// Date: Spring 2021
// Desc: Unit tests for list and matrix graph implementations
//----------------------------------------------------------------------


#include <iostream>
#include <string>
#include <list>
#include <set>
#include <vector>
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


//----------------------------------------------------------------------
// Adjacency List Tests
//----------------------------------------------------------------------

TEST(AdjacencyListTest, BasicPositiveWeightBellmanFordShortestPath) {
  AdjacencyList g(4);
  g.set_edge(0, 1, 1);
  g.set_edge(1, 6, 2);
  g.set_edge(1, 2, 3);
  g.set_edge(3, 3, 2);
  g.set_edge(0, 4, 3);  
  Map path_costs;
  ASSERT_EQ(true, g.bellman_ford_shortest_path(0, path_costs));
  // check size
  ASSERT_EQ(g.vertex_count(), path_costs.size());
  // check path costs
  ASSERT_EQ(0, path_costs[0]);
  ASSERT_EQ(1, path_costs[1]);
  ASSERT_EQ(6, path_costs[2]);
  ASSERT_EQ(3, path_costs[3]);
}

TEST(AdjacencyListTest, BasicNegativeWeightBellmanFordShortestPath) {
  AdjacencyList g(5);
  g.set_edge(0, 6, 1);
  g.set_edge(0, 7, 3);
  g.set_edge(1, 5, 2);
  g.set_edge(1, 8, 3);
  g.set_edge(1, -4, 4);
  g.set_edge(2, -2, 1);
  g.set_edge(3, -3, 2);
  g.set_edge(3, 9, 4);  
  g.set_edge(4, 7, 2);
  Map path_costs;
  ASSERT_EQ(true, g.bellman_ford_shortest_path(0, path_costs));
  // check size
  ASSERT_EQ(g.vertex_count(), path_costs.size());
  // check path costs
  ASSERT_EQ(0, path_costs[0]);
  ASSERT_EQ(2, path_costs[1]);
  ASSERT_EQ(4, path_costs[2]);
  ASSERT_EQ(7, path_costs[3]);
  ASSERT_EQ(-2, path_costs[4]);
}

TEST(AdjacencyListTest, NegativeCycleWeightBellmanFordShortestPath) {
  AdjacencyList g(5);
  g.set_edge(0, 1, 1);
  g.set_edge(0, 5, 2);
  g.set_edge(2, 5, 3);
  g.set_edge(3, 4, 1);
  g.set_edge(2, -4, 4);
  g.set_edge(4, 2, 2);
  Map path_costs;
  ASSERT_EQ(false, g.bellman_ford_shortest_path(0, path_costs));
}

TEST(AdjacencyListTest, ComplexBellmanFordShortestPath) {
  AdjacencyList g(5);
  g.set_edge(0, 1, 1);
  g.set_edge(0, 5, 2);
  g.set_edge(1, 3, 4);
  g.set_edge(2, 5, 3);
  g.set_edge(2, 2, 4);
  g.set_edge(3, 4, 1);
  g.set_edge(3, 7, 4);
  g.set_edge(3, -1, 2);
  g.set_edge(4, 2, 1);
  g.set_edge(4, 3, 0);
  g.set_edge(4, -2, 2);
  Map path_costs;
  ASSERT_EQ(true, g.bellman_ford_shortest_path(0, path_costs));
  // check size
  ASSERT_EQ(g.vertex_count(), path_costs.size());
  // check path costs
  ASSERT_EQ(0, path_costs[0]);
  ASSERT_EQ(1, path_costs[1]);
  ASSERT_EQ(2, path_costs[2]);
  ASSERT_EQ(7, path_costs[3]);
  ASSERT_EQ(4, path_costs[4]);
}

//----------------------------------------------------------------------
// Adjacency Matrix Tests
//----------------------------------------------------------------------


TEST(AdjacencyMatrixTest, BasicPositiveWeightBellmanFordShortestPath) {
  AdjacencyMatrix g(4);
  g.set_edge(0, 1, 1);
  g.set_edge(1, 6, 2);
  g.set_edge(1, 2, 3);
  g.set_edge(0, 4, 3);  
  g.set_edge(3, 3, 2);
  Map path_costs;
  ASSERT_EQ(true, g.bellman_ford_shortest_path(0, path_costs));
  // check size
  ASSERT_EQ(g.vertex_count(), path_costs.size());
  // check path costs
  ASSERT_EQ(0, path_costs[0]);
  ASSERT_EQ(1, path_costs[1]);
  ASSERT_EQ(6, path_costs[2]);
  ASSERT_EQ(3, path_costs[3]);
}

TEST(AdjacencyMatrixTest, BasicNegativeWeightBellmanFordShortestPath) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 6, 1);
  g.set_edge(0, 7, 3);
  g.set_edge(1, 5, 2);
  g.set_edge(1, 8, 3);
  g.set_edge(1, -4, 4);
  g.set_edge(2, -2, 1);
  g.set_edge(3, -3, 2);
  g.set_edge(3, 9, 4);  
  g.set_edge(4, 7, 2);
  Map path_costs;
  ASSERT_EQ(true, g.bellman_ford_shortest_path(0, path_costs));
  // check size
  ASSERT_EQ(g.vertex_count(), path_costs.size());
  // check path costs
  ASSERT_EQ(0, path_costs[0]);
  ASSERT_EQ(2, path_costs[1]);
  ASSERT_EQ(4, path_costs[2]);
  ASSERT_EQ(7, path_costs[3]);
  ASSERT_EQ(-2, path_costs[4]);
}

TEST(AdjacencyMatrixTest, NegativeCycleWeightBellmanFordShortestPath) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 1, 1);
  g.set_edge(0, 5, 2);
  g.set_edge(2, 5, 3);
  g.set_edge(3, 4, 1);
  g.set_edge(2, -4, 4);
  g.set_edge(4, 2, 2);
  Map path_costs;
  ASSERT_EQ(false, g.bellman_ford_shortest_path(0, path_costs));
}

TEST(AdjacencyMatrixTest, ComplexBellmanFordShortestPath) {
  AdjacencyMatrix g(5);
  g.set_edge(0, 1, 1);
  g.set_edge(0, 5, 2);
  g.set_edge(1, 3, 4);
  g.set_edge(2, 5, 3);
  g.set_edge(2, 2, 4);
  g.set_edge(3, 4, 1);
  g.set_edge(3, 7, 4);
  g.set_edge(3, -1, 2);
  g.set_edge(4, 2, 1);
  g.set_edge(4, 3, 0);
  g.set_edge(4, -2, 2);
  Map path_costs;
  ASSERT_EQ(true, g.bellman_ford_shortest_path(0, path_costs));
  // check size
  ASSERT_EQ(g.vertex_count(), path_costs.size());
  // check path costs
  ASSERT_EQ(0, path_costs[0]);
  ASSERT_EQ(1, path_costs[1]);
  ASSERT_EQ(2, path_costs[2]);
  ASSERT_EQ(7, path_costs[3]);
  ASSERT_EQ(4, path_costs[4]);
}


int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

