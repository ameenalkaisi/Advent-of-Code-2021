#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
#include <numeric>

using namespace std;

struct pair_num {
    virtual int get_value() {return -1;}
};

struct reg_num : public pair_num {
    int value;

    reg_num(int value) : value(value) {}

    int get_value() override {
        return value;
    }
};

struct real_pair : public pair_num {
    pair_num *left, *right;

    real_pair(pair_num *left, pair_num *right) : left {left}, right {right} {}

    int get_value() override {
        return 3*left->get_value() + 2*right->get_value();
    }
};

pair_num* add(pair_num * a, pair_num * b) {
    return new real_pair{a, b};
}

void print_pair_num(pair_num const *p) {
    real_pair const *cur_real = dynamic_cast<real_pair const*>(p);
    if(cur_real) {
        cout << '[';
        print_pair_num(cur_real->left);
        cout << ",";
        print_pair_num(cur_real->right);
        cout << ']';
    }

    reg_num const *cur_reg = dynamic_cast<reg_num const*>(p);
    if(cur_reg)
        cout << cur_reg->value;
}

pair_num *read_pair_(string line, int &index) {
    char starting_char = line[index++];
    if(starting_char == '[') {
        pair_num *left = read_pair_(line, index);
        
        ++index; // since elements are sepearated by commas move past them

        pair_num *right = read_pair_(line, index);

        ++index; // in recursive call, move past the right bracket as well

        return new real_pair{left, right};
    } else // if(starting_char >= '0' && starting_char <= '9')
        return new reg_num{starting_char - '0'};
}

pair_num *read_pair(string line) {
    int index = 0;
    return read_pair_(line, index);
}

bool is_parent_of_leaves(real_pair *real) {
    return real != nullptr && dynamic_cast<reg_num*>(real->left) != nullptr 
        && dynamic_cast<reg_num*>(real->right) != nullptr;
}

real_pair *get_left_most(real_pair *real) {
    real_pair *temp = real;
    do {
        real = temp;
        temp = dynamic_cast<real_pair*>(real->left);
    } while(temp !=  NULL);

    return real;
}

// very confusing due to having to deal with polymorphism alot
// but currently does work
bool explode(pair_num *num) {
    // list of regular numbers and their depths
    vector<pair<reg_num*, int>> num_depths;

    real_pair *pair_to_replace = nullptr;
    bool replace_left = true;

    stack<pair<pair_num*, int>> next_num{};
    next_num.push({num, 0});

    /*print_pair_num(num);
    cout << endl;*/
    while(!next_num.empty()) {
        auto [cur_num, cur_depth] = next_num.top();
        next_num.pop();
        
        /*cout << "--";
        print_pair_num(cur_num);
        cout << " " << cur_depth << endl;*/

        reg_num *reg = dynamic_cast<reg_num*>(cur_num);
        if(reg)
            num_depths.push_back({reg, cur_depth});

        real_pair *real = dynamic_cast<real_pair*>(cur_num);
        if(real) {
            next_num.push({real->right, cur_depth + 1});
            next_num.push({real->left, cur_depth + 1});

            // must find the parent of pair to replace
            // and if left or right of that pair is being replaced
            real_pair *left_pair = dynamic_cast<real_pair*>(real->left);
            real_pair *right_pair = dynamic_cast<real_pair*>(real->right);

            if(cur_depth == 3 && pair_to_replace == nullptr && is_parent_of_leaves(left_pair))
                pair_to_replace = real;
            
            if(cur_depth == 3 && pair_to_replace == nullptr && is_parent_of_leaves(right_pair)) {
                pair_to_replace = real;
                replace_left = false;
            }
        }
    }

    // regular numbers will appear in the order they are seen
    for(int i = 0; i < num_depths.size() - 1; ++i) {
        // first two consecutive numbers of same pair of depth 5 we should replace
        if(num_depths[i].second == 5 && num_depths[i + 1].second == 5) { // depth 4 of pair
            // do the additions to their adjacents
            if(i - 1 >= 0)
                num_depths[i - 1].first->value += num_depths[i].first->value;

            if(i + 2 < num_depths.size())
                num_depths[i + 2].first->value += num_depths[i + 1].first->value;

            // replace the parent pair
            // delete memory here
            if(replace_left) {
                delete pair_to_replace->left;

                pair_to_replace->left = new reg_num{0};
            }
            else {
                delete pair_to_replace->right;

                pair_to_replace->right = new reg_num{0};
            }

            // return true since it did explode
            return true;
        }
    }

    // return false if it went through everything and didn't find 
    // elements to explode
    return false;
}

bool pair_reduce(pair_num *num, real_pair *parent = nullptr, bool is_left = true) {
    reg_num *reg = dynamic_cast<reg_num*>(num);
    if(reg) {
        if(reg->value >= 10) {
            reg_num *new_left = new reg_num{(int)floor(reg->value/2.0)};
            reg_num *new_right = new reg_num{(int)ceil(reg->value/2.0)};
            if(is_left) {
                delete parent->left;
                parent->left = new real_pair{new_left, new_right};
            } else {
                delete parent->right;
                parent->right = new real_pair{new_left, new_right};
            }

            return true;
        }

        return false;
    }

    real_pair *real = dynamic_cast<real_pair*>(num);
    if(real)
        return pair_reduce(real->left, real, true) || pair_reduce(real->right, real, false);

    return false;
}

pair_num *copy_pair(pair_num *other) {
    real_pair *real = dynamic_cast<real_pair*>(other);
    if(real)
        return new real_pair{copy_pair(real->left), copy_pair(real->right)};

    reg_num *reg = dynamic_cast<reg_num*>(other);
    if(reg) {
        return new reg_num{reg->value};
    }

    return nullptr;
}

int get_sum(pair_num *a, pair_num *b) {
    pair_num *result = copy_pair(add(a, b));
    while(explode(result) || pair_reduce(result));

    return result->get_value();
}

int main(int argc, char **argv) {
    ifstream input{argv[1]};
    if(!input)
        exit(1);

    vector<pair_num*> nums;
    for(string line; getline(input, line);)
        nums.push_back(read_pair(line));

    int largest = 0;

    // go through every possible additon find largest and save it
    for(int i = 0; i < nums.size(); ++i) {
        for(int j = 0; j < nums.size(); ++j) {
            if(i == j)
                continue;

            int result = get_sum(nums[i], nums[j]);
            if(result > largest)
                largest = result;
        }
    }

    cout << largest << endl;
}