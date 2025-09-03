#include <bits/stdc++.h>
using namespace std;
int n, distance_matrix[100][100];
vector<int> solution;
set<pair<int,int>> tabu_list; 

void input(){
    cin >> n;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            cin >> distance_matrix[i][j];
        }
    }
}

void output(){
    cout << n;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    input();
    output();
    return 0;
}