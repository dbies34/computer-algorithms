//----------------------------------------------------------------------
// Name: S. Bowers
// File: hw1_examples.cpp
// Date: Spring 2021
// Desc: Basic tests for different sized example graphs
//----------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <gtest/gtest.h>
#include "adjacency_list.h"
#include "adjacency_matrix.h"

using namespace std;


// Helper function to obtain number of edges and vertices in an
// example graph.
void get_stats(int& vertices, int& edges, const string& filename)
{
  ifstream f(filename);
  f >> vertices >> edges; 
  f.close();
}

// Builds a graph from the given example graph. Requires graph to
// already be created (and thus, number of vertices known)
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

// Returns the number of in edges for the vertex with the largest
// number of in edges in the graph.
int max_in_edges(Graph* g)
{
  list<int> connections;
  int max = 0;
  for (int i = 0; i < g->vertex_count(); ++i) {
    g->connected_from(i, connections);
    if (max < connections.size())
        max = connections.size();
    connections.clear();
  }
  return max;
}

// Returns the number of out edges for the vertex with the largest
// number of out edges in the graph.
int max_out_edges(Graph* g)
{
  list<int> connections;
  int max = 0;
  for (int i = 0; i < g->vertex_count(); ++i) {
    g->connected_to(i, connections);
    if (max < connections.size())
        max = connections.size();
    connections.clear();
  }
  return max;
}


//----------------------------------------------------------------------
// Adjacency Matrix Tests
//----------------------------------------------------------------------

// Random Graph Tests

