/**
 * @file PageRank.h
 * PageRank class header file
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

#include "Airports.h"

using namespace std;
typedef std::vector<std::vector<std::string> > V2D; 

class PageRank {
public:
    PageRank(int n, Airports airports);
    /**
    * @brief Finds the most popular airport
    * 
    * @param start_name name of airport to start from
    * 
    * @return The algorithm returns an int which is basically the unique ID of the most popular airport of the specified country.
    */
    int calculate_rank();

private:
    int num_nodes;
    vector<double> pagerank;
    Airports airports;
    int max_iterations = 100;
    double damping_factor = 0.85;
    double epsilon = 0.001;

    bool converged(vector<double>& newPagerank) {
        for (int i = 0; i < num_nodes; i++) {
            if (abs(newPagerank[i] - pagerank[i]) > epsilon) {
                return false;
            }
        }
        return true;
    }
};
