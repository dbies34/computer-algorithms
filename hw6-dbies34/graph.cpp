//----------------------------------------------------------------------
// NAME: Drew Bies
// FILE: graph.cpp
// DATE: Spring 2021
// DESC: A directed graph consists of vertices and edges. Each vertex
// has a unique identifier (from 0 to n-1) and each edge has a
// (non-unique) integer label.
//----------------------------------------------------------------------

#include "graph.h"
#include <stack>
#include <iostream>
#include <queue>

using namespace std;

//----------------------------------------------------------------------
// HW-3 graph operations
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Breadth-first search from a given source vertex.
//
// Inputs:
//   dir -- true if directed
//   src -- the source vertex
// Outputs:
//   tree -- search tree that maps vertices found during bfs from the
//           source to their parent vertices
//----------------------------------------------------------------------
void Graph::bfs(bool dir, int src, Map &tree) const
{
    int size = vertex_count();

    // create explored array
    bool explored[size];

    // mark all vertices as unexplored
    for (int i = 0; i < size; i++)
    {
        explored[i] = false;
    }

    // mark vertex v (src) as explored
    explored[src] = true;

    // set the parent of s to -1 (denoting the root)
    tree[src] = -1;

    list<int> queue;
    queue.push_back(src);

    while (!queue.empty())
    {
        int currVertex = queue.front();

        queue.pop_front();

        list<int> vertices;
        if (dir)
        {
            // use connected_to() for directed
            connected_to(currVertex, vertices);
        }
        else
        {
            // use adjacent() for undirected
            adjacent(currVertex, vertices);
        }

        for (list<int>::iterator it = vertices.begin(); it != vertices.end(); ++it)
        {
            int vertex = *it;
            if (!explored[vertex])
            {
                // mark vertex as explored
                explored[vertex] = true;
                // set parent of adjacent vertex to current vertex
                tree[vertex] = currVertex;
                // add to the end of queue
                queue.push_back(vertex);
            }
        }
    }
}

//----------------------------------------------------------------------
// Shortest path length from source to destination vertices.
//
// Conditions:
//   The source and destination vertices must be unique.
// Inputs:
//   dir -- true if directed
//   src -- the vertex starting the path
//   dst -- the vertex ending the path
// Outputs:
//   path -- sequence of nodes that define the shortest path
//----------------------------------------------------------------------
void Graph::shortest_path_length(bool dir, int src, int dst, std::list<int> &path) const
{
    // The source and destination vertices must be unique
    if (src == dst)
        return;

    Map searchTree;
    bfs(dir, src, searchTree);

    // return if the destination cannot be reached
    if (searchTree.find(dst) == searchTree.end())
    {
        path.push_front(src);
        return;
    }
    path.push_front(dst);

    int currVertex = searchTree[dst];
    while (currVertex != src)
    {
        path.push_front(currVertex);
        currVertex = searchTree[currVertex];
    }
    path.push_front(src);
}

//----------------------------------------------------------------------
// Find connected components based on breadth-first search.
//
// Conditions:
//   Finds strongly connected components in an undirected graph and
//   weakly-connected components in a directed graph.
// Inputs:
//   None
// Outputs:
//   components -- mapping from each graph vertex to its corresponding
//                 component number where component numbers range from
//                 0 to c-1 (for c components)
//----------------------------------------------------------------------
void Graph::bfs_connected_components(Map &components) const
{
    // make a list of all the undiscovered vertices
    list<int> undiscovered;
    for (int i = 0; i < vertex_count(); ++i)
    {
        undiscovered.push_back(i);
    }

    int componentNum = 0;
    Map searchTree;
    while (!undiscovered.empty())
    {
        bfs(false, undiscovered.front(), searchTree);

        // iterate through the search tree
        for (Map::iterator it = searchTree.begin(); it != searchTree.end(); ++it)
        {
            // set the vertex to the current component number
            components[it->first] = componentNum;
            // remove the vertex from the undiscovered list
            undiscovered.remove(it->first);
        }
        searchTree.clear();
        componentNum++;
    }
}

