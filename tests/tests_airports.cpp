#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <limits>
#include "Airports.h"
#include "csv_to_V2D.h"
#include "tests_util.h"

TEST_CASE("distance between two coordinates", "") {
    Coordinates A(51.4706, -0.461941); //London Heathrow
    Coordinates B(68.534401, -89.808098); //Kugaaruk

    unsigned distance = unsigned(A.dist(B));

    REQUIRE(distance == 4788);
    


}

TEST_CASE("Airports constructr", "") {
    const V2D correct_airport =   { \
    {"507","51.4706", "-0.461941"},\
    {"26","68.534401", "-89.808098"},\
    {"3127","28.200899124145508", "83.98210144042969"},\
    {"8810","53.552776", "10.006683"},};

    const V2D correct_route =   { \
    {"507","26"},\
    {"507","3127"},\
    {"26","3127"},\
    {"26","8810"},\
    {"8810","507"},\
    {"3127","8810"}};

    Airports sample(correct_airport,correct_route,15000);

    //check if distance is accurate
    REQUIRE(sample.dist("507","26")==4788);
    REQUIRE(sample.dist("507","3127")==7226);
    REQUIRE(sample.dist("26","3127")==9246);
    REQUIRE(sample.dist("26","8810")==4963);
    REQUIRE(sample.dist("8810","507")==744);
    REQUIRE(sample.dist("3127","8810")==6488);
    REQUIRE(sample.dist("507","27")==numeric_limits<unsigned>::max());


    //check access to points
    REQUIRE(sample.ports.at(507).first.location.lat == 51.4706);
    REQUIRE(sample.ports.at(507).first.location.lon == -0.461941);

    REQUIRE(sample.ports.at(26).first.location.lat == 68.534401);
    REQUIRE(sample.ports.at(26).first.location.lon == -89.808098);

    REQUIRE(sample.ports.at(3127).first.location.lat == 28.200899124145508);
    REQUIRE(sample.ports.at(3127).first.location.lon == 83.98210144042969);

    REQUIRE(sample.ports.at(8810).first.location.lat == 53.552776);
    REQUIRE(sample.ports.at(8810).first.location.lon == 10.006683);

    //adjency tests
    //507
    REQUIRE(sample.areAdj("507","26")==true);
    REQUIRE(sample.areAdj("507","3127")==true);
    REQUIRE(sample.areAdj("507","8810")==false);

    //26
    REQUIRE(sample.areAdj("26","507")==false);
    REQUIRE(sample.areAdj("26","3127")==true);
    REQUIRE(sample.areAdj("26","8810")==true);

    //8810
    REQUIRE(sample.areAdj("3127","507")==false);
    REQUIRE(sample.areAdj("3127","26")==false);
    REQUIRE(sample.areAdj("3127","8810")==true);

    //3127
    REQUIRE(sample.areAdj("8810","507")==true);
    REQUIRE(sample.areAdj("8810","26")==false);
    REQUIRE(sample.areAdj("8810","3127")==false);


    //edge list tests
    //507
    list<Airports::edge*> flights = sample.incidentEdges("507");
    vector<string> sources;
    vector<string> destinations;
    vector<unsigned> distances;

    vector<string> correct_sources = {"507","507"};
    vector<string> correct_destinations = {"26","3127"};
    vector<unsigned> correct_distances = {sample.dist("507","26"), sample.dist("507","3127")};

    for(Airports::edge* flight : flights) {
        sources.push_back(flight->source->port_name);
        destinations.push_back(flight->destination->port_name);
        distances.push_back(flight->dist);
    }

    REQUIRE(sources==correct_sources);
    REQUIRE(destinations==correct_destinations);
    REQUIRE(sample.neighbors("507")==correct_destinations);
    REQUIRE(distances==correct_distances);

    flights.clear();

    sources.clear();
    destinations.clear();
    distances.clear();

    correct_sources.clear();
    correct_destinations.clear();
    correct_distances.clear();

    //26
    flights = sample.incidentEdges("26");

    correct_sources = {"26","26"};
    correct_destinations = {"3127","8810"};
    correct_distances = {sample.dist("26","3127"), sample.dist("26","8810")};

    for(Airports::edge* flight : flights) {
        sources.push_back(flight->source->port_name);
        destinations.push_back(flight->destination->port_name);
        distances.push_back(flight->dist);
    }

    REQUIRE(sources==correct_sources);
    REQUIRE(destinations==correct_destinations);
    REQUIRE(sample.neighbors("26")==correct_destinations);
    REQUIRE(distances==correct_distances);

    flights.clear();

    sources.clear();
    destinations.clear();
    distances.clear();

    correct_sources.clear();
    correct_destinations.clear();
    correct_distances.clear();

    //3127
    flights = sample.incidentEdges("3127");

    correct_sources = {"3127"};
    correct_destinations = {"8810"};
    correct_distances = {sample.dist("3127","8810")};

    for(Airports::edge* flight : flights) {
        sources.push_back(flight->source->port_name);
        destinations.push_back(flight->destination->port_name);
        distances.push_back(flight->dist);
    }

    REQUIRE(sources==correct_sources);
    REQUIRE(destinations==correct_destinations);
    REQUIRE(sample.neighbors("3127")==correct_destinations);
    REQUIRE(distances==correct_distances);

    flights.clear();

    sources.clear();
    destinations.clear();
    distances.clear();

    correct_sources.clear();
    correct_destinations.clear();
    correct_distances.clear();

    //8810
    flights = sample.incidentEdges("8810");

    correct_sources = {"8810"};
    correct_destinations = {"507"};
    correct_distances = {sample.dist("8810","507")};

    for(Airports::edge* flight : flights) {
        sources.push_back(flight->source->port_name);
        destinations.push_back(flight->destination->port_name);
        distances.push_back(flight->dist);
    }

    REQUIRE(sources==correct_sources);
    REQUIRE(destinations==correct_destinations);
    REQUIRE(sample.neighbors("8810")==correct_destinations);
    REQUIRE(distances==correct_distances);

}

TEST_CASE("shortest path between two airports", "") {
    const V2D correct_airport =   { \
    {"507","51.4706", "-0.461941"},\
    {"26","68.534401", "-89.808098"},\
    {"3127","28.200899124145508", "83.98210144042969"},\
    {"8810","53.552776", "10.006683"},};

    const V2D correct_route =   { \
    {"507","26"},\
    {"507","3127"},\
    {"26","3127"},\
    {"26","8810"},\
    {"8810","507"},\
    {"3127","8810"}};

    Airports sample(correct_airport,correct_route,15000);
    vector<pair<string,unsigned>> shortest_path = sample.Dijkstra("507","8810");

    REQUIRE(shortest_path.at(0).first=="26");
    REQUIRE(shortest_path.at(0).second==4788);
    REQUIRE(shortest_path.at(1).first=="8810");
    REQUIRE(shortest_path.at(1).second==4963+4788);
}

