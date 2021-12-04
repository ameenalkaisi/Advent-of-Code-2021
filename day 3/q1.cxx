#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

int main(int argc, char **argv) {
    ifstream input{argv[1]};

    if(!input)
        exit(1);

    vector<string> binary_nums{};

    // read each string and also find out highest bit count of a string
    string cur_string;
    int max_str_size = 0;
    while(input >> cur_string) {
        if(cur_string.size() > max_str_size)
            max_str_size = cur_string.size();
    
        binary_nums.push_back(cur_string);
    }

    // prepend 0's so that they match size for each binary num 
    for(string &str : binary_nums) {
        int size_diff = max_str_size - str.size();
        for(int i = 0; i < size_diff; ++i)
            str = "0" + str;
    }

    // go through each bit, find max and least of the binary number
    // for epsilon and gamma respectively
    int gamma, epsilon;
    gamma = epsilon = 0;
    for(int i = 0; i < max_str_size; ++i) {
        // find bit at that position
        int high_count, low_count;
        high_count = low_count = 0;
        for(const auto &j : binary_nums) {
            if(j.at(i) == '0')
                ++low_count;
            else if(j.at(i) == '1')
                ++high_count;
            else exit(2);
        }

        // add bit to espilon and gamma
        // note, since we are going form left to right
        // and a binary number goes like this 2^n 2^(n-1) ... 2^0
        // we can just add the bit like this
        if(high_count >= low_count)
            gamma += 1 * (int) pow(2, max_str_size - i - 1);
        else
            epsilon += 1 * (int) pow(2, max_str_size - i - 1);
    }

    cout << "gamma: " << gamma << "\tepsilon: " << epsilon 
        << "\n\tpower consumption: " << gamma * epsilon;
}