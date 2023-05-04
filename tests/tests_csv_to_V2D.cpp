#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "csv_to_V2D.h"
#include "tests_util.h"

TEST_CASE("file_to_V2D Public Test 1", "[weight=5]") {
    V2D roster = file_to_V2D("../data/c5_s10_3_roster.csv");

    const V2D correct_roster = {{"CS577", "JydY", "MnWd", "tnkL"},\
     {"CS591", "gpDS", "94Ol", "tnkL"}, \
     {"CS386", "SjC0", "EYge", "Nvu1"}, \
     {"CS500", "MnWd", "uAcT", "EYge"}, \
     {"CS395", "gpDS", "EYge", "MnWd"}
     };

    REQUIRE(roster == correct_roster);

    V2D students = file_to_V2D("../data/c5_s10_3_students.csv");

    const V2D answer = {
        {"JydY", "CS577"}, \
        {"MnWd", "CS577", "CS500", "CS395"}, \
        {"tnkL", "CS577", "CS591"}, \
        {"gpDS", "CS591", "CS395"}, \
        {"94Ol", "CS591"}, \
        {"SjC0", "CS386"}, \
        {"EYge", "CS386", "CS500", "CS395"}, \
        {"Nvu1", "CS386"}, \
        {"uAcT", "CS500"}
        };
    REQUIRE(students == answer);
}

TEST_CASE("file_to_V2D Public Test 2", "[weight=5]") {
    V2D roster = file_to_V2D("../data/c10_s50_2_roster.csv");

    const V2D correct_roster = {
        {"CS502", "RAq7", "4GUp"}, \
        {"CS512", "nEFp", "n4mr"}, \
        {"CS161", "n4mr", "gLbj"}, \
        {"CS259", "rJvt", "RAq7"}, \
        {"CS411", "OZOR", "nHTn"}, \
        {"CS299", "3p9F", "xrmC"}, \
        {"CS190", "STSG", "3p9F"}, \
        {"CS180", "ZFvu", "Mkds"}, \
        {"CS583", "UkhY", "Xxka"}, \
        {"CS245", "RAq7", "t57x"}
        };

    REQUIRE(roster == correct_roster);

    V2D students = file_to_V2D("../data/c10_s50_2_students.csv");

    const V2D answer = {
        {"RAq7", "CS502", "CS259", "CS245"}, \
        {"4GUp", "CS502"}, \
        {"nEFp", "CS512"}, \
        {"n4mr", "CS512", "CS161"}, \
        {"gLbj", "CS161"}, \
        {"rJvt", "CS259"}, \
        {"OZOR", "CS411"}, \
        {"nHTn", "CS411"}, \
        {"3p9F", "CS299", "CS190"}, \
        {"xrmC", "CS299"}, \
        {"STSG", "CS190"}, \
        {"ZFvu", "CS180"}, \
        {"Mkds", "CS180"}, \
        {"UkhY", "CS583"}, \
        {"Xxka", "CS583"}, \
        {"t57x", "CS245"}
        };

    REQUIRE( students == answer);
}

TEST_CASE("file_to_V2D airport case", "") {

    V2D airport_data = file_to_V2D("../data/airport_sample_data.csv");

    const V2D correct_airport_data =   { \
    { "507", "\"London Heathrow Airport\"", "\"London\"", "\"United Kingdom\"","\"LHR\"", "\"EGLL\"", "51.4706", "-0.461941", "83", "0", "\"E\"", "\"Europe/London\"", "\"airport\"", "\"OurAirports\"" },\
    { "26", "\"Kugaaruk Airport\"","\"Pelly Bay\"", "\"Canada\"", "\"YBB\"", "\"CYBB\"", "68.534401", "-89.808098", "56", "-7", "\"A\"", "\"America/Edmonton\"", "\"airport\"", "\"OurAirports\"" },\
    {"3127", "\"Pokhara Airport\"", "\"Pokhara\"", "\"Nepal\"", "\"PKR\"", "\"VNPK\"", "28.200899124145508", "83.98210144042969", "2712", "5.75", "\"N\"", "\"Asia/Katmandu\"", "\"airport\"", "\"OurAirports\"" },\
    { "8810", "\"Hamburg Hbf\"","\"Hamburg\"", "\"Germany\"", "\"ZMB\"", "\\N", "53.552776", "10.006683", "30", "1","\"E\"", "\"Europe/Berlin\"", "\"station\"", "\"User\"" } };

    REQUIRE(airport_data == correct_airport_data);

    V2D airport = V2D_to_airportsV2D(airport_data);

    const V2D correct_airport =   { \
    {"507","51.4706", "-0.461941"},\
    {"26","68.534401", "-89.808098"},\
    {"3127","28.200899124145508", "83.98210144042969"},\
    {"8810","53.552776", "10.006683"},};

    REQUIRE(airport == correct_airport);
}

TEST_CASE("file_to_V2D route case", "") {

    V2D route_data = file_to_V2D("../data/route_sample_data.csv");

    const V2D correct_route_data =   { \
    {"BA","1355","SIN","3316","LHR","507","","0","744 777" },\
    {"BA","1355","SIN","3316","MEL","3339","Y","0","744" },\
    {"TOM","5013","ACE","1055","BFS","465","","0","320"}, };


    REQUIRE(route_data == correct_route_data);

    V2D route = V2D_to_routesV2D(route_data);

    const V2D correct_route =   { \
    {"3316","507"},\
    {"3316","3339"},\
    {"1055","465"},};

    REQUIRE(route == correct_route);
}






