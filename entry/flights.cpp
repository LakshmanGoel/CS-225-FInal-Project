#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "utils.h"
#include "csv_to_V2D.h"
#include "Airports.h"
#include "PageRank.h"

using namespace std;

static void usage(std::string executable) {
    cout << "Usage: " << executable << " country_1 " << " country_2" << std::endl;
}

/**
Argument parsing
*/
int main(int argc, char* argv[]) {
    std::string airports_data = "";
    std::string routes_data = "";

    if (argc == 3) {
        V2D airport_data = file_to_V2D("../data/airports.csv");
        V2D routes_data = file_to_V2D("../data/routes.csv");
        V2D airports_unfiltered = V2D_to_airportsV2D(airport_data);
        V2D routes = V2D_to_routesV2D(routes_data);
        std::string country_1 = "";
        int count_1 =  0;
        while(argv[1][++count_1] != '\0');
        for (int i = 0; i < count_1; ++i) {
            if (argv[1][i] == '_') {
                country_1 += ' ';
            } else {
                country_1 += argv[1][i];
            }
        }
        std::string country_2 = "";
        int count_2 =  0;
        while(argv[2][++count_2] != '\0');
        for (int i = 0; i < count_2; ++i) {
            if (argv[2][i] == '_') {
                country_2 += ' ';
            } else {
                country_2 += argv[2][i];
            }
        }
        std::cout << country_1 << " " << country_2 << std::endl;
        V2D airports_filtered_1 = V2D_to_country_airportsV2D(airport_data, country_1);
        V2D airports_filtered_2 = V2D_to_country_airportsV2D(airport_data, country_2);
        Airports airports(airports_unfiltered, routes, 15000);
        Airports airports_1(airports_filtered_1, routes, 15000);
        Airports airports_2(airports_filtered_2, routes, 15000);
        PageRank page_rank_1(15000, airports_1);
        PageRank page_rank_2(15000, airports_2);
        int popular_1 = page_rank_1.calculate_rank();
        int popular_2 = page_rank_2.calculate_rank();
        std::cout << "The most popular airports in " << argv[1] << " & " << argv[2] << " " << "are: " << popular_1 << " " << popular_2 << std::endl;
        vector<pair<string, unsigned>> shortest_path = airports.Dijkstra(to_string(popular_1), to_string(popular_2));
        std::cout << "The shortest path between the most popular airports in " << argv[1] << " & " << argv[2] << " is: " << std::endl;
        std::string path = "";
        for (unsigned i = 0; i < shortest_path.size(); ++i) {
            path += shortest_path.at(i).first + " ";
        }
        std::cout << path << std::endl;
        vector<tuple<string, string, int>> span = airports.Spanning_bfs(to_string(popular_1));
        ofstream BFSFile("bfs.txt");
        for (auto && p : span) {
            BFSFile << "node: " << std::get<0>(p) << " prev: " << std::get<1>(p) << "  depth: " << std::get<2>(p) << endl;
        }
        BFSFile.close();

    } else {
        usage(argv[0]);
        return 1;
    }
}