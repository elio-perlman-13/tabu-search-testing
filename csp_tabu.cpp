#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>

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
int distance_matrix[1000][1000];
vi solution;
set<pii> tabu_list; 

const int TABU_TENURE = 15;
const int INF = 1e9;
const int MAX_ITERATIONS = 5000;
const int NEIGHBOR_SAMPLE_SIZE = 500;

void input(){
    cin >> n;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            cin >> distance_matrix[i][j];
        }
    }
}

// difference in cost after swapping cities at positions i and j
int incremental_cost(const vi& sol, int i, int j, int current_cost) {
    int n = sol.size();
    int delta = 0;

    int a = sol[i];
    int b = sol[j];
    int a_next = sol[(i + 1) % n];
    int b_next = sol[(j + 1) % n];

    // Remove old edges
    delta -= distance_matrix[a][a_next];
    delta -= distance_matrix[b][b_next];

    // Add new edges
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

int tabu_search(const int max_iterations) {
    solution = generate_greedy_solution();
    int best_cost = calculate_cost(solution);
    vi best_solution = solution;

    int iterations = max_iterations;
    while (iterations--) {
        int current_cost = calculate_cost(solution);
        int best_neighbor_cost = INT_MAX;
        vi best_neighbor;
        int n = solution.size();

        for (int k = 0; k < NEIGHBOR_SAMPLE_SIZE; ++k) {
            int i = rand() % n;
            int j = rand() % n;
            if (i == j) continue;
            vi neighbor = solution;
            swap(neighbor[i], neighbor[j]);
            pii move = {i, j};
            bool is_tabu = tabu_list.find(move) != tabu_list.end();
            int cost = incremental_cost(solution, i, j, current_cost);

            if (!is_tabu || (is_tabu && cost < best_cost)) {
                if (cost < best_neighbor_cost) {
                    best_neighbor_cost = cost;
                    best_neighbor = neighbor;
                }
            }
        }

        if (best_neighbor_cost < best_cost) {
            best_cost = best_neighbor_cost;
            best_solution = best_neighbor;
        }

        if (!best_neighbor.empty()) {
            pii move = {solution[0], solution[1]};
            update_tabu_list(move);
            solution = best_neighbor;
        }
    }

    solution = best_solution;
    return best_cost;
}


void output(){
    input();
    int global_best_cost = INF;
    vi global_best_solution;
    int NUM_RESTARTS = 10;

    for (int restart = 0; restart < NUM_RESTARTS; ++restart) {
        solution = generate_greedy_solution(); // or generate_greedy_solution()
        int cost = tabu_search(MAX_ITERATIONS);
        if (cost < global_best_cost) {
            global_best_cost = cost;
            global_best_solution = solution;
        }
    }

    solution = global_best_solution;
    cout << "Best Cost: " << global_best_cost << endl;
    cout << "Best Solution: ";
    for (int city : solution) cout << city << " ";
    cout << endl;
}

int main() {
    freopen("input_a280.txt", "r", stdin);
    freopen("output_tabo_tsp.txt", "w", stdout);
    input();
     auto start = std::chrono::high_resolution_clock::now();
    output();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed Time: " << elapsed.count() << " seconds\n";
    return 0;
}