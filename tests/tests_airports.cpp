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

TEST_CASE("shortest path between two airports when a smaller path in the wrong direction", "") {
    const V2D correct_airport =   { \
    {"1","51.4706", "-0.461941"},\
    {"2","68.534401", "-89.808098"},\
    {"3","28.200899124145508", "83.98210144042969"},\
    {"4","53.552776", "10.006683"},};

    const V2D correct_route =   { \
    {"1","2"},\
    {"1","3"},\
    {"2","3"},\
    {"2","4"},\
    {"4","1"},\
    {"3","4"}};

    Airports sample(correct_airport,correct_route,4);
    vector<pair<string,unsigned>> shortest_path = sample.Dijkstra("1","4");

    REQUIRE(shortest_path.at(0).first=="2");
    REQUIRE(shortest_path.at(0).second==4788);
    REQUIRE(shortest_path.at(1).first=="4");
    REQUIRE(shortest_path.at(1).second==4963+4788);
}

TEST_CASE("shortest path between two airports when we have a cycle", "") {
    const V2D correct_airport =   { \
    {"1","51.4706", "-0.461941"},\
    {"2","68.534401", "-89.808098"},\
    {"3","28.200899124145508", "83.98210144042969"},\
    {"4","53.552776", "10.006683"},
    {"5","41.8781","87.6298"}};

    const V2D correct_route =   { \
    {"1","2"},\
    {"2","4"},\
    {"4","5"},\
    {"5","2"},
    {"4","3"}};

    Airports sample(correct_airport,correct_route,5);
    vector<pair<string,unsigned>> shortest_path = sample.Dijkstra("1","3");

    REQUIRE(shortest_path.at(0).first=="2");
    REQUIRE(shortest_path.at(0).second==4788);
    REQUIRE(shortest_path.at(1).first=="4");
    REQUIRE(shortest_path.at(1).second==4788+4963);
    REQUIRE(shortest_path.at(2).first=="3");
    REQUIRE(shortest_path.at(2).second==4788+4963+6488);
}

TEST_CASE("shortest path between two airports when path with more edges is shorter", "") {
    const V2D correct_airport =   { \
    {"1","51.4706", "-0.461941"},\
    {"2","68.534401", "-89.808098"},\
    {"3","28.200899124145508", "83.98210144042969"},\
    {"5","41.8781","87.6298"}, 
    {"4","53.552776", "10.006683"},
    };

    const V2D correct_route =   { \
    {"1","2"},\
    {"2","3"},\
    {"1","4"},\
    {"4","5"},\
    {"5","3"}};

    Airports sample(correct_airport,correct_route,5);

    REQUIRE(sample.dist("1","2")==4788);
    REQUIRE(sample.dist("1","3")==7226);
    REQUIRE(sample.dist("2","3")==9246);
    REQUIRE(sample.dist("2","4")==4963);
    REQUIRE(sample.dist("4","1")==744);
    REQUIRE(sample.dist("3","4")==6488);
    REQUIRE(sample.dist("5","1")==6390);
    REQUIRE(sample.dist("5","2")==7735);
    REQUIRE(sample.dist("5","4")==5650);
    REQUIRE(sample.dist("5","3")==1556);

    vector<pair<string,unsigned>> shortest_path = sample.Dijkstra("1","3");

    REQUIRE(shortest_path.at(0).first=="4");
    REQUIRE(shortest_path.at(0).second==744);
    REQUIRE(shortest_path.at(1).first=="5");
    REQUIRE(shortest_path.at(1).second==744+5650);
    REQUIRE(shortest_path.at(2).first=="3");
    REQUIRE(shortest_path.at(2).second==744+5650+1556);
}

