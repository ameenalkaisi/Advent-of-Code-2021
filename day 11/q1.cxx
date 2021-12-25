#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

template <typename T>
void print_matrix(vector<vector<T>> m) {
    for(auto const &i : m) {
        for(auto const &j : i)
            cout << j << " ";
        cout << endl;
    }
}

template <typename T>
bool contains(vector<T> vec, T elem) {
    return find(vec.begin(), vec.end(), elem) != vec.end();
}

bool append_check_flashable(vector<vector<int>> &energy_levels, int i, int j) {
    if(i < 0 || i >= energy_levels.size() || j < 0 || j >= energy_levels[0].size())
        return false;
    
    return ++energy_levels[i][j] > 9;
}

long flash(vector<vector<int>> &energy_levels, int i, int j, vector<pair<int, int>> &visited) {
    if(contains(visited, {i, j}))
        return 0;
    
    energy_levels[i][j] = 0;

    long total = 1;
    visited.push_back({i, j});

    for(int k = -1; k < 2; ++k) {
        for(int h = -1; h < 2; ++h) {
            if(!contains(visited, {i + k, j + h}) && append_check_flashable(energy_levels, i + k, j + h))
                total += flash(energy_levels, i + k, j + h, visited);
        }
    }

    return total;
}

long simulate_step(vector<vector<int>> &energy_levels) {
    vector<pair<int, int>> visited{};

    long total = 0;
    for(int i = 0; i < energy_levels.size(); ++i) {
        for(int j = 0; j < energy_levels[i].size(); ++j) {
            if(contains(visited, {i, j}))
                continue;

            ++energy_levels[i][j];

            if(energy_levels[i][j] > 9) {
                total += flash(energy_levels, i, j, visited);
            }
        }
    }

    return total;
}

int main(int argc, char **argv) {
    ifstream input{argv[1]};

    if(!input)
        exit(1);

    vector<vector<int>> octopus_level{};

    // initialize the octopus level matrix
    string line;
    for(int i = 0; getline(input, line); ++i) {
        istringstream iss{line};

        octopus_level.push_back(vector<int>{});

        char next_digit;
        while(iss >> next_digit)
            octopus_level[i].push_back(next_digit - '0');
    }

    long total = 0;
    for(int i = 0; i < 100; ++i) {
        cout << "--" << endl;
        total += simulate_step(octopus_level);
        print_matrix(octopus_level);
        cout << "--" << endl;
    }

    cout << total << endl;
}