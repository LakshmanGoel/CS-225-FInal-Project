/**
 * @file flights.cpp
 * flight paths using graph algorithms
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>

#include "Airports.h"
#include "csv_to_V2D.h"
#include "utils.h"
#include <algorithm>
#include <list>
#include <limits>
#include <queue>

using namespace std;

Coordinates::Coordinates() {
        lat=0;
        lon=0;
}

Coordinates::Coordinates(double A, double B) {
        lat=A;
        lon=B;
}

Airports::vertex::vertex() {
    valid=false;
    location = Coordinates();
    port_name = "0";
}

Airports::vertex::vertex(Coordinates loc, string name) {
    location=loc;
    valid=true;
    port_name = name;
}

Airports::priority_vertex::priority_vertex(string name, unsigned distance) {
    name_port=name;
    dist=distance;
}

bool Airports::priority_vertex::operator<(const priority_vertex other) const {
    if(dist>other.dist) {
        return true;
    }

    return false;
}

Airports::edge::edge() {
    source = NULL;
    destination = NULL;
    dist = 0;
}

Airports::edge::edge(vertex* src, vertex* dest, unsigned distance) {
    source=src;
    destination=dest;
    dist=distance;

}

//Haversine approximation 
unsigned Coordinates::dist(Coordinates other) {
    double degs_to_rads = 3.14159265359/180;
return 2*6371*asin(sqrt(sin((other.lat-lat)/2*degs_to_rads)*sin((other.lat-lat)/2*degs_to_rads)\
                        +cos(other.lat*degs_to_rads)*cos(lat*degs_to_rads)\
                        *sin((other.lon-lon)/2*degs_to_rads)*sin((other.lon-lon)/2*degs_to_rads)));
}

// airport constructor
Airports::Airports(V2D airports, V2D routes, unsigned num_of_airports) {

    ports.resize(num_of_airports+1);
    for(std::vector<std::string> row_data : airports) {
        //cout << row_data.at(1) << " Airport ctr" << std::endl;
        Coordinates location(stod(row_data.at(1),NULL),stod(row_data.at(2),NULL));
        string name=row_data.at(0);
        vertex port(location,name);
        ports.at(stod(row_data.at(0))).first=port;
    }

    for(vector<string> row_data : routes) {
        edge route(&ports.at(stod(row_data.at(0))).first,\
            &ports.at(stod(row_data.at(1))).first,\
            dist(row_data.at(0),row_data.at(1)));
        
        flights.push_back(route);
        ports.at(stod(row_data.at(0))).second.push_back(&flights.back());
    }
}

// airport constructor with distance for testing
Airports::Airports(V2D airports, V2D routes, unsigned num_of_airports, bool distance) {
    if(!distance) {
        return;
    }

    ports.resize(num_of_airports+1);
    for(std::vector<std::string> row_data : airports) {
        Coordinates location(stod(row_data.at(1),NULL),stod(row_data.at(2),NULL));
        string name=row_data.at(0);
        vertex port(location,name);
        ports.at(stod(row_data.at(0))).first=port;
    }

    for(vector<string> row_data : routes) {
        edge route(&ports.at(stod(row_data.at(0))).first,\
            &ports.at(stod(row_data.at(1))).first,\
            stod(row_data.at(2)));
        
        flights.push_back(route);
        ports.at(stod(row_data.at(0))).second.push_back(&flights.back());
    }
}

// function to find distance between two airports
unsigned Airports::dist(string A, string B) {
    if(ports.at(stod(A)).first.valid && ports.at(stod(B)).first.valid) {
        return ports.at(stod(A)).first.location.dist(ports.at(stod(B)).first.location);
    }

    return numeric_limits<unsigned>::max();
        
}

bool Airports::areAdj(string src, string dest) {
    for(edge* flightPtr : ports.at(stod(src)).second) {
        string flight_dest = flightPtr->destination->port_name;
        if(dest==flight_dest) {
            return true;
        }
    }

    return false;
}

list<Airports::edge*> Airports::incidentEdges(string port) {
    return ports.at(stod(port)).second;

};

vector<string> Airports::neighbors(string port) {
    list<edge*> flights = incidentEdges(port);
    vector<string> bors;
    for(edge* flight : flights) {
        bors.push_back(flight->destination->port_name);
    }

    return bors;
}

vector<pair<string,unsigned>> Airports::Dijkstra(string src, string dest) {
    //initialize dist
    vector<unsigned> dist;
    dist.resize(ports.size());

    //initialize prev
    vector<string> prev;
    prev.resize(ports.size());

    for(pair<vertex,list<edge*>> port : ports) {
        dist.at(stod(port.first.port_name)) = numeric_limits<unsigned>::max();
        prev.at(stod(port.first.port_name)) = "-1";
    }

    dist.at(stod(src)) = 0;
    vector<bool> visited;
    visited.resize(ports.size());
    visited.at(stod(src))=true;

    //initialize priority queue
    priority_queue<priority_vertex> queue;
    for(pair<vertex,list<edge*>> port : ports) {
        priority_vertex v(port.first.port_name,dist.at(stod(port.first.port_name)));
        queue.push(v);
    }

    while(queue.top().name_port!=dest) {
        priority_vertex curr_node = queue.top();
        queue.pop();
        for(string bor : neighbors(curr_node.name_port)) {
            if(visited.at(stod(bor))) {
                continue;
            }

            if(this->dist(curr_node.name_port,bor) + dist.at(stod(curr_node.name_port)) < dist.at(stod(bor))) {
                dist.at(stod(bor))=this->dist(curr_node.name_port,bor) + dist.at(stod(curr_node.name_port));
                priority_vertex v(bor,dist.at(stod(bor)));
                queue.push(v);
                prev.at(stod(bor))= curr_node.name_port;
            }
        }
        visited.at(stod(curr_node.name_port))=true;
    }

    vector<pair<string,unsigned>> shortest_path;
    string path=dest;
    while(path!=src) {
       shortest_path.push_back(pair<string,unsigned>(path,dist.at(stod(path))));
        path=prev.at(stod(path));
    }

    reverse(shortest_path.begin(),shortest_path.end());
        
    return shortest_path;
}

vector<tuple<string, string, int>> Airports::Spanning_bfs (string start_name) {

    vector<bool> visited (ports.size(), false);
    queue<string> to_visit;
    vector<pair<string, int>> help_tree (ports.size(), make_pair("", -1));

    //initialize the queue and spanTree with start node
    to_visit.push(start_name);
    help_tree.at(stod(start_name)).first = "NULL"; // prev of start is "NULL"
    help_tree.at(stod(start_name)).second = 0; // depth of start is 0

    while (!to_visit.empty()) {
        const string & curr_node = to_visit.front();
        to_visit.pop();

        visited.at(stod(curr_node)) = true;

        /* update depth of all unvisited neigbours to be depth+1 */
        int depth = help_tree.at(stod(curr_node)).second;
        for (const string & neigbour : neighbors(curr_node)) {
            pair<string, int> &span_info = help_tree.at(stod(neigbour));

            if (span_info.second == -1) {
                span_info.second = depth + 1;
                span_info.first = curr_node;
            }
            
            /* add unvisited neighbours to be visited */
            if (!visited.at(stod(neigbour))) {
                to_visit.push(neigbour);
            }
        }
    }

    //copy all reachable nodes (depth != -1) to return value
    vector<tuple<string, string, int>> spanTree;
    for (unsigned int i = 0; i < help_tree.size(); i++) {
        pair<string, int> elem = help_tree[i];
        if(elem.second != -1){
            auto tup = make_tuple(to_string(i), elem.first, elem.second);
            spanTree.push_back(tup);
        }
    }
    
    return spanTree;
}