//----------------------------------------------------------------------
// Determine if the graph is bipartite (i.e., 2-colorable)
//
// Inputs:
//   None
// Outputs:
//   returns  -- true if the graph is bipartite, false otherwise
//   coloring -- mapping from each graph vertex to its corresponding
//               color (either 0 or 1) if graph is bipartite
//----------------------------------------------------------------------
bool Graph::bipartite_graph(Map &coloring) const
{
    list<int> undiscovered;

    // set all the vertices to no color
    for (int i = 0; i < vertex_count(); ++i)
    {
        coloring[i] = -1;
        undiscovered.push_front(i);
    }

    coloring[0] = 1;

    list<int> queue;
    queue.push_front(0);
    undiscovered.remove(0);

    while (!undiscovered.empty() || !queue.empty())
    {
        int u = queue.front();
        queue.pop_front();

        list<int> adjacentVertices;
        adjacent(u, adjacentVertices);

        for (list<int>::iterator it = adjacentVertices.begin(); it != adjacentVertices.end(); ++it)
        {
            int vertex = *it;

            // check for no coloring
            if (coloring[vertex] == -1)
            {
                undiscovered.remove(vertex);
                // color the vertex the opposite of the parent vertex
                coloring[vertex] = 1 - coloring[u];
                // add to the queue
                queue.push_front(vertex);
            }
            // check if the current vertex color is the same as the parent color
            else if (coloring[vertex] == coloring[u])
            {
                return false;
            }
        }
        if (queue.empty() && !undiscovered.empty())
        {
            queue.push_front(undiscovered.front());
            undiscovered.pop_front();
        }
    }
    return true;
}

//----------------------------------------------------------------------
// Depth-first search from a given source vertex.
//
// Inputs:
//   dir -- if true assumes graph is directed
//   src -- the source vertex
// Outputs:
//   tree -- search tree that maps vertices found during dfs to their
//           corresponding parent vertex.
//----------------------------------------------------------------------
void Graph::dfs(bool dir, int src, Map &tree) const
{
    bool explored[vertex_count()];

    // mark all vertices as unexplored
    for (int i = 0; i < vertex_count(); i++)
    {
        explored[i] = false;
    }

    stack <int> s;

    s.push(src);
    // src is the root of the tree
    tree[src] = -1;
    explored[src] = true;

    while (!s.empty())
    {
        // parent vertex
        int u = s.top();
        
        list <int> vertices;

        if (dir)
        {
            // use connected_to() for directed
            connected_to(u, vertices);
        }
        else
        {
            // use adjacent() for undirected
            adjacent(u, vertices);
        }

        bool isDeadEnd = true;
        for (list<int>::iterator it = vertices.begin(); it != vertices.end(); ++it)
        {
            // current vertex
            int v = *it;

            if (!explored[v])
            {
                tree[v] = u;
                explored[v] = true;
                s.push(v);
                isDeadEnd = false;
            }
        }

        // pop the stack if there is a dead end
        if (isDeadEnd)
            s.pop();
    }
}

//----------------------------------------------------------------------
// Determine if the graph is acyclic or not.
//
// Inputs:
//   dir -- if true assumes graph is directed
// Outputs:
//   returns -- true if acyclic
//----------------------------------------------------------------------
bool Graph::acyclic(bool dir) const
{
    Map coloring;

    for (int i = 0; i < vertex_count(); i++)
    {
        // set every color to 0 (WHITE)
        coloring[i] = 0;
    }

    for (int i = 0; i < vertex_count(); i++)
    {
        if (coloring[i] != 2)
        {
            stack <int> s;
            s.push(i);

            while (!s.empty())
            {
                list <int> vertices;
                int u = s.top();

                // set the color to 1 (GRAY)
                coloring[u] = 1;

                if (dir)
                {
                    // use connected_to() for directed
                    connected_to(u, vertices);
                }
                else
                {
                    // use adjacent() for undirected
                    adjacent(u, vertices);
                }

                bool deadEnd = true;
                // kepp track of adj grays for undirected graphs
                int grayCount = 0;
                for (list<int>::iterator it = vertices.begin(); it != vertices.end(); ++it)
                {
                    int v = *it;
                    
                    if(coloring[v] == 0)
                    {
                        s.push(v);
                        deadEnd = false;

                    } else if(coloring[v] == 1)
                    {
                        grayCount++;
                        // undirected cycles must be touching two gray vertices
                        if (dir || (grayCount > 1 && !dir))
                            return false;
                    }
                    
                }
                // set the vertex to black if its a dead end
                if (deadEnd || (!dir && grayCount == 1 && vertices.size() == 1))
                {
                    coloring[u] = 2;
                    s.pop();
                }
            }
        }
        
    }
    return true;
}

