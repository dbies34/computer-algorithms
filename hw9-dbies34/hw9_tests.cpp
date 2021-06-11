//----------------------------------------------------------------------
// Name: 
// File: hw9_tests.cpp
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

void print_weight_vector(const vector<vector<int>>& weights, int n) {
  for (int u = 0; u < n; ++u) {
    for (int v = 0; v < n; ++v) {
      if (weights[u][v] == std::numeric_limits<int>::max())
        cout << "inf" << " ";
      else
        cout << weights[u][v] << " ";
    }
    cout << endl;
  }
}

void print_set(const Set &S) 
{
  for (auto num : S)
  {
    cout << num << ", ";
  }
  cout << endl;
}


//----------------------------------------------------------------------
// Adjacency List Tests
//----------------------------------------------------------------------

TEST(AdjacencyListTest, BasicMaxIndependentSet) {
  AdjacencyList g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  Set S;
  int weights[4] = {1, 4, 5, 4};
  ASSERT_EQ(true, g.path_max_independent_set(weights, S));
  // check size  
  ASSERT_EQ(2, S.size());
  // check members
  ASSERT_EQ(true, S.count(1));
  ASSERT_EQ(true, S.count(3));
}

TEST(AdjacencyListTest, SmallMaxIndependentSet) {
  AdjacencyList g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  Set S;
  int weights[6] = {3, 2, 1, 6, 4, 5};
  ASSERT_EQ(true, g.path_max_independent_set(weights, S));
  // check size  
  ASSERT_EQ(3, S.size());
  // check members
  ASSERT_EQ(true, S.count(0));
  ASSERT_EQ(true, S.count(3));
  ASSERT_EQ(true, S.count(5));
}

TEST(AdjacencyListTest, BigMaxIndependentSet) {
  AdjacencyList g(10);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  g.set_edge(5, 0, 6);
  g.set_edge(6, 0, 7);
  g.set_edge(7, 0, 8);
  g.set_edge(8, 0, 9);
  Set S;
  int weights[10] = {1, 6, 3, 9, 2, 6, 2, 8, 3, 7};
  ASSERT_EQ(true, g.path_max_independent_set(weights, S));
  // check size  
  ASSERT_EQ(5, S.size());
  // check members
  ASSERT_EQ(true, S.count(1));
  ASSERT_EQ(true, S.count(3));
  ASSERT_EQ(true, S.count(5));
  ASSERT_EQ(true, S.count(7));
  ASSERT_EQ(true, S.count(9));
}

TEST(AdjacencyListTest, NegMaxIndependentSet) {
  AdjacencyList g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  Set S;
  int weights[6] = {-1, 1, -6, 3, 8, 6};
  ASSERT_EQ(true, g.path_max_independent_set(weights, S));
  // check size  
  ASSERT_EQ(3, S.size());
  // check members
  ASSERT_EQ(true, S.count(1));
  ASSERT_EQ(true, S.count(3));
  ASSERT_EQ(true, S.count(5));
}

TEST(AdjacencyListTest, BasicAllPairsShortestPaths) {
  AdjacencyList g(5);
  g.set_edge(0, -10, 1);
  g.set_edge(0, 2, 3);
  g.set_edge(1, -10, 2);
  g.set_edge(3, -4, 4);
  g.set_edge(4, 5, 2);
  vector<vector<int>> weights;
  ASSERT_EQ(true, g.all_pairs_shortest_paths(weights));
  // Note that you can call the following to help debug: 
  //   print_weight_vector(weights, 5);
  ASSERT_EQ(-10, weights[0][1]);
  ASSERT_EQ(-20, weights[0][2]);
  ASSERT_EQ(2, weights[0][3]);
  ASSERT_EQ(-2, weights[0][4]);
  ASSERT_EQ(-10, weights[1][2]);
  ASSERT_EQ(1, weights[3][2]);
  ASSERT_EQ(-4, weights[3][4]);
  ASSERT_EQ(5, weights[4][2]);
  // Note should also test MAX_INT cases above
}


TEST(AdjacencyListTest, NegCycleAllPairsShortestPaths) {
  AdjacencyList g(5);
  g.set_edge(0, -10, 1);
  g.set_edge(1, 5, 0);
  g.set_edge(0, 2, 3);
  g.set_edge(1, -10, 2);
  g.set_edge(3, -4, 4);
  g.set_edge(4, 5, 2);
  vector<vector<int>> weights;
  ASSERT_EQ(false, g.all_pairs_shortest_paths(weights));
}

// test an unreachable vertex
TEST(AdjacencyListTest, UnreachableAllPairsShortestPaths) {
  AdjacencyList g(2);
  vector<vector<int>> weights;
  ASSERT_EQ(true, g.all_pairs_shortest_paths(weights));
  ASSERT_EQ(numeric_limits<int>::max(), weights[0][1]);
}

TEST(AdjacencyListTest, BigAllPairsShortestPaths) {
  AdjacencyList g(8);
  g.set_edge(0, 2, 1);
  g.set_edge(0, 2, 3);
  g.set_edge(1, 8, 2);
  g.set_edge(3, -2, 4);
  g.set_edge(4, 9, 2);
  g.set_edge(4, 0, 7);
  g.set_edge(6, 5, 7);
  g.set_edge(7, 10, 5);
  g.set_edge(5, -5, 7);
  vector<vector<int>> weights;
  ASSERT_EQ(true, g.all_pairs_shortest_paths(weights));
  
  ASSERT_EQ(2, weights[0][1]);
  ASSERT_EQ(9, weights[0][2]);
  ASSERT_EQ(2, weights[0][3]);
  ASSERT_EQ(0, weights[0][4]);
  ASSERT_EQ(10, weights[0][5]);
  ASSERT_EQ(numeric_limits<int>::max(), weights[0][6]);
  ASSERT_EQ(0, weights[0][7]);
  ASSERT_EQ(8, weights[1][2]);
  ASSERT_EQ(9, weights[4][2]);
}

