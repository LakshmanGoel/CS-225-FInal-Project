// #include <iostream>
// #include <vector>
// #include <cmath>

// using namespace std;

// const int MAX_ITERATIONS = 100;
// const double DAMPING_FACTOR = 0.85;
// const double EPSILON = 1e-6;

// class PageRank {
// public:
//     PageRank(int n): N(n), adjList(n), pagerank(n, 1.0 / n) {}

//     void addEdge(int u, int v) {
//         adjList[u].push_back(v);
//     }

//     void calculate() {
//         vector<double> newPagerank(N, 0);
//         for (int i = 0; i < MAX_ITERATIONS; i++) {
//             for (int j = 0; j < N; j++) {
//                 newPagerank[j] = (1 - DAMPING_FACTOR) / N;
//                 for (unsigned k = 0; k < adjList[j].size(); k++) {
//                     int neighbor = adjList[j][k];
//                     newPagerank[j] += DAMPING_FACTOR * pagerank[neighbor] / adjList[neighbor].size();
//                 }
//             }
//             if (converged(newPagerank)) {
//                 break;
//             }
//             pagerank = newPagerank;
//         }
//     }

//     void print() {
//         for (int i = 0; i < N; i++) {
//             cout << "Node " << i << ": " << pagerank[i] << endl;
//         }
//     }

// private:
//     int N;
//     vector<vector<int>> adjList;
//     vector<double> pagerank;

//     bool converged(vector<double>& newPagerank) {
//         for (int i = 0; i < N; i++) {
//             if (abs(newPagerank[i] - pagerank[i]) > EPSILON) {
//                 return false;
//             }
//         }
//         return true;
//     }
// };

// int main() {
//     int n = 5;
//     PageRank pagerank(n);
//     pagerank.addEdge(0, 1);
//     pagerank.addEdge(0, 2);
//     pagerank.addEdge(1, 2);
//     pagerank.addEdge(2, 0);
//     pagerank.addEdge(2, 3);
//     pagerank.addEdge(3, 4);
//     pagerank.addEdge(4, 0);
//     pagerank.calculate();
//     pagerank.print();
//     return 0;
// }

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const double damping_factor = 0.85;
const double convergence_threshold = 0.0001;

void pageRank(vector<vector<int>> &adjacency_matrix, vector<double> &page_rank_scores) {
    int num_nodes = adjacency_matrix.size();
    vector<double> prev_scores(num_nodes, 1.0 / num_nodes);

    while (true) {
        double max_diff = 0.0;
        for (int i = 0; i < num_nodes; i++) {
            double sum = 0.0;
            for (int j = 0; j < num_nodes; j++) {
                if (adjacency_matrix[j][i] == 1) {
                    sum += prev_scores[j] / adjacency_matrix[j].size();
                }
            }
            double new_score = (1.0 - damping_factor) / num_nodes + damping_factor * sum;
            max_diff = max(max_diff, abs(new_score - prev_scores[i]));
            page_rank_scores[i] = new_score;
        }
        if (max_diff < convergence_threshold) {
            break;
        }
        prev_scores = page_rank_scores;
    }
}

int main() {
    // Example adjacency matrix
    vector<vector<int>> adj_matrix = {
        {0, 1, 1, 0},
        {0, 0, 1, 1},
        {1, 0, 0, 1},
        {0, 0, 0, 1}
    };
    
    int num_nodes = adj_matrix.size();
    vector<double> page_rank_scores(num_nodes, 0.0);
    
    pageRank(adj_matrix, page_rank_scores);
    
    for (int i = 0; i < num_nodes; i++) {
        cout << "Node " << i << ": " << page_rank_scores[i] << endl;
    }
    
    return 0;
}