//----------------------------------------------------------------------
// Brute force implementation to compute the transitive closure of
// the current graph without consideration of edge weights.
//
// Conditions: Assumes that the given graph (the closed_graph) is a
//             copy of the current graph prior to the call.
//
// Inputs:
//   dir -- if true assumes graph is directed
// Outputs:
//   closed_graph -- the transitively closed graph, where added
//                   edges have
//----------------------------------------------------------------------
void Graph::unweighted_transitive_closure(bool dir, Graph &closed_graph) const
{
    
    int size = vertex_count();

    for (int i = 0; i < size; i++)
    {
        stack <int> s;
        s.push(i);
        
        bool explored[vertex_count()] = {false};

        explored[i] = true;

        while (!s.empty())
        {
            list <int> vertices;
            // parent vertex
            int u = s.top();

            if (dir)
            {
                // use connected_to() for directed
                connected_to(u, vertices);
            }
            else
            {
                // use adjacent() for undirected
                adjacent(u, vertices);
            }

            bool isDeadEnd = true;
            for (list<int>::iterator it = vertices.begin(); it != vertices.end(); ++it)
            {
                // current vertex
                int v = *it;

                if (!explored[v])
                {
                    closed_graph.set_edge(i, 0, v);
                    explored[v] = true;
                    s.push(v);
                    isDeadEnd = false;
                }
            }

            // pop if there is a dead end
            if (isDeadEnd)
                s.pop();
        }
    }
}

//----------------------------------------------------------------------
// Computes a topological sort of the current graph based on dfs.
//
// Conditions: Assumes the graph is directed.
// Outputs:
//
//   vertex_ordering -- a map from vertex to its corresponding
//                      order in the topological sort (where nodes
//                      are ordered from 1 to n)
//----------------------------------------------------------------------
void Graph::dfs_topological_sort(Map &vertex_ordering) const
{
    bool explored[vertex_count()] = {false};
    bool addedToStack[vertex_count()] = {false};

    int size = vertex_count();
    

    stack <int> topStack;

    // look through every unexplored vertex
    for (int i = 0; i < size; i++)
    {
        if (!explored[i])
        {
            stack <int> s;
            s.push(i);
            
            while (!s.empty())
            {
                list <int> vertices;
                int u = s.top();

                explored[u] = true;

                // use connected_to() for directed
                connected_to(u, vertices);

                bool isDeadEnd = true;
                for (list<int>::iterator it = vertices.begin(); it != vertices.end(); ++it)
                {
                    int v = *it;

                    // check for a dead end
                    if (!explored[v])
                    {
                        isDeadEnd = false;
                        s.push(v);
                    }
                }

                // if its a new dead end add to the sorting stack and pop
                if (isDeadEnd)
                {
                    if (!addedToStack[u])
                    {
                        topStack.push(u);
                        addedToStack[u] = true;
                    }
                    s.pop();
                }
            }
        }
    }

    // add the sorting stack to the ordering map
    for (int i = 1; i <= size; i++)
    {
        vertex_ordering[topStack.top()] = i;
        topStack.pop();
    }
}

