#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <cmath>

using namespace std;

int natural_sum(int x) {
    return x*(x + 1)/2;
}

int get_cost(vector<int> positions, int comp_num) {
    return accumulate(positions.begin(), positions.end(), 0, [&](int x, int y) {
        return (x + natural_sum(abs(y - comp_num)));
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

    // sort and get middle point
    // get average element
    int elements_sum = accumulate(positions.begin(), positions.end(), 0);
    float float_pos = elements_sum/(float)positions.size();

    // get both upper and lower and see which one is lower cost then output that
    int lower = (int)floor(float_pos);
    int upper = (int)ceil(float_pos);

    int lower_cost = get_cost(positions, lower);
    int upper_cost = get_cost(positions, upper);

    cout << (upper_cost >= lower_cost ? lower_cost : upper_cost) << endl;
}