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

    while(getline(input, line)) {
        stack<int> last_index{};

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
                    if(cur != closers[expected_index])
                        unexpecteds.push_back(cur);
                    
                    last_index.pop();
                }
            }

        }


    }

    unordered_map<char, int> value_of_unexp = {
        {')', 3},
        {']', 57},
        {'}', 1197},
        {'>', 25137}
    };

    // calculate score
    // can probably be done at the spot
    long score = 0;
    for(char i : unexpecteds)
        score += value_of_unexp[i];

    std::cout << score << endl;
}