TEST(AdjacencyMatrixTest, SmallSizedRandomGraphs) {
  int sizes[] = {5, 10, 15, 20, 25};
  for (int size : sizes) {
    string filename = "examples/rand-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyMatrix(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyMatrixTest, MediumSizedRandomGraphs) {
  int sizes[] = {500, 1000, 1500, 2000, 2500};
  for (int size : sizes) {
    // read example graph and check stats
    string filename = "examples/rand-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyMatrix(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyMatrixTest, RandomGraphInOutEdgeTest) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_EQ(6, max_in_edges(g));
  ASSERT_EQ(7, max_out_edges(g));
  delete g;
}


// Hypercube Graph Tests

TEST(AdjacencyMatrixTest, SmallSizedHypercubeGraphs) {
  int sizes[] = {5, 10, 15, 20, 25};
  for (int size : sizes) {
    string filename = "examples/hypercube-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyMatrix(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyMatrixTest, MediumSizedHypercubeGraphs) {
  int sizes[] = {500, 1000, 1500, 2000, 2500};
  for (int size : sizes) {
    string filename = "examples/hypercube-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyMatrix(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyMatrixTest, HypercubeGraphInOutEdgeTest) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_EQ(11, max_in_edges(g));
  ASSERT_EQ(11, max_out_edges(g));
  delete g;
}


// 2d Grid Graph Tests

TEST(AdjacencyMatrixTest, SmallSized2dGridGraphs) {
  int sizes[] = {5, 10, 15, 20, 25};
  for (int size : sizes) {
    string filename = "examples/grid-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyMatrix(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyMatrixTest, MediumSized2dGridGraphs) {
  int sizes[] = {500, 1000, 1500, 2000, 2500};
  for (int size : sizes) {
    string filename = "examples/grid-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyMatrix(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyMatrixTest, 2dGraphInOutEdgeTest) {
  int size = 2500;
  string filename = "examples/grid-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_EQ(4, max_in_edges(g));
  ASSERT_EQ(4, max_out_edges(g));
  delete g;
}


// Barbell Graph Tests

TEST(AdjacencyMatrixTest, SmallSizedBarbellGraphs) {
  int sizes[] = {5, 10, 15, 20, 25};
  for (int size : sizes) {
    string filename = "examples/barbell-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyMatrix(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyMatrixTest, MediumSizedBarbellGraphs) {
  int sizes[] = {500, 1000, 1500, 2000, 2500};
  for (int size : sizes) {
    string filename = "examples/barbell-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyMatrix(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyMatrixTest, BarbellGraphInOutEdgeTest) {
  int size = 2500;
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyMatrix(vertices);
  build_graph(g, filename);
  ASSERT_EQ(1248, max_in_edges(g));
  ASSERT_EQ(1248, max_out_edges(g));
  delete g;
}


//----------------------------------------------------------------------
// Adjacency List Tests
//----------------------------------------------------------------------

// Random Graph Tests

TEST(AdjacencyListTest, SmallSizedRandomGraphs) {
  int sizes[] = {5, 10, 15, 20, 25};
  for (int size : sizes) {
    string filename = "examples/rand-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyList(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyListTest, MediumSizedRandomGraphs) {
  int sizes[] = {500, 1000, 1500, 2000, 2500};
  for (int size : sizes) {
    string filename = "examples/rand-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyList(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyListTest, RandomGraphInOutEdgeTest) {
  int size = 2500;
  string filename = "examples/rand-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_EQ(6, max_in_edges(g));
  ASSERT_EQ(7, max_out_edges(g));
  delete g;
}


// Hypercube Graph Tests

TEST(AdjacencyListTest, SmallSizedHypercubeGraphs) {
  int sizes[] = {5, 10, 15, 20, 25};
  for (int size : sizes) {
    string filename = "examples/hypercube-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyList(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyListTest, MediumSizedHypercubeGraphs) {
  int sizes[] = {500, 1000, 1500, 2000, 2500};
  for (int size : sizes) {
    string filename = "examples/hypercube-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyList(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyListTest, HypercubeGraphInOutEdgeTest) {
  int size = 2500;
  string filename = "examples/hypercube-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_EQ(11, max_in_edges(g));
  ASSERT_EQ(11, max_out_edges(g));
  delete g;
}


// 2d Grid Graph Tests

TEST(AdjacencyListTest, SmallSized2dGridGraphs) {
  int sizes[] = {5, 10, 15, 20, 25};
  for (int size : sizes) {
    string filename = "examples/grid-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyList(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyListTest, MediumSized2dGridGraphs) {
  int sizes[] = {500, 1000, 1500, 2000, 2500};
  for (int size : sizes) {
    string filename = "examples/grid-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyList(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyListTest, 2dGraphInOutEdgeTest) {
  int size = 2500;
  string filename = "examples/grid-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_EQ(4, max_in_edges(g));
  ASSERT_EQ(4, max_out_edges(g));
  delete g;
}


// Barbell Graph Tests

TEST(AdjacencyListTest, SmallSizedBarbellGraphs) {
  int sizes[] = {5, 10, 15, 20, 25};
  for (int size : sizes) {
    string filename = "examples/barbell-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyList(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyListTest, MediumSizedBarbellGraphs) {
  int sizes[] = {500, 1000, 1500, 2000, 2500};
  for (int size : sizes) {
    string filename = "examples/barbell-" + to_string(size) + ".txt";
    int vertices = 0, edges = 0;
    get_stats(vertices, edges, filename);
    Graph* g = new AdjacencyList(vertices);
    build_graph(g, filename);
    ASSERT_LT(0, g->vertex_count());
    ASSERT_LT(0, g->edge_count());
    ASSERT_EQ(edges, g->edge_count());
    delete g;
  }
}

TEST(AdjacencyListTest, BarbellGraphInOutEdgeTest) {
  int size = 2500;
  string filename = "examples/barbell-" + to_string(size) + ".txt";
  int vertices = 0, edges = 0;
  get_stats(vertices, edges, filename);
  Graph* g = new AdjacencyList(vertices);
  build_graph(g, filename);
  ASSERT_EQ(1248, max_in_edges(g));
  ASSERT_EQ(1248, max_out_edges(g));
  delete g;
}


int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

