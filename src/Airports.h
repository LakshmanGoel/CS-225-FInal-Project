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

    list<edge*> incidentEdges(string port);
    vector<string> neighbors(string port);
    bool areAdj(string src, string dest);
    unsigned dist(string A, string B);
    vector<pair<string,unsigned>> Dijkstra(string src, string dest);
    
};


