#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <stack>
#include <algorithm>
#include <sstream>

using namespace std;

template <typename T>
bool contains(vector<T> vec, T elem) {
    return find(vec.begin(), vec.end(), elem) != vec.end();
}

int main(int argc, char **argv) {
    ifstream input{argv[1]};
    if(!input)
        exit(1);

    string line;

    vector<char> openers = {'<', '{', '[', '('};
    vector<char> closers = {'>', '}', ']', ')'};

    vector<char> unexpecteds;
    
    
    vector<unsigned long long> scores;
    unordered_map<char, int> value_of_char = {
        {')', 1},
        {']', 2},
        {'}', 3},
        {'>', 4}
    };

    while(getline(input, line)) {
        stack<int> last_index{};

        bool is_corrupted = false;

        istringstream iss{line};
        for(char cur; iss >> cur;) {
            if(last_index.empty() && !contains(openers, cur)) {
                continue;
            } else {
                // if opener, add it's index
                if(contains(openers, cur)) {
                    last_index.push(find(openers.begin(), openers.end(), cur) - openers.begin()); // add index
                }
                // if closer, check same index
                else if(contains(closers, cur)) {
                    int expected_index = last_index.top();
                    if(cur != closers[expected_index]) {
                        is_corrupted = true;
                        break;
                    }
                    
                    last_index.pop();
                }
            }
        }

        // skip this iteration if corrupted
        if(is_corrupted)
            continue;

        unsigned long long total = 0;
        while(!last_index.empty()) {
            total = 5*total + value_of_char[closers[last_index.top()]];
            last_index.pop();
        }
        
        scores.push_back(total);
    }

    sort(scores.begin(), scores.end());

    std::cout << scores[scores.size()/2] << endl;
}