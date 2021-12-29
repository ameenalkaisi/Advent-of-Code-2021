#include <iostream>
#include <unordered_map>
#include <fstream>

using namespace std;

string pair_insertion(string const &template_s, unordered_map<string, char> const &char_in_between) {
    string result = "";

    for(int i = 0; i < template_s.size() - 1; ++i) {
        // get each pair from the original string and its supposed in between character
        string cur_pair = string{template_s[i]} + string{template_s[i + 1]};
        const char placeable = char_in_between.at(cur_pair);

        // add to result only beginning character and in between character
        // template_s[i + 1] will be added next time
        result += template_s[i];
        result += placeable;
    }

    // add last character to result then it should be complete
    result += template_s.back();

    return result;
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

    for(int i = 0; i < 10; ++i)
        template_s = pair_insertion(template_s, char_in_between);

    // get the score
    unordered_map<char, long> frequencies;

    for(char const &i : template_s)
        long cur_freq = ++frequencies[i];

    long highest, lowest;
    highest = lowest = -1;
    for(auto const &[cur_char, cur_freq] : frequencies) {
        if(highest == -1 || cur_freq > highest)
            highest = cur_freq;

        if(lowest == -1 || cur_freq < lowest)
            lowest = cur_freq;
    }

    cout << highest - lowest << endl;
}