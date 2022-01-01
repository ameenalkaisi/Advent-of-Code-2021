#include <iostream>
#include <fstream>
#include <functional>
#include <climits>

using namespace std;

int constexpr max_x_attempt = 2000;
int constexpr max_y_attempt = 1000;
int constexpr min_x_attempt = -1000;
int constexpr min_y_attempt = -1000;

bool is_within(int val, int min_val, int max_val) {
    return (min_val <= val && val <= max_val);
}

bool do_initials_hit(int x_vel, int y_vel, int min_x, int max_x, int min_y, int max_y) {
    int cur_x, cur_y;
    cur_x = cur_y = 0;

    // can be optimized probably through math (heavily)
    while(x_vel != 0) {
        if(is_within(cur_x, min_x, max_x) && is_within(cur_y, min_y, max_y))
            return true;
        
        cur_x += x_vel;
        cur_y += y_vel;

        // simulate one step of the velocities
        x_vel += (x_vel > 0 ? -1 : 1);
        --y_vel;
    }

    // will eventually hit min_y or max_y
    // at this point projectile only going down

    if(!is_within(cur_x, min_x, max_x))
        return false;

    while(cur_y >= min_y) {
        if(is_within(cur_y, min_y, max_y))
            return true;
        
        cur_y += y_vel--;
    }

    return false;
}

int main(int argc, char **argv) {
    ifstream input{argv[1]};
    if(!input)
        exit(1);

    int target_x_min, target_x_max, target_y_min, target_y_max;

    // read input
    input.ignore(15); // 'target area: x=' ignored
    input >> target_x_min;

    input.ignore(2); // dots ignored
    input >> target_x_max;

    input.ignore(4); // ', ' ignored
    input >> target_y_min;

    input.ignore(2); // dots ignored
    input >> target_y_max;

    if(target_y_min > target_y_max)
        swap(target_y_min, target_y_max);

    if(target_x_min > target_x_max)
        swap(target_x_min, target_x_max);

    input.close();

    auto x_y_hits = bind(do_initials_hit, placeholders::_1, placeholders::_2, 
        target_x_min, target_x_max, target_y_min, target_y_max);

    int sum = 0;
    for(int i = min_x_attempt; i <= max_x_attempt; ++i) {
        for(int j = min_y_attempt; j <= max_y_attempt; ++j) {
            if(x_y_hits(i, j))
                ++sum;
        }
    }

    cout << sum << endl;
}