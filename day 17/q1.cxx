#include <iostream>
#include <fstream>
#include <functional>
#include <climits>

using namespace std;

int constexpr max_x_attempt = 100;
int constexpr max_y_attempt = 100;
int constexpr min_x_attempt = -100;
int constexpr min_y_attempt = -100;

bool is_within(int val, int min_val, int max_val) {
    return (min_val <= val && val <= max_val);
}

int do_initials_hit_maxy(int x_vel, int y_vel, int min_x, int max_x, int min_y, int max_y) {
    int cur_x, cur_y;
    cur_x = cur_y = 0;

    int highest = INT_MIN;

    // can be optimized probably through math (heavily)
    while(x_vel != 0) {

        if(cur_y > highest)
            highest = cur_y;

        if(is_within(cur_x, min_x, max_x) && is_within(cur_y, min_y, max_y))
            return highest;
        
        cur_x += x_vel;
        cur_y += y_vel;

        // simulate one step of the velocities
        x_vel += (x_vel > 0 ? -1 : 1);
        --y_vel;
    }

    // will eventually hit min_y or max_y
    // at this point projectile only going down

    if(!is_within(cur_x, min_x, max_x))
        return -1;

    while(cur_y >= min_y) {
        cur_y += y_vel--;

        if(cur_y > highest)
            highest = cur_y;

        if(is_within(cur_y, min_y, max_y))
            return highest;
    }

    return -1;
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

    auto x_y_hits = bind(do_initials_hit_maxy, placeholders::_1, placeholders::_2, 
        target_x_min, target_x_max, target_y_min, target_y_max);

    int goal_x, goal_y, max_val = -1;
    for(int i = min_x_attempt; i <= max_x_attempt; ++i) {
        for(int j = min_y_attempt; j <= max_y_attempt; ++j) {
            int cur = x_y_hits(i, j);
            if(cur > max_val) {
                max_val = cur; 
                goal_x = i;
                goal_y = j;
            }
        }
    }

    cout << goal_x << "," << goal_y << " " << max_val << endl;
}