//----------------------------------------------------------------------
// HW-5 graph operations
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Computes the strongly connected components.
//
// Inputs:
//   none
// Outputs: 
//   components -- mapping from each graph vertex to its corresponding
//                 component number where component numbers range from
//                 0 to c-1 (for c components)
//----------------------------------------------------------------------
void Graph::strongly_connected_components(Map& components) const
{
    bool explored[vertex_count()] = {false};
    int clock = 0;
    Map postTimes;
    list<int> ordering;

    // look through every unexplored vertex
    for (int i = 0; i < vertex_count(); i++)
    {
        if (!explored[i])
        {
            stack <int> s;
            clock++;

            s.push(i);

            explored[i] = true;

            while (!s.empty())
            {
                // parent vertex
                int u = s.top();
                
                list <int> vertices;

                // use connected_from() for reverse graph
                connected_from(u, vertices);

                bool isDeadEnd = true;
                for (list<int>::iterator it = vertices.begin(); it != vertices.end(); ++it)
                {
                    // current vertex
                    int v = *it;

                    if (!explored[v])
                    {
                        clock++;
                        explored[v] = true;
                        s.push(v);
                        isDeadEnd = false;
                    }
                }

                // pop the stack if there is a dead end
                if (isDeadEnd)
                {
                    // set the post time of the vertex
                    postTimes[s.top()] = clock++;
                    // add to the vertex ordering 
                    ordering.push_back(s.top());
                    s.pop();
                }
            }
        }
    }

    // build the components map
    int compNum = 0, lastPostTime = postTimes[ordering.front()] - 1;
    while (!ordering.empty())
    {
        // grab the current vertex
        int currVertex = ordering.front();
        if (lastPostTime == postTimes[currVertex] - 1)
        {
            components[currVertex] = compNum;
        }
        else {
            components[currVertex] = ++compNum;
        }
        // increment the last post time
        lastPostTime = postTimes[currVertex];
        ordering.pop_front();
    }
}

//----------------------------------------------------------------------
// Computes the transitive reduction.
//
// Conditions: Assumes that the given graph (the closed_graph) has
//             the same number of nodes as the current graph. But
//             does not have any edges prior to the call
//
// Inputs:
//   none
// Outputs:
//   reduced_graph -- the reduced edges are added to the reduced graph
//----------------------------------------------------------------------
void Graph::transitive_reduction(Graph& reduced_graph) const
{
    Map strongComps;
    strongly_connected_components(strongComps);

    // make a list of a single vertex from each SCC
    list<int> basicComps;

    // keep track
    int lastComp = -1, firstVertex = -1, lastVertex = -1, index = 0;

    // build the reduced graph with the SCC first
    for (Map::iterator it = strongComps.begin(); it != strongComps.end(); ++it)
    {
        // keep track of how many vertices were looked at
        index++;
        // grab the current vertex and the component
        int currVertex = it -> first;
        int currComp = it -> second;

        if (currComp == lastComp)
        {
            // set the new edge
            reduced_graph.set_edge(lastVertex, 0, currVertex);
            // increment the last vertex
            lastVertex = currVertex;

            // check for last vertex in the map
            if (index == vertex_count())
            {
                reduced_graph.set_edge(currVertex, 0, firstVertex);
            }
        }
        else {
            basicComps.push_back(currVertex);
            // make sure its not the first vertex in the map
            if (lastComp != -1 && lastVertex != firstVertex)
            {
                reduced_graph.set_edge(lastVertex, 0, firstVertex);
            } 
            // on a new component so increment last component and the first vertex
            lastComp = currComp;
            firstVertex = currVertex;
            lastVertex = currVertex;
        }
    }

    // connect the basic cycles together
    while (!basicComps.empty())
    {
        int currVertex = basicComps.front();
        for (int i = 0; i < vertex_count(); i++)
        {
            // dont add more edges between vertices in the same SCC
            if (strongComps[currVertex] != strongComps[i])
            {
                list<int> shortestPath, shortestPathReduced;
                shortest_path_length(true, currVertex, i, shortestPath);
                reduced_graph.shortest_path_length(true, currVertex, i, shortestPathReduced);
                
                // shortest path returns size 1 if the destination cannot be reached
                // check if the edge needs to be on the reduced graph
                if(shortestPath.size() != 1 && currVertex != i && shortestPathReduced.size() == 1)
                {
                    // add the edge to the reduced graph if the two vertices are connected in the original graph
                    reduced_graph.set_edge(currVertex, 0, i);
                    shortestPath.clear();
                }
                shortestPathReduced.clear();
            }
            
        }
        basicComps.pop_front();
    }
}

