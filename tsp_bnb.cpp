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
int best_cost = INT_MAX;
vi best_solution;

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

void tsp_bnb(int pos, int curr_cost, vector<bool>& visited, vi& curr_solution) {
    if (pos == n) {
        curr_cost += distance_matrix[curr_solution[n - 1]][curr_solution[0]];
        if (curr_cost < best_cost) {
            best_cost = curr_cost;  
            best_solution = curr_solution;
        }
        return;
    }

    for (int city = 0; city < n; city++) {
        if (!visited[city]) {
            visited[city] = true;
            curr_solution[pos] = city;
            int new_cost = curr_cost;
            if (pos > 0) {
                new_cost += distance_matrix[curr_solution[pos - 1]][city];
            }
            if (new_cost < best_cost) {
                tsp_bnb(pos + 1, new_cost, visited, curr_solution);
            }
            visited[city] = false;
        }
    }
}

void output(){
    vector<bool> visited(n, false);
    vi curr_solution(n);
    visited[0] = true;
    curr_solution[0] = 0; 
    tsp_bnb(1, 0, visited, curr_solution);
    cout << "Best Cost: " << best_cost << "\n";
    cout << "Best Solution: ";
    for (int city : best_solution) {
        cout << city << " ";
    }
    cout << "\n";
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output_bnb.txt", "w", stdout);
    input();
    auto start = chrono::high_resolution_clock::now();
    output();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Elapsed Time: " << elapsed.count() << " seconds\n";
    return 0;
}