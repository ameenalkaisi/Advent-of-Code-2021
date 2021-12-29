#include <iostream>
#include <unordered_map>
#include <map>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
bool contains(vector<T> vec, T elem) {
    return find(vec.begin(), vec.end(), elem) != vec.end();
}

template <typename T>
void print_vec(vector<T> const &vec) {
    for(auto i : vec)
        cout << i << " ";
    cout << endl;
}

// resulting pairs and between_char are for information only
// changing rule_visit_levle, freqs depending on step
void simulate_steps(map<string, pair<string, string>> const &resulting_pairs, 
    map<string, unordered_map<int, unsigned long long>> &rule_visit_level, 
    unordered_map<char, unsigned long long> &freqs, 
    unordered_map<string, char> const &between_char,
    int step) {
    // go through each visited rule
    for(auto &[rule, counts_at_level] : rule_visit_level) {
        // get number of times this rule was used in the last step
        unsigned long long rule_presence_count = counts_at_level[step - 1];

        // if it hasn't been used, continue
        if(rule_presence_count == 0)
            continue;

        // for both of its following rules
        auto [left, right] = resulting_pairs.at(rule);

        // state that they were visited same number of times as this rule in last step
        rule_visit_level[left][step] += rule_presence_count;
        rule_visit_level[right][step] += rule_presence_count;

        // character between the current rule occurs number of times rule was there
        freqs[between_char.at(rule)] += rule_presence_count;
    }
}

unordered_map<char, unsigned long long> pair_insert(string const &template_s, unordered_map<string, char> const &char_in_between, int steps) {
    // result
    unordered_map<char, unsigned long long> freqs{};

    // intialize resulting pairs map
    // e.g., AB -> (AA, AB) if AB -> A is a rule
    map<string, pair<string, string>> resulting_pairs{};
    for(auto const &[rule, between] : char_in_between) {
        string left = string{rule[0]} + between;
        string right = string{between} + rule[1];

        resulting_pairs[rule] = {left, right};
    }

    // rule used at which step
    map<string, unordered_map<int, unsigned long long>> rule_visited_at{};

    for(int i = 0; i < template_s.size() - 1; ++i) {
        string cur_pair = template_s.substr(i, 2);

        ++rule_visited_at[cur_pair][0]; // at step 0, this rule was used 

        ++freqs[template_s[i]]; // add initial letters in as well
    }
    ++freqs[template_s.back()];

    // simulate steps and update the frequencies
    for(int i = 1; i <= steps; ++i)
        simulate_steps(resulting_pairs, rule_visited_at, freqs, char_in_between, i);

    return freqs;
}

int main(int argc, char **argv) {
    ifstream input{argv[1]};

    if(!input)
        exit(1);

    // get the template
    string template_s;
    input >> template_s;

    // get the rules
    unordered_map<string, char> char_in_between{};

    string left_side;
    char right_side;
    while(input >> left_side) {
        input.ignore(4); // ignore the " -> " immediately

        input >> right_side;

        char_in_between[left_side] = right_side;
    }

    // get the frequencies somehow
    unordered_map<char, unsigned long long> frequencies = pair_insert(template_s, char_in_between, 40);

    // count the scores
    long highest, lowest;
    highest = lowest = -1;
    for(auto const &[cur_char, cur_freq] : frequencies) {
        if(highest == -1 || cur_freq > highest)
            highest = cur_freq;

        if(lowest == -1 || cur_freq < lowest)
            lowest = cur_freq;
    }

    for(auto [c, s] : frequencies)
        cout << c << ": " << s << endl;

    cout << highest - lowest << endl;
}