//----------------------------------------------------------------------
// Check if an eulerian exists in a directed graph, and if so,
// return one.
//
// Conditions: Assumes the graph is not disconnected.
//
// Inputs:
//   none
// Outputs:
//   path -- the path as an ordered list of vertices
//----------------------------------------------------------------------
bool Graph::directed_eulerian_path(std::list<int>& path) const
{
    int edgesLeft[vertex_count()];
    int sourceVertex = 0, sinkVertex = 0;
    // keep track of the number of sinks, sources, and the vertices with the same number of in and out edges
    int numOfSinks = 0, numOfSrcs = 0, numOfNormEdges = 0;
    for (int i = 0; i < vertex_count(); i++)
    {
        list<int> vertices;
        connected_to(i, vertices);
        int outEdges = vertices.size();
        vertices.clear();
        connected_from(i, vertices);
        int inEdges = vertices.size();
        edgesLeft[i] = outEdges;

        // sort the vertices based on their edge counts
        if(outEdges == inEdges + 1)
        {
            sourceVertex = i;
            numOfSrcs++;
        }
        else if (inEdges == outEdges + 1)
        {
            sinkVertex == i;
            numOfSinks++;
        }
        else if (inEdges == outEdges)
        {
            numOfNormEdges++;
        }
    }

    // at most one vertex has one less in-edge than out-edge (a “source”)
    // at most one vertex has one less out-edge than in-edge (a “sink”)
    // all other vertices have the same number of in-edges and out-edges
    if (numOfNormEdges != vertex_count() - numOfSinks - numOfSrcs || numOfSinks > 1 || numOfSrcs > 1)
        return false;


    // keep track of the edges used up
    Map edgesUsed;

    stack<int> s;
    s.push(sourceVertex);

    while (!s.empty())
    {
        int u = s.top();

        // add the vertex if its a dead end
        if (edgesLeft[u] == 0)
        {
            path.push_front(u);
            s.pop();
        // else traverse the graph
        } else {
            list<int> vertices;

            connected_to(u, vertices);
            bool foundVertex = false;

            for (list<int>::iterator it = vertices.begin(); it != vertices.end(); ++it)
            {
                // current vertex
                int v = *it;
                bool hasEdgeBeenUsed = false;

                // check if the edge has been used before
                for (Map::iterator it = edgesUsed.begin(); it != edgesUsed.end(); ++it)
                {
                    if (u == it -> first && v == it -> second)
                    {
                        hasEdgeBeenUsed = true;
                        break;
                    }
                }

                // add the vertex to the stack and mark it as used
                if (!hasEdgeBeenUsed)
                {
                    edgesUsed[u] = v;
                    edgesLeft[u]--;
                    s.push(v);
                    foundVertex = true;
                    break;
                }
            }
        }
    }

    return true;
}

//----------------------------------------------------------------------
// HW-6 graph operations
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Returns a Hamiltonian path if one exists in the current graph.
//
// Conditions: Treats the graph as directed.
//
// Outputs:
//   path -- the Hamiltonian path
//
// Returns:
//   true if a Hamiltonian path exists, false otherwise.
//----------------------------------------------------------------------
bool Graph::directed_hamiltonian_path(std::list<int>& path) const
{
    for (int i = 0; i < vertex_count(); i++)
    {
        bool discovered[vertex_count()] = {false};
        path.push_front(i);
        discovered[i] = true;

        if (directed_hamiltonian_rec(i, path, discovered))
        {
            return true;
        }
        else {
            path.clear();
        }
    }
    return false;
}

