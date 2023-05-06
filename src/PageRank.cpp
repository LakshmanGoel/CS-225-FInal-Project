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

#include "PageRank.h"

using namespace std;

AdjMatrix::AdjMatrix(Airports airport, V2D countries, std::string country, unsigned num_airports) {
    matrix.resize(num_airports+1);
    for (unsigned i = 0; i < num_airports+1; ++i) {
        matrix.at(i).resize(num_airports+1);
        for (unsigned j = 0; j < num_airports+1; ++j) {
            matrix.at(i).at(j) = 0;
        }
    }
    for (std::vector<std::string> row : countries) {
        if (row.at(1) == country) {
            std::vector<std::string> destinations(num_airports);
            destinations = airport.neighbors(row.at(0));
            for (std::string destination : destinations) {
                matrix.at(std::stod(row.at(0))).at(std::stod(destination))++;
            }
        }
    }
}


PageRank::PageRank(int n, Airports airports): num_nodes(n), pagerank(n, 1.0/n), airports(airports) {}

int PageRank::calculate_rank() {
    std::vector<std::vector<std::string>> adjList(num_nodes);
    for (pair<Airports::vertex,list<Airports::edge*>> airport : airports.ports) {
        adjList.at(std::stoi(airport.first.port_name)) = airports.neighbors(airport.first.port_name);
    }
    vector<double> newPagerank(num_nodes, 0);
    for (int i = 0; i < max_iterations; i++) {
        for (int j = 0; j < num_nodes; j++) {
            newPagerank[j] = (1 - damping_factor) / num_nodes;
            for (unsigned k = 0; k < adjList[j].size(); k++) {
                int neighbor = std::stoi(adjList[j][k]);
                newPagerank[j] += damping_factor * pagerank[neighbor] / adjList[neighbor].size();
            }
        }
        if (converged(newPagerank)) {
            break;
        }
        pagerank = newPagerank;
    }
    double largest = pagerank[3];
    int largest_index = 3;
    for (int i = 0; i < num_nodes; i++) {
        if (pagerank[i] > largest) {
            largest = pagerank[i];
            largest_index = i;
        }
    }
    return largest_index;
}