#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <optional>
#include <map>

using namespace std;

constexpr int days = 256;

int main(int argc, char **argv) {
    // much more optimized version of q1
    map<int, unsigned long long> fishes{};

    // read input
    fstream input{argv[1]};
    if(!input)
        exit(1);

    int fish_timer;
    while(input >> fish_timer) {
        ++fishes[fish_timer];

        if(input.peek() == ',')
            input.ignore();
    }

    // key-value pair to represent fish to number of fish there are
    // number of fishes at a timer are counted at fishes[timer]
    for(int day = 0; day < days; ++day) {
        // calculations for fishes[0], fishes[8] have to be done later, so store fishes[0] for now
        // also fishes[6] will be added later
        unsigned long long old_fish1 = fishes[1];

        // bubble fishes to the right
        for(int i = 1; i < 8; ++i)
            fishes[i] = fishes[i + 1];

        // fishes[8] is now what fishes[0] is after this day
        // fishes[6] updated to include the parents
        // fishes[0] now inhertifs original size of fishes[1]
        fishes[8] = fishes[0];
        fishes[6] += fishes[0];
        fishes[0] = old_fish1;
    }

    // count all the fish and display
    unsigned long long fish_counter = 0;
    for(auto [fish, count] : fishes)
        fish_counter += count;

    cout << fish_counter << endl;    
}