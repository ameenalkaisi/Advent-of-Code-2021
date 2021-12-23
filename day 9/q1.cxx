#include <vector>
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

    int total_risk = 0;
    for(int i = 0; i < depth_matrix.size(); ++i) {
        for(int j = 0; j < depth_matrix[i].size(); ++j) {
            if(is_low(depth_matrix, i, j)) {
                total_risk += 1 + depth_matrix[i][j];
                cout << i << " " << j << endl;
            }
        }
    }

    cout << total_risk << endl;
}