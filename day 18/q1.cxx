#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct pair_num {
    int depth;
    vector<pair_num*> all_nums;
    vector<pair_num*> all_four_depths;
    
    pair_num(int depth) : depth{depth} {
        if(depth == 4)
            all_four_depths.push_back(this);
    }
    
    virtual int get_value() {return -1;}
    virtual void increment_depth() {}
};

struct reg_num : public pair_num {
    int value;

    reg_num(int depth, int value) : pair_num::pair_num(depth), value(value) {
        all_nums.push_back(this);
    }

    int get_value() override {
        return value;
    }

    void increment_depth() override {
        ++depth;
    }
};

/*
reg_num *get_reg_num(pair_num *p) {
    return dynamic_cast<reg_num*>(p);
}

bool is_reg_num(pair_num *p) {
    return get_reg_num(p) != nullptr;
}
*/

struct real_pair : public pair_num {
    pair_num *left, *right;

    real_pair(int depth, pair_num *left, pair_num *right) : pair_num::pair_num(depth), left {left}, right {right} {}

    /*
    bool explode() {
        bool has_exploded = false;
        for(int i = 0; i < all_four_depths.size(); ++i) {
            real_pair *cur_real = dynamic_cast<real_pair*>(all_four_depths[i]);
            if(cur_real) {
                // get left-most pair
                while(!is_reg_num(cur_real->left))
                    cur_real = dynamic_cast<real_pair*>(cur_real->left);

                reg_num *cur_left = get_reg_num(cur_real->left);
                reg_num *cur_right = get_reg_num(cur_real->right);

                int left_index = find(all_nums.begin(), all_nums.end(), cur_real->left) - all_nums.begin();
                int right_index = find(all_nums.begin(), all_nums.end(), cur_real->right) - all_nums.begin();

                if(left_index != 0)
                    get_reg_num(all_nums[left_index - 1])->value += cur_left->value;

                if(right_index != all_nums.size() - 1)
                    get_reg_num(all_nums[right_index + 1])->value += cur_right->value;

                *all_four_depths[i] = reg_num{4, 0}; // replace current by a 0 regular number

                has_exploded = true;
            }
        }

        return has_exploded;
    }

    bool reduce() {
        bool has_reduced = false;
        for(int i = 0; i < all_nums.size(); ++i) {
            if(get_reg_num(all_nums[i])->value >= 10) {
                int cur_depth = all_nums[i]->depth;

                int left_val = (int)floor(all_nums[i]->get_value()/2.0);
                int right_val = (int)ceil(all_nums[i]->get_value()/2.0);

                reg_num new_left = reg_num{cur_depth + 1, left_val};
                reg_num new_right = reg_num{cur_depth + 1, right_val};

                // replace that element by new pair
                *all_nums[i] = real_pair{cur_depth, &new_left, &new_right};

                // delete this element from all_nums as it is no longer a regular number
                all_nums.erase(all_nums.begin() + i);

                has_reduced = true;
            }
        }

        return has_reduced;
    }
    */
};

pair_num* add(pair_num * a, pair_num * b) {
    a->increment_depth();
    b->increment_depth();

    return new real_pair{0, a, b};
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

pair_num *read_pair_(string line, int &index, int depth = 0) {
    char starting_char = line[index++];
    if(starting_char == '[') {
        pair_num *left = read_pair_(line, index, depth + 1);
        
        ++index; // since elements are sepearated by commas move past them

        pair_num *right = read_pair_(line, index, depth + 1);

        ++index; // in recursive call, move past the right bracket as well

        return new real_pair{depth, left, right};
    } else // if(starting_char >= '0' && starting_char <= '9')
        return new reg_num{depth, starting_char - '0'};
}

pair_num *read_pair(string line) {
    int index = 0;
    return read_pair_(line, index);
}

int main(int argc, char **argv) {
    ifstream input{argv[1]};
    if(!input)
        exit(1);

    vector<pair_num*> nums;
    for(string line; getline(input, line);)
        nums.push_back(read_pair(line));

    // for now only working with first 2
    pair_num *a = read_pair("[0,1]");
    pair_num *b = read_pair("[2,2]");

    print_pair_num(add(a, b));
}