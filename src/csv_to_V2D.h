/**
 * @file schedule.h
 * Exam scheduling using graph coloring
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <set>

typedef std::vector<std::vector<std::string> > V2D; 


/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings where each row
 * in the text file is a row in the V2D and each comma-separated value is stripped of whitespace
 * and stored as its own string. You must preserve the order from the input file.
 *  
 * @param filename The filename of a CSV-formatted text file. 
 * @return The vector of vectors of strings imported from filename
 */
V2D file_to_V2D(const std::string & filename);

/**
 * Given a V2D with fields as corresponding to airports.dat. Make a corresponding V2D with fields:
 * Airport ID (unique ID by openflights), Latitude, Longitude.
 * 
 * @param cv A 2D vector
 * @return A 2D vector of airport data 
 */

V2D V2D_to_airportsV2D(const V2D & airport_data);

/**
 * Given a V2D with fields as corresponding to routes.dat. Make a corresponding V2D with fields:
 * source airport ID , destination airport ID
 * 
 * @param cv A 2D vector
 * @return A 2D vector of route data 
 */

V2D V2D_to_routesV2D(const V2D & airport_data);

V2D V2D_to_countriesV2D(const V2D & airport_data);

V2D V2D_to_country_airportsV2D(const V2D & airport_data, std::string country);

