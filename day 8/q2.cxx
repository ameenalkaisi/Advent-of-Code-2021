#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>

constexpr int LINE_BUF = 512;

using namespace std;

bool is_superset(string const& super, string const& under) {
    // false if there is at least one character not in super
    for(int i = 0; i < under.size(); ++i) {
        if(find(super.begin(), super.end(), under[i]) == super.end())
            return false;
    }

    // true otherwise
    return true;
}

template <typename T, typename J>
bool has_mapping(map<T, J> mapping, T key) {
    return mapping.count(key);
}

int main(int argc, char **argv) {
    map<int, int> size_to_digit{{2, 1}, {4, 4}, {3, 7}, {7, 8}};

    ifstream finput{argv[1]};
    if(!finput)
        exit(1);

    string line;

    deque<string> unknowns{};
    
    int total = 0;
    while(getline(finput, line)) {
        map<int, string> num_set{};
        istringstream iss(line);

        string cur_string;

        // every entry is required to have a delimeter and output after, so 
        // now we just skip them and go to the output
        while(iss >> cur_string) {
            if(cur_string == "|")
                break;

            // if it is a known length, add this string to known digits
            int cur_size = static_cast<int>(cur_string.size());

            // sort first, then add to known if it is otherwise to unknown
            sort(cur_string.begin(), cur_string.end());
            if(size_to_digit.count(cur_size))
                num_set[size_to_digit[cur_size]] = cur_string;
            else 
                unknowns.push_back(cur_string);
        }

        // complete rest of num_set through the unknowns
        // note: this appraoch was inspired by github/NickyMeuleman
        while(!unknowns.empty()) {
            string unknown = unknowns.back();
            int cur_size = static_cast<int>(unknown.size());

            if(cur_size == 6 && is_superset(unknown, num_set[4]))
                num_set[9] = unknown;
            else if(cur_size == 5 && is_superset(unknown, num_set[1]))
                num_set[3] = unknown;
            else if(cur_size == 6 && is_superset(unknown, num_set[1]))
                num_set[0] = unknown;
            else if(cur_size == 6)
                num_set[6] = unknown;
            else if(cur_size == 5 && has_mapping(num_set, 6) && is_superset(num_set[6], unknown))
                num_set[5] = unknown;
            else if(cur_size == 5 && has_mapping(num_set, 6))
                num_set[2] = unknown;
            else
                unknowns.push_front(unknown);

            unknowns.pop_back();
        }

        int count = 0;
        for(int index = 0; iss >> cur_string; ++index) {
            sort(cur_string.begin(), cur_string.end());

            for(int i = 0; i <= 9; ++i) {
                if(num_set[i] == cur_string) {
                    count = 10 * count + i;

                    break;
                }
            }

            if(index == 3)
                std::cout << cur_string << " " << count << endl;
        }

        total += count;
    }

    std::cout << total << endl;
}