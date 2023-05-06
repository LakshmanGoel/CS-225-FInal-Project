/**
 * @file Airports.h
 * Airport class header file
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <set>
#include <list>


using namespace std;
typedef std::vector<std::vector<std::string> > V2D; 

struct Coordinates {
    Coordinates();

    Coordinates(double A, double B);

    double lat;
    double lon;
    unsigned dist(Coordinates B);
};

struct Airports {
    struct vertex {
        vertex();
        vertex(Coordinates loc, string name);
        Coordinates location;
        string port_name;
        bool valid;
        
    };

    struct edge {
        edge();
        edge(vertex* source, vertex* destnation, unsigned distance);
        unsigned dist;
        vertex* source;
        vertex* destination; 
    };

    struct priority_vertex {
        priority_vertex(string name, unsigned distance);

        string name_port;
        unsigned dist;
        bool operator<(const priority_vertex other) const;
    };

    vector<pair<vertex,list<edge*>>> ports;
    list<edge> flights;


    Airports(V2D airports, V2D routes, unsigned num_of_airports);
    Airports(V2D airports, V2D routes, unsigned num_of_airports, bool distance);

    list<edge*> incidentEdges(string port);
    vector<string> neighbors(string port);
    bool areAdj(string src, string dest);
    unsigned dist(string A, string B);

    /**
    * @brief generates a spanning tree of the graph from a given node.
    * 
    * @param src Unique ID of starting airport
    * @param dest Unique ID of destination airport
    * 
    * @return A pair including string and unsigned int. The strings describe the nodes along the path and the unsigned int describe for each node is the distance from the source
    */
    vector<pair<string,unsigned>> Dijkstra(string src, string dest);

    /**
    * @brief generates a spanning tree of the graph from a given node.
    * 
    * @param start_name name of airport to start from
    * 
    * @return A list of 3-tuples with the following elements:
    *         The first element names the current node
    *         The second element names the previous node (node at depth
    *         1 less than current node).
    *         The third element is the depth of the current node.
    */
    vector<tuple<string, string, int>> Spanning_bfs (string start_name);
    
};