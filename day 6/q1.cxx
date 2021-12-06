#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <optional>

using namespace std;

constexpr int days = 80;

struct lantern_fish {
    int timer;

    lantern_fish(int timer) : timer {timer} {}

    optional<lantern_fish> simulate() {
        if(timer-- == 0) {
            timer = 6;
            return lantern_fish{8};
        } else return nullopt;
    }
};

int main(int argc, char **argv) {
    vector<lantern_fish> fishes{};

    // read input
    fstream input{argv[1]};
    if(!input)
        exit(1);

    int fish_timer;
    while(input >> fish_timer) {
        fishes.push_back(lantern_fish{fish_timer});

        if(input.peek() == ',')
            input.ignore();
    }

    // simulate input
    for(int day = 0; day < days; ++day) {
        // only go through the current size, newly pushed back fishes are after this size
        // simulate each currently considered fish
        int cur_size = fishes.size();
        for(int i = 0; i < cur_size; ++i) {
            // if there was a new fish, add it to fishes
            optional<lantern_fish> new_fish = fishes[i].simulate();
            if(new_fish)
                fishes.push_back(*new_fish);
        }
    }

//    for(auto i : fishes)
 //       cout << i.timer << " ";

    cout << fishes.size() << endl;    
}