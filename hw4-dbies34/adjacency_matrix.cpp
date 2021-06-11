//----------------------------------------------------------------------
// NAME: Drew Bies
// FILE: adjacency_matrix.cpp
// DATE: Spring 2021
// DESC: Adjacency matrix implementation of the graph interface
//----------------------------------------------------------------------

#include "adjacency_matrix.h"

// checks if the vertices is in bounds
bool is_in_bounds_matrix(int v1, int v2, int vertex_count)
{
    return v2 < vertex_count && v1 < vertex_count && v1 >= 0 && v2 >= 0;
}

// checks if the vertex is in bounds
bool is_in_bounds_matrix(int v1, int vertex_count)
{
    return v1 < vertex_count && v1 >= 0;
}

// default constructor
AdjacencyMatrix::AdjacencyMatrix(int vertex_count)
{
    // check for negative vertex_count
    if(vertex_count < 0)
    {
        number_of_vertices = 0;
    } else {
        number_of_vertices = vertex_count;
    }
    
    number_of_edges = 0;

    // create array
    int size = number_of_vertices * number_of_vertices;
    adj_matrix = new int*[size];

    // initialize array
    for (int i = 0; i < size; ++i)
    {
        adj_matrix[i] = nullptr;
    }
}

// destructor
AdjacencyMatrix::~AdjacencyMatrix()
{
    make_empty();
}

// copy constructor
AdjacencyMatrix::AdjacencyMatrix(const AdjacencyMatrix& rhs)
{
    number_of_vertices = 0;
    number_of_edges = 0;

    // call assignment operator
    *this = rhs;
}

// assignment operator
AdjacencyMatrix& AdjacencyMatrix::operator=(const AdjacencyMatrix& rhs)
{
    if(&rhs != this)
    {
        // free memory
        //make_empty(); // causes double free error

        number_of_vertices = rhs.number_of_vertices;
        number_of_edges = rhs.number_of_edges;

        int size = number_of_vertices * number_of_vertices;

        // create new array
        adj_matrix = new int*[size];

        // copy array
        for (int i = 0; i < size; ++i)
        {
            adj_matrix[i] = rhs.adj_matrix[i];
        }
    }
    return *this;
}

// add or update an edge between existing vertices in the graph
void AdjacencyMatrix::set_edge(int v1, int edge_label, int v2)
{
    // check if out of bounds
    if (!is_in_bounds_matrix(v1, v2, number_of_vertices))
    {
        return;
    }
    // increment if edge does not exist
    if(!has_edge(v1, v2))
    {
        number_of_edges++;
    }
    adj_matrix[index(v1, v2)] = &edge_label;
}

// check if the given edge is in the graph
bool AdjacencyMatrix::has_edge(int v1, int v2) const
{
    // first check if out of bounds
    if (!is_in_bounds_matrix(v1, v2, number_of_vertices))
    {
        return false;
    } else {
        return adj_matrix[index(v1, v2)] != nullptr;
    }
}

// returns true if there is an edge between v1 and v2 in the graph
// the output parameter returns the edge label
bool AdjacencyMatrix::get_edge(int v1, int v2, int& edge) const
{
    if (has_edge(v1, v2))
    {
        edge = *adj_matrix[index(v1, v2)];
        return true;
    }
    else{
        return false;
    }
}
  
// get all vertices on an outgoing edge from given vertex
void AdjacencyMatrix::connected_to(int v1, std::list<int>& vertices) const
{
    // check if the vertex is in bounds
    if(!is_in_bounds_matrix(v1, number_of_vertices))
    {
        return;
    }
    // index through the row
    int startIndex = v1 * number_of_vertices;
    for(int i = startIndex; i < startIndex + number_of_vertices; ++i)
    {
        if(adj_matrix[i] != nullptr)
        {
            vertices.push_front(i - number_of_vertices * v1);
        }
    }
}

// get all vertices on an incoming edge to given vertex
void AdjacencyMatrix::connected_from(int v2, std::list<int>& vertices) const
{
    // check if the vertex is in bounds
    if(!is_in_bounds_matrix(v2, number_of_vertices))
    {
        return;
    }
    // index down the column
    for(int i = v2; i < number_of_vertices * number_of_vertices; i += number_of_vertices)
    {
        if(adj_matrix[i] != nullptr)
        {
            vertices.push_front(i / number_of_vertices);
        }
    }
}

// get all vertices adjacent to a vertex, that is, connected to or connected
// from the vertex (may return duplicate vertices)
void AdjacencyMatrix::adjacent(int v, std::list<int>& vertices) const
{
    connected_to(v, vertices);
    connected_from(v, vertices);
}

// get number of nodes in the graph
int AdjacencyMatrix::vertex_count() const
{
    return number_of_vertices;
}

// get number of edges in the graph
int AdjacencyMatrix::edge_count() const
{
    return number_of_edges;
}

// return the matrix index for edge x->y
int AdjacencyMatrix::index(int x, int y) const
{
    return (x * number_of_vertices) + y;
}

// delete the adjacency matrix
void AdjacencyMatrix::make_empty()
{
    delete[] adj_matrix;
}
