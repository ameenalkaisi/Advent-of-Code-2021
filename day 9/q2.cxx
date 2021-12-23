#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

bool is_low(vector<vector<int>> const &depth_matrix, int i, int j) {
    int top, right, bottom, left;
    top = right = bottom = left = 10; // max digit is 10

    // set the digits accordingly if they are present
    if(i - 1 >= 0)
        top = depth_matrix[i - 1][j];
    
    if(j + 1 < depth_matrix[0].size())
        right = depth_matrix[i][j + 1];

    if(i + 1 < depth_matrix.size())
        bottom = depth_matrix[i + 1][j];
    
    if(j - 1 >= 0)
        left = depth_matrix[i][j - 1];

    // if it an edge, will satsify the condition of that edge since x < max is always true
    // just ends up checking the other ones
    int cur = depth_matrix[i][j];
    return (cur < top && cur < right && cur < bottom && cur < left);
}

template <typename T>
bool contains(vector<T> vec, T element) {
    return (find(vec.begin(), vec.end(), element) != vec.end());
}

int get_size(vector<vector<int>> const &depth_matrix, int i, int j, vector<pair<int, int>> &visited) {
    // return 0 if:
    //  location is invalid
    //  already visited
    //  value is 9
    if(i >= depth_matrix.size() || j >= depth_matrix[0].size() || contains(visited, {i, j}) || depth_matrix[i][j] == 9)
        return 0;

    visited.push_back({i, j});

    return 1 
        + get_size(depth_matrix, i + 1, j, visited) 
        + get_size(depth_matrix, i - 1, j, visited)
        + get_size(depth_matrix, i, j + 1, visited)
        + get_size(depth_matrix, i, j - 1, visited);
}

int main(int argc, char **argv) {
    ifstream input{argv[1]};

    if(!input)
        exit(1);

    vector<vector<int>> depth_matrix;

    string line;
    for(int i = 0; getline(input, line); ++i) {
        depth_matrix.push_back(vector<int>{});

        // get one character at a time to guarantee
        // one digit at a time
        // then convert and add it to the matrix
        istringstream line_reader{line};
        for(char next_char; line_reader >> next_char;)
            depth_matrix[i].push_back(next_char - '0');
    }

    vector<int> basin_sizes{};
    for(int i = 0; i < depth_matrix.size(); ++i) {
        for(int j = 0; j < depth_matrix[i].size(); ++j) {
            if(is_low(depth_matrix, i, j)) {
                vector<pair<int, int>> temp_visited{};
                basin_sizes.push_back(get_size(depth_matrix, i, j, temp_visited));
            }
        }
    }

    sort(basin_sizes.begin(), basin_sizes.end());
    for(auto i : basin_sizes)
        cout << i << endl;

    long multiplied_sizes = 1;
    for(int i = 1; i <= 3; ++i) {
        if(basin_sizes.size() - i >= 0 && basin_sizes.size() != 0)
            multiplied_sizes *= basin_sizes[basin_sizes.size() - i];
    }

    cout << multiplied_sizes << endl;
}