//----------------------------------------------------------------------
// Adjacency Matrix Tests
//----------------------------------------------------------------------


TEST(AdjacencyMatrixTest, BasicMaxIndependentSet) {
  AdjacencyMatrix g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  Set S;
  int weights[4] = {1, 4, 5, 4};
  ASSERT_EQ(true, g.path_max_independent_set(weights, S));
  // check size  
  ASSERT_EQ(2, S.size());
  // check members
  ASSERT_EQ(true, S.count(1));
  ASSERT_EQ(true, S.count(3));
}

TEST(AdjacencyMatrixTest, SmallMaxIndependentSet) {
  AdjacencyMatrix g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  Set S;
  int weights[6] = {3, 2, 1, 6, 4, 5};
  ASSERT_EQ(true, g.path_max_independent_set(weights, S));
  // check size  
  ASSERT_EQ(3, S.size());
  // check members
  ASSERT_EQ(true, S.count(0));
  ASSERT_EQ(true, S.count(3));
  ASSERT_EQ(true, S.count(5));
}

TEST(AdjacencyMatrixTest, BigMaxIndependentSet) {
  AdjacencyMatrix g(10);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  g.set_edge(5, 0, 6);
  g.set_edge(6, 0, 7);
  g.set_edge(7, 0, 8);
  g.set_edge(8, 0, 9);
  Set S;
  int weights[10] = {1, 6, 3, 9, 2, 6, 2, 8, 3, 7};
  ASSERT_EQ(true, g.path_max_independent_set(weights, S));
  // check size  
  ASSERT_EQ(5, S.size());
  // check members
  ASSERT_EQ(true, S.count(1));
  ASSERT_EQ(true, S.count(3));
  ASSERT_EQ(true, S.count(5));
  ASSERT_EQ(true, S.count(7));
  ASSERT_EQ(true, S.count(9));
}

TEST(AdjacencyMatrixTest, NegMaxIndependentSet) {
  AdjacencyMatrix g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  Set S;
  int weights[6] = {-1, 1, -6, 3, 8, 6};
  ASSERT_EQ(true, g.path_max_independent_set(weights, S));
  // check size  
  ASSERT_EQ(3, S.size());
  // check members
  ASSERT_EQ(true, S.count(1));
  ASSERT_EQ(true, S.count(3));
  ASSERT_EQ(true, S.count(5));
}

TEST(AdjacencyMatrixTest, BasicAllPairsShortestPaths) {
  AdjacencyMatrix g(5);
  g.set_edge(0, -10, 1);
  g.set_edge(0, 2, 3);
  g.set_edge(1, -10, 2);
  g.set_edge(3, -4, 4);
  g.set_edge(4, 5, 2);
  vector<vector<int>> weights;
  ASSERT_EQ(true, g.all_pairs_shortest_paths(weights));
  // Note that you can call the following to help debug: 
  //   print_weight_vector(weights, 5);
  ASSERT_EQ(-10, weights[0][1]);
  ASSERT_EQ(-20, weights[0][2]);
  ASSERT_EQ(2, weights[0][3]);
  ASSERT_EQ(-2, weights[0][4]);
  ASSERT_EQ(-10, weights[1][2]);
  ASSERT_EQ(1, weights[3][2]);
  ASSERT_EQ(-4, weights[3][4]);
  ASSERT_EQ(5, weights[4][2]);
  // Note should also test MAX_INT cases above
}


TEST(AdjacencyMatrixTest, NegCycleAllPairsShortestPaths) {
  AdjacencyMatrix g(5);
  g.set_edge(0, -10, 1);
  g.set_edge(1, 5, 0);
  g.set_edge(0, 2, 3);
  g.set_edge(1, -10, 2);
  g.set_edge(3, -4, 4);
  g.set_edge(4, 5, 2);
  vector<vector<int>> weights;
  ASSERT_EQ(false, g.all_pairs_shortest_paths(weights));
}

// test an unreachable vertex
TEST(AdjacencyMatrixTest, UnreachableAllPairsShortestPaths) {
  AdjacencyMatrix g(2);
  vector<vector<int>> weights;
  ASSERT_EQ(true, g.all_pairs_shortest_paths(weights));
  ASSERT_EQ(numeric_limits<int>::max(), weights[0][1]);
}

TEST(AdjacencyMatrixTest, BigAllPairsShortestPaths) {
  AdjacencyMatrix g(8);
  g.set_edge(0, 2, 1);
  g.set_edge(0, 2, 3);
  g.set_edge(1, 8, 2);
  g.set_edge(3, -2, 4);
  g.set_edge(4, 9, 2);
  g.set_edge(4, 0, 7);
  g.set_edge(6, 5, 7);
  g.set_edge(7, 10, 5);
  g.set_edge(5, -5, 7);
  vector<vector<int>> weights;
  ASSERT_EQ(true, g.all_pairs_shortest_paths(weights));
  
  ASSERT_EQ(2, weights[0][1]);
  ASSERT_EQ(9, weights[0][2]);
  ASSERT_EQ(2, weights[0][3]);
  ASSERT_EQ(0, weights[0][4]);
  ASSERT_EQ(10, weights[0][5]);
  ASSERT_EQ(numeric_limits<int>::max(), weights[0][6]);
  ASSERT_EQ(0, weights[0][7]);
  ASSERT_EQ(8, weights[1][2]);
  ASSERT_EQ(9, weights[4][2]);
}


int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

