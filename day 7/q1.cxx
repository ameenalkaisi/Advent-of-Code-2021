#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <cmath>

using namespace std;

int get_cost(vector<int> positions, int comp_num) {
    return accumulate(positions.begin(), positions.end(), 0, [&](int x, int y) {
        return (x + abs(y - comp_num));
    });
}

int main(int argc, char **argv) {
    vector<int> positions{};

    // read input
    fstream input{argv[1]};
    if(!input)
        exit(1);

    for(int next_int; input >> next_int;) {
        if(input.peek() == ',')
            input.ignore();
        
        positions.push_back(next_int);
    }

    // sort the list
    sort(positions.begin(), positions.end());

    // output middle point or the greater of the two middle points if list is not even
    if(positions.size() % 2 == 0)
        cout << get_cost(positions, positions.size()/2) << endl;
    else {
        int lower_index = (int)floor(positions.size()/2.0);
        int upper_index = (int)ceil(positions.size()/2.0);

        int lower = positions[lower_index];
        int upper = positions[upper_index];

        int lower_cost = get_cost(positions, lower);
        int upper_cost = get_cost(positions, upper);
        
        cout << (upper_cost >= lower_cost ? lower_cost : upper_cost) << endl;
    }
}