TEST_CASE("shortest path between siebel (uiuc) and cloud gate (from cs225 resources for dijkstra but implemented using coordinates instead of time)", "") {

    const V2D cities = {
        {"1","40.1020","88.2272"}, //siebel (uiuc)
        {"2","40.3084","88.1559"}, //rantoul
        {"3","39.1653","86.5264"}, //bloomington
        {"4","41.1200","87.8612"}, //kankakee 
        {"5","42.6389","83.2910"}, //pontiac 
        {"6","41.8781","87.6298"}, //chicago
        {"7","41.5250","88.0817"}, //joliet 
        {"8","41.8827","87.6233"} //cloud gate
    };

    const V2D correct_route =   { \
    {"1","2"},\
    {"1","3"},\
    {"2","4"},\
    {"3","5"},\
    {"4","5"},\
    {"4","6"},\
    {"5","4"},\
    {"5","6"},\
    {"5","7"},\
    {"7","6"},\
    {"6","8"}};

    Airports sample(cities,correct_route,8);


    REQUIRE(sample.dist("1","2")==23);
    REQUIRE(sample.dist("1","3")==179);
    REQUIRE(sample.dist("2","4")==93);
    REQUIRE(sample.dist("3","5")==472);
    REQUIRE(sample.dist("4","6")==86);
    REQUIRE(sample.dist("5","4")==414);
    REQUIRE(sample.dist("5","6")==366);
    REQUIRE(sample.dist("5","7")==414);
    REQUIRE(sample.dist("7","6")==54);
    REQUIRE(sample.dist("6","8")==0);
    vector<pair<string,unsigned>> shortest_path=sample.Dijkstra("1","8");

    REQUIRE(shortest_path.at(0).first=="2");
    REQUIRE(shortest_path.at(0).second==23);
    REQUIRE(shortest_path.at(1).first=="4");
    REQUIRE(shortest_path.at(1).second==23+93);
    REQUIRE(shortest_path.at(2).first=="6");
    REQUIRE(shortest_path.at(2).second==23+93+86);
    REQUIRE(shortest_path.at(3).first=="8");
    REQUIRE(shortest_path.at(3).second==23+93+86);

}


TEST_CASE ("bfs test 1", "") {
    const V2D correct_airport =   { \
    {"1","51.4706", "-0.461941"},\
    {"2","68.534401", "-89.808098"},\
    {"3","28.200899124145508", "83.98210144042969"},\
    {"4","53.552776", "10.006683"},};

    const V2D correct_route =   { \
    {"1","2"},\
    {"1","3"},\
    {"2","3"},\
    {"2","4"},\
    {"4","1"}};

    Airports sample(correct_airport,correct_route,4);
    vector<tuple<string, string, int>> span = sample.Spanning_bfs("1");

    REQUIRE(get<0>(span.at(0))=="1");
    REQUIRE(get<1>(span.at(0))=="NULL");
    REQUIRE(get<2>(span.at(0))==0);
    REQUIRE(get<0>(span.at(1))=="2");
    REQUIRE(get<1>(span.at(1))=="1");
    REQUIRE(get<2>(span.at(1))==1);
    REQUIRE(get<0>(span.at(2))=="3");
    REQUIRE(get<1>(span.at(2))=="1");
    REQUIRE(get<2>(span.at(2))==1);
    REQUIRE(get<0>(span.at(3))=="4");
    REQUIRE(get<1>(span.at(3))=="2");
    REQUIRE(get<2>(span.at(3))==2);
}

TEST_CASE("BFS with a cycle", "") {
    const V2D correct_airport =   { \
    {"1","51.4706", "-0.461941"},\
    {"2","68.534401", "-89.808098"},\
    {"3","28.200899124145508", "83.98210144042969"},\
    {"4","53.552776", "10.006683"},
    {"5","41.8781","87.6298"}};

    const V2D correct_route =   { \
    {"1","2"},\
    {"2","4"},\
    {"4","5"},\
    {"5","2"},
    {"4","3"}};

    Airports sample(correct_airport,correct_route,5);
    vector<tuple<string, string, int>> span = sample.Spanning_bfs("1");

    // for (auto && p : span) {
    //     cout << "node: " << std::get<0>(p) << " prev: " << std::get<1>(p) << "  depth: " << std::get<2>(p) << endl;
    // }

    REQUIRE(get<0>(span.at(0))=="1");
    REQUIRE(get<1>(span.at(0))=="NULL");
    REQUIRE(get<2>(span.at(0))==0);
    REQUIRE(get<0>(span.at(1))=="2");
    REQUIRE(get<1>(span.at(1))=="1");
    REQUIRE(get<2>(span.at(1))==1);
    REQUIRE(get<0>(span.at(2))=="3");
    REQUIRE(get<1>(span.at(2))=="4");
    REQUIRE(get<2>(span.at(2))==3);
    REQUIRE(get<0>(span.at(3))=="4");
    REQUIRE(get<1>(span.at(3))=="2");
    REQUIRE(get<2>(span.at(3))==2);
    REQUIRE(get<0>(span.at(4))=="5");
    REQUIRE(get<1>(span.at(4))=="4");
    REQUIRE(get<2>(span.at(4))==3);

   
}
