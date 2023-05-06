/**
 * @file flights.cpp
 * flight paths using graph algorithms
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>

#include "csv_to_V2D.h"
#include "utils.h"
#include <algorithm>

using namespace std;
/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings where each row
 * in the text file is a row in the V2D and each comma-separated value is stripped of whitespace
 * and stored as its own string. 
 * 
 * Your V2D should match the exact structure of the input file -- so the first row, first column
 * in the original file should be the first row, first column of the V2D.
 *  
 * @param filename The filename of a CSV-formatted text file. 
 */
V2D file_to_V2D(const std::string & filename){
    // Your code here!
    std::ifstream file;
    file.open(filename);
    std::string list = file_to_string(filename);
    std::vector<std::vector<std::string>> data;
    char sep=',';
    std::vector<std::string> field;
    std::string line="";
    
    //string to V2D
    for(char s : list) {
        if(s!='\n') {
            line+=s;
        }
        else {
            split_string(line,sep,field);
            data.push_back(field);
            line="";
            field.clear();
        }
        
    }

    //catch last field
    split_string(line,sep,field);
    data.push_back(field);

    //whitespace removal
    // for(std::vector<std::string>& row : data) {
    //     for(std::string& element : row) {
    //         element=trim(element);
    //     }

    // }
    
    file.close();
    return data;

}

/**
 * Given a V2D with fields as corresponding to airports.dat. Make a corresponding V2D with fields:
 * Airport ID (unique ID by openflights), Latitude, Longitude.
 * 
 * @param cv A 2D vector 
 */

V2D V2D_to_airportsV2D(const V2D & airport_data) {
    V2D airport;
    

    for(vector<string> row_data : airport_data) {
        vector<string> airport_row;
        if (row_data.size() < 7 || row_data.size() > 14) {
            continue;
        }
        airport_row.push_back(row_data.at(0)); //airport id
        airport_row.push_back(row_data.at(6));//latitude
        airport_row.push_back(row_data.at(7));//longitude

        airport.push_back(airport_row);
    }

    return airport;
}

V2D V2D_to_country_airportsV2D(const V2D & airport_data, std::string country) {
    V2D airport;

    for(vector<string> row_data : airport_data) {
        vector<string> airport_row;
        if (row_data.size() < 7 || row_data.size() > 14) {
            continue;
        }
        if (row_data.at(3) == country) {
            airport_row.push_back(row_data.at(0)); //airport id
            airport_row.push_back(row_data.at(6));//latitude
            airport_row.push_back(row_data.at(7));//longitude

            airport.push_back(airport_row);
        }
    }

    return airport;
}

V2D V2D_to_routesV2D(const V2D & route_data) {
    V2D route;
    

    for(vector<string> row_data : route_data) {
        vector<string> route_row;
        if (row_data.size() < 7 || row_data.size() > 14) {
            continue;
        }
        if (row_data.at(3) == "\\N" || row_data.at(5) == "\\N" || row_data.at(3) == "\\N" || row_data.at(5) == "\\N") {
            continue;
        }
        //filter non direct routes
        if(row_data.at(7)!="0") {
            continue;
        }

        route_row.push_back(row_data.at(3)); //source airport id
        route_row.push_back(row_data.at(5)); //destination airport id


        route.push_back(route_row);
    }

    return route;
}

V2D V2D_to_countriesV2D(const V2D & airport_data) {
    V2D countries;
    

    for(vector<string> row_data : airport_data) {
        vector<string> airport_row;
        airport_row.push_back(row_data.at(0)); //airport id
        airport_row.push_back(row_data.at(3));//country

        countries.push_back(airport_row);
    }

    return countries;
}