//----------------------------------------------------------------------
// Find a maximum matched graph using the augmenting paths algorithm
// from the textbook.
//
// Conditions: Finds a matching only if the current graph is bipartite
//
// Output:
//   max_matched_graph -- A graph with same vertices as original
//                        graph, containing the edges in the
//                        matching. The output graph is assumed to
//                        be initialized with the same number of
//                        vertices, but without any edges.
// Returns:
//   true if the current graph is bipartite, false otherwise
//----------------------------------------------------------------------
bool Graph::bipartite_graph_matching(Graph& max_matched_graph) const
{    
    Map coloring;
    // return false if the graph is not bipartite
    if(!bipartite_graph(coloring)) 
        return false;
    
    // init the labels
    int labels[vertex_count()];
    for(int v = 0; v < vertex_count(); v++) 
        labels[v] = -1;

    list<int> V, U;
    queue<int> q;

    for(auto p : coloring) 
    {
        if(p.second == 0) 
            V.push_back(p.first);
        else 
            U.push_back(p.first);
    }

    // put all the vertices from V into the queue
    for(int v : V) 
        q.push(v);
    
    while(!q.empty()) 
    {
        int w = q.front(); 
        q.pop();
        bool inV = false;

        for(int v : V)
        {
            if(w == v) 
            {
                inV=true; 
                break;
            }
        }

        // if w is in the V list 
        if(inV) {
            list<int> vertices;
            adjacent(w, vertices);

            for(int u : vertices) 
            {
                list<int> adj;
                max_matched_graph.adjacent(u, adj);
                // check if u is free
                if(adj.empty()) 
                {
                    max_matched_graph.set_edge(w, 0, u);
                    int v = w;

                    while(labels[v] != -1) 
                    {
                        u = labels[v];
                        max_matched_graph.remove_edge(v, u);
                        v = labels[u];
                        max_matched_graph.set_edge(v, 0, u);
                    }

                    // reset the labels
                    for(int i = 0; i < vertex_count(); i++) 
                        labels[i] = -1;

                    for(int v : V) 
                    {
                        list<int> adj;
                        max_matched_graph.adjacent(v, adj);
                        if(adj.size() == 0) q.push(v);
                    } 
                    break;
                // u is matched
                } else {                                                                                                                                                              
                    if(!max_matched_graph.has_edge(w,u) && labels[u] == -1) 
                    {
                        labels[u] = w;
                        q.push(u);
                    }
                }
            }
        // w in U and matched   
        } else {                                                                                                                                                         
            list<int> vertices;
            max_matched_graph.adjacent(w, vertices);
            int v;
            if(vertices.size() == 1) 
                v = vertices.front();
            labels[v] = w;
            q.push(v);
        }
    }
    return true;
}

//----------------------------------------------------------------------
// Finds all (maximal) cliques in the graph using the Bron-Kerbosch
// algorithm.
//
// Conditions: Assumes the graph is undirected.
//
// Output:
//   cliques -- a list of list of vertices denoting a maximal clique
//----------------------------------------------------------------------
void Graph::cliques(std::list<Set>& cliques) const
{
    Set p, r, x;

    // fill p with all the possible vertices
    for (int i = 0; i < vertex_count(); i++)
    {
        p.insert(i);
    }
    cliques_rec(p, r, x, cliques);
}
  

// helper function for directed hamiltonian recursive function
bool Graph::directed_hamiltonian_rec(int v, std::list<int>& path, bool discovered[]) const
{
    if (path.size() == vertex_count())
    {
        return true;
    } else {
        list<int> vertices;
        connected_to(v, vertices);

        for (list<int>::iterator it = vertices.begin(); it != vertices.end(); ++it)
        {
            int u = *it;
            if (!discovered[u])
            {
                discovered[u] = true;
                path.push_back(u);
                if (directed_hamiltonian_rec(u, path, discovered))
                {
                    return true;
                } else {
                    path.remove(u);
                }
            }
        }
    }
    return false;
}

// helper function for finding all cliques
void Graph::cliques_rec(Set& p, Set& r, Set& x, std::list<Set>& cliques) const
{
    if (p.empty() && x.empty())
    {
        cliques.push_back(r);
        return;
    } else {

        Set pCopy = p;
        for (Set::iterator it = p.begin(); it != p.end(); ++it)
        {
            int v = *it;
            list<int> adjVertices;
            adjacent(v, adjVertices);
            Set adj(adjVertices.begin(), adjVertices.end());

            list<int> tmp;
            set_intersection(pCopy.begin(), pCopy.end(), adj.begin(), adj.end(), back_inserter(tmp));
            Set newP(tmp.begin(), tmp.end());

            tmp.clear();

            Set newR = r;
            newR.insert(v);

            set_intersection(x.begin(), x.end(), adj.begin(), adj.end(), back_inserter(tmp));
            Set newX(tmp.begin(), tmp.end());

            cliques_rec(newP, newR, newX, cliques);
            pCopy.erase(v);
            x.insert(v);
        }
        return;
    }
}