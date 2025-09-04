#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
#include <filesystem>

#define ll long long
#define pb push_back
#define mp make_pair
#define pii pair<int,int>
#define vi vector<int>
#define vvi vector<vector<int>>
#define vpi vector<pair<int,int>>
#define all(v) v.begin(),v.end()
#define FOR(i,a,b) for(int i=a;i<b;i++)
#define RFOR(i,a,b) for(int i=a-1;i>=b;i--)

using namespace std;

int n;
int distance_matrix[2000][2000];
vi solution;
set<pii> tabu_list;

const int TABU_TENURE = 10;
const int INF = 1e9;
const int MAX_ITERATIONS = 1000;
const int NEIGHBOR_SAMPLE_SIZE = 200;

void input(){
    cin >> n;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            cin >> distance_matrix[i][j];
        }
    }
}

// difference in cost after flipping tour at positions i and j
int incremental_cost(const vi& sol, int i, int j, int current_cost) {
    int n = sol.size();
    int delta = 0;

    int a = sol[i];
    int b = sol[j];
    int a_next = sol[(i + 1) % n];
    int b_next = sol[(j + 1) % n];

    delta -= distance_matrix[a][a_next];
    delta -= distance_matrix[b][b_next];

    delta += distance_matrix[a][b];
    delta += distance_matrix[a_next][b_next];

    return current_cost + delta;
}

// calculate total cost of a solution
int calculate_cost(const vi& sol) {
    int cost = 0;
    FOR(i, 0, n - 1) {
        cost += distance_matrix[sol[i]][sol[i + 1]];
    }
    cost += distance_matrix[sol[n - 1]][sol[0]];
    return cost;
}

// generate a random initial solution
vi generate_greedy_solution() {
    vi sol(n, -1);
    vector<bool> visited(n, false);
    sol[0] = rand() % n;
    visited[sol[0]] = true;
    for (int i = 1; i < n; ++i) {
        int last = sol[i-1], next = -1, min_dist = INF;
        for (int j = 0; j < n; ++j) {
            if (!visited[j] && distance_matrix[last][j] < min_dist) {
                min_dist = distance_matrix[last][j];
                next = j;
            }
        }
        if (next == -1) break;
        sol[i] = next;
        visited[next] = true;
    }
    return sol;
}


void update_tabu_list(const pii& move) {
    tabu_list.insert(move);
    if (tabu_list.size() > TABU_TENURE) {
        tabu_list.erase(tabu_list.begin());
    }
}

pair<int, vi> tabu_search(const int max_iterations) {
    solution = generate_greedy_solution();
    int best_cost = calculate_cost(solution);
    vi best_solution = solution;

    int iterations = max_iterations;
    while (iterations--) {
        int current_cost = calculate_cost(solution);
        int best_neighbor_cost = INT_MAX;
        vi best_neighbor;
        int n = solution.size();
        int i, j, best_i = -1, best_j = -1;

        for (int k = 0; k < NEIGHBOR_SAMPLE_SIZE; ++k) {
            i = rand() % n;
            j = rand() % n;
            j = (j + i + 1) % n;
            vi neighbor = solution;
            if (i > j) swap(i, j);
            reverse(neighbor.begin() + i, neighbor.begin() + j + 1);

            pii move = {i, j};
            bool is_tabu = tabu_list.find(move) != tabu_list.end();
            int cost = incremental_cost(solution, i, j, current_cost);

            if (!is_tabu || (is_tabu && cost < best_cost)) {
                if (cost < best_neighbor_cost) {
                    best_neighbor_cost = cost;
                    best_neighbor = neighbor;
                    best_i = i;
                    best_j = j;
                }
            }
        }

        if (best_neighbor_cost < best_cost) {
            best_cost = best_neighbor_cost;
            best_solution = best_neighbor;
        }

        if (!best_neighbor.empty()) {
            pii move = {best_i, best_j};
            update_tabu_list(move);
            solution = best_neighbor;
        }
    }

    solution = best_solution;
    return {best_cost, best_solution};
}


void output(){
    input();
    int global_best_cost = INF;
    vi global_best_solution;
    int NUM_RESTARTS = 10;

    for (int restart = 0; restart < NUM_RESTARTS; ++restart) {
        auto [cost, best_sol] = tabu_search(MAX_ITERATIONS);
        if (cost < global_best_cost) {
            global_best_cost = cost;
            global_best_solution = best_sol;
        }
    }

    solution = global_best_solution;
    cout << "Best Cost: " << global_best_cost << endl;
    cout << "Best Solution: ";
    for (int city : solution) cout << city << " ";
    cout << endl;
}

int main() {
    namespace fs = filesystem;
    ofstream result("tabu_results.txt");
    string input_folder = "txt_folder";

    for (const auto& entry : fs::directory_iterator(input_folder)) {
        if (entry.path().extension() == ".txt") {
            freopen(entry.path().c_str(), "r", stdin);
            input();
            cout << "Processing " << entry.path().filename() << endl;
            auto start = std::chrono::high_resolution_clock::now();
            long long global_best_cost = INF;
            vi global_best_solution;
            int NUM_RESTARTS = 10;

            for (int restart = 0; restart < NUM_RESTARTS; ++restart) {
                auto [cost, best_sol] = tabu_search(MAX_ITERATIONS);
                if (cost < global_best_cost) {
                    global_best_cost = cost;
                    global_best_solution = best_sol;
                }
            }
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = end - start;

            result << entry.path().filename() << " Best Cost: " << global_best_cost << "\n";
            result << "Best Solution: ";
            for (long long city : global_best_solution) result << city << " ";
            result << "\nElapsed Time: " << elapsed.count() << " seconds\n\n";
        }
    }
    result.close();
    return 0;
}