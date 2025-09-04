#include <bits/stdc++.h>
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

int n, distance_matrix[100][100];
vi solution;
set<pii> tabu_list; 

const int TABU_TENURE = 10;
const int INF = 1e9;
const int MAX_ITERATIONS = 1000;

void input(){
    cin >> n;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            cin >> distance_matrix[i][j];
        }
    }
}

int calculate_cost(const vi& sol) {
    int cost = 0;
    FOR(i, 0, n - 1) {
        cost += distance_matrix[sol[i]][sol[i + 1]];
    }
    cost += distance_matrix[sol[n - 1]][sol[0]];
    return cost;
}

vi generate_initial_solution() {
    vi sol(n);
    iota(sol.begin(), sol.end(), 0);
    random_shuffle(sol.begin(), sol.end());
    return sol;
}

vvi get_neighbors(const vi& sol) {
    vvi neighbors;
    FOR(i, 0, n) {
        FOR(j, i + 1, n) {
            if (tabu_list.find({sol[i], sol[j]}) == tabu_list.end()) {
                vi neighbor = sol;
                swap(neighbor[i], neighbor[j]);
                neighbors.pb(neighbor);
            }
        }
    }
    return neighbors;
}

void update_tabu_list(const pii& move) {
    tabu_list.insert(move);
    if (tabu_list.size() > TABU_TENURE) {
        tabu_list.erase(tabu_list.begin());
    }
}

int tabu_search(const int max_iterations) {
    solution = generate_initial_solution();
    int best_cost = calculate_cost(solution);
    vi best_solution = solution;

    int iterations = max_iterations;
    while (iterations--) {
        vvi neighbors = get_neighbors(solution);
        int best_neighbor_cost = INT_MAX;
        vi best_neighbor;

        for (const auto& neighbor : neighbors) {
            int cost = calculate_cost(neighbor);
            if (cost < best_neighbor_cost) {
                best_neighbor_cost = cost;
                best_neighbor = neighbor;
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
    int best_cost = tabu_search(MAX_ITERATIONS);
    cout << "Best Cost: " << best_cost << "\n";
    cout << "Best Solution: ";
    for (int city : solution) {
        cout << city << " ";
    }
    cout << "\n";
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    input();
     auto start = std::chrono::high_resolution_clock::now();
    output();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed Time: " << elapsed.count() << " seconds\n";
    return 0;
}