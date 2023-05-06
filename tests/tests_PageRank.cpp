#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <limits>
#include "Airports.h"
#include "PageRank.h"
#include "csv_to_V2D.h"
#include "tests_util.h"

TEST_CASE("PageRank algorithm test 1") {
    const V2D correct_airport =   { \
    {"0","51.4706", "-0.461941"},\
    {"1","68.534401", "-89.808098"},\
    {"2","28.200899124145508", "83.98210144042969"},\
    {"3","53.552776", "10.006683"},};
    //{"4","53.552776", "10.006683"},};

    const V2D correct_route = { \
    {"0","1"},\
    {"1","2"},\
    {"1","3"},\
    {"3","1"},\
    {"2","3"},};

    Airports sample(correct_airport,correct_route,100); 

    PageRank page_rank(4, sample);
    
    REQUIRE(page_rank.calculate_rank() == 1);
}

TEST_CASE("PageRank algorithm test 2") {
    V2D airport_data = file_to_V2D("../data/airport_sample_data_2.csv");

    const V2D correct_route = { \
    {"0","1"},\
    {"1","2"},\
    {"1","3"},\
    {"3","1"},\
    {"2","3"},\
    {"4","0"},\
    {"4","1"}};

    V2D correct_airport = V2D_to_country_airportsV2D(airport_data, "\"India\"");

    REQUIRE(correct_airport.size() == 4);

    Airports sample(correct_airport,correct_route,100); 

    PageRank page_rank(4, sample);
    
    REQUIRE(page_rank.calculate_rank() == 1);
}