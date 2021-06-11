//----------------------------------------------------------------------
// NAME: S. Bowers
// FILE: graph.h
// DATE: Spring 2021
// DESC: A directed graph consists of vertices and edges. Each vertex
// has a unique identifier (from 0 to n-1) and each edge has a
// (non-unique) integer label.
//----------------------------------------------------------------------

#ifndef GRAPH_H
#define GRAPH_H

#include <list>


class Graph
{
public:

  // default destructor
  virtual ~Graph() {}

  // add or update an edge between existing vertices in the graph
  virtual void set_edge(int v1, int edge_label, int v2) = 0;

  // check if the given edge is in the graph
  virtual bool has_edge(int v1, int v2) const = 0;  

  // returns true if there is an edge between v1 and v2 in the graph
  // the output parameter returns the edge label
  virtual bool get_edge(int v1, int v2, int& edge) const = 0; 
  
  // get all vertices on an outgoing edge from given vertex
  virtual void connected_to(int v1, std::list<int>& vertices) const = 0;

  // get all vertices on an incoming edge to given vertex
  virtual void connected_from(int v2, std::list<int>& vertices) const = 0;

  // get all vertices adjacent to a vertex, that is, connected to or connected
  // from the vertex (may return duplicate vertices)
  virtual void adjacent(int v, std::list<int>& vertices) const = 0;

  // get number of nodes in the graph
  virtual int vertex_count() const = 0; 

  // get number of edges in the graph
  virtual int edge_count() const = 0; 

};


#endif
