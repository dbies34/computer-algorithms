//----------------------------------------------------------------------
// NAME: Drew Bies
// FILE: adjacency_list.cpp
// DATE: Spring 2021
// DESC: Adjacency list implementation of the graph interface
//----------------------------------------------------------------------

#include "adjacency_list.h"

// checks if the vertices is in bounds
bool is_in_bounds_list(int v1, int v2, int vertex_count)
{
    return v2 < vertex_count && v1 < vertex_count && v1 >= 0 && v2 >= 0;
}

// checks if the vertex is in bounds
bool is_in_bounds_list(int v1, int vertex_count)
{
    return v1 < vertex_count && v1 >= 0;
}

// default constructor
AdjacencyList::AdjacencyList(int vertex_count)
{
    // check for negative vertex count
    if(vertex_count < 0)
    {
        number_of_vertices = 0;
    } else {
        number_of_vertices = vertex_count;
    }
    
    number_of_edges = 0;

    // create adjacency lists
    int size = number_of_vertices;
    adj_list_out = new Node*[size];
    adj_list_in = new Node*[size];

    // initialize
    for (int i = 0; i < size; ++i)
    {
        adj_list_out[i] = nullptr;
        adj_list_in[i] = nullptr;
    }
}

// destructor
AdjacencyList::~AdjacencyList()
{
    make_empty();
}

// copy constructor
AdjacencyList::AdjacencyList(const AdjacencyList& rhs)
{
    // intialize the lists
    number_of_vertices = 0;
    number_of_edges = 0;
    adj_list_in = nullptr;
    adj_list_out = nullptr;
    // call assignment operator
    *this = rhs;
}

// assignment operator
AdjacencyList& AdjacencyList::operator=(const AdjacencyList& rhs)
{
    if(&rhs != this)
    {
        // free memory
        make_empty();
        
        // make new arrays
        int size = rhs.number_of_vertices;
        adj_list_out = new Node *[size];
        adj_list_in = new Node *[size];

        // initialize
        for (int i = 0; i < size; ++i)
        {
            adj_list_out[i] = nullptr;
            adj_list_in[i] = nullptr;

            Node* currPtr = rhs.adj_list_out[i];
            while (currPtr != nullptr)
            {
                Node* newNode = new Node();
                newNode -> next = adj_list_out[i];
                newNode -> edge = currPtr -> edge;
                newNode -> vertex = currPtr -> vertex;
                adj_list_out[i] = newNode;
                currPtr = currPtr -> next;
            }

            currPtr = rhs.adj_list_in[i];
            while (currPtr != nullptr)
            {
                Node* newNode = new Node();
                newNode -> next = adj_list_in[i];
                newNode -> edge = currPtr -> edge;
                newNode -> vertex = currPtr -> vertex;
                adj_list_in[i] = newNode;
                currPtr = currPtr -> next;
            }
        }
        number_of_vertices = size;
        number_of_edges = rhs.number_of_edges;
    }
    return *this;
}

// add or update an edge between existing vertices in the graph
void AdjacencyList::set_edge(int v1, int edge_label, int v2)
{
    // check if the vertices are out of bounds
    if(!is_in_bounds_list(v1, v2, number_of_vertices)) 
    {
        return;
    }

    // if edge exists, update both lists, otherwise create node in both lists
    if(!has_edge(v1, v2))
    {
        // create new node for in list and add to front 
        Node* inNode = new Node();
        inNode -> next = adj_list_in[v2];
        inNode -> edge = edge_label;
        inNode -> vertex = v1;
        adj_list_in[v2] = inNode;

        // create new node for out list and add to front
        Node* outNode = new Node();
        outNode -> next = adj_list_out[v1];
        outNode -> edge = edge_label;
        outNode -> vertex = v2;
        adj_list_out[v1] = outNode;

        // increment number of edges
        number_of_edges++;
    }
    else
    {
        // update adj in
        Node* inPtr = adj_list_in[v2];
        while(inPtr -> vertex != v1)
        {
            inPtr = inPtr -> next;
        }
        inPtr -> edge = edge_label;

        // update adj out
        Node* outPtr = adj_list_out[v1];
        while(outPtr -> vertex != v2)
        {
            outPtr = outPtr -> next;
        }
        outPtr -> edge = edge_label;
    }
}

// check if the given edge is in the graph
bool AdjacencyList::has_edge(int v1, int v2) const
{
    // check if vertex is out of bounds
    if(!is_in_bounds_list(v2, number_of_vertices))
    {
        return false;
    }
    // only one array needs to be checked
    // check adj in
    Node* inPtr = adj_list_in[v2];
    while(inPtr != nullptr)
    {
        if (inPtr -> vertex == v1)
        {
            return true;           
        }
        inPtr = inPtr -> next;
    }
    return false;
}

// returns true if there is an edge between v1 and v2 in the graph
// the output parameter returns the edge label
bool AdjacencyList::get_edge(int v1, int v2, int& edge) const
{
    // check if vertex is out of bounds
    if(!is_in_bounds_list(v2, number_of_vertices))
    {
        return false;
    }
    // only one array needs to be checked
    // check adj in
    Node* inPtr = adj_list_in[v2];
    while(inPtr != nullptr)
    {
        if (inPtr -> vertex == v1)
        {
            edge = inPtr -> edge;
            return true;           
        }
        inPtr = inPtr -> next;
    }
    return false;
}
  
// get all vertices on an outgoing edge from given vertex
void AdjacencyList::connected_to(int v1, std::list<int>& vertices) const
{
    // check if vertex is out of bounds
    if(!is_in_bounds_list(v1, number_of_vertices))
    {
        return;
    }
    // retrieve all vertices from the adj out list
    Node* outPtr = adj_list_out[v1];
    while (outPtr != nullptr)
    {
        vertices.push_front(outPtr -> vertex);
        outPtr = outPtr -> next;
    }
}

// get all vertices on an incoming edge to given vertex
void AdjacencyList::connected_from(int v2, std::list<int>& vertices) const
{
    // check if vertex is out of bounds
    if(!is_in_bounds_list(v2, number_of_vertices))
    {
        return;
    }
    // retrieve all vertices from the adj in list
    Node* inPtr = adj_list_in[v2];
    while (inPtr != nullptr)
    {
        vertices.push_front(inPtr -> vertex);
        inPtr = inPtr -> next;
    }
}

// get all vertices adjacent to a vertex, that is, connected to or connected
// from the vertex (may return duplicate vertices)
void AdjacencyList::adjacent(int v, std::list<int>& vertices) const
{
    connected_to(v, vertices);
    connected_from(v, vertices);
}

// get number of nodes in the graph
int AdjacencyList::vertex_count() const
{
    return number_of_vertices;
}

// get number of edges in the graph
int AdjacencyList::edge_count() const
{
    return number_of_edges;
}

// helper function to delete adj_list
void AdjacencyList::make_empty()
{
    for(int i = 0; i < number_of_vertices; ++i)
    {
        Node* delPtr = adj_list_out[i];
        while(delPtr != nullptr)
        {
            Node* tempPtr = delPtr -> next;
            delete delPtr;
            delPtr = tempPtr;
        }
        // delete all nodes from in list
        delPtr = adj_list_in[i];
        while(delPtr != nullptr)
        {
            Node* tempPtr = delPtr -> next;
            delete delPtr;
            delPtr = tempPtr;
        }
    }
    delete[] adj_list_in;
    delete[] adj_list_out;
}