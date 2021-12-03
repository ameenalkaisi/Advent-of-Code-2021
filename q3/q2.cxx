#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

// returns 0 or 1 if most common bits are 0 or 1 respectively
// otherwise returns 2 if they are equally common
int most_common_bit_at(vector<string> binary_nums, int pos) {
    // find bit at that position
    int high_count, low_count;
    high_count = low_count = 0;
    for(const auto &j : binary_nums) {
        if(j.at(pos) == '0')
            ++low_count;
        else if(j.at(pos) == '1')
            ++high_count;
        else exit(2);
    }

    if(high_count == low_count)
        return 2;
    else 
        return (high_count > low_count);
}

int bin_str_to_int (string binary_num) {
    int result = 0;
    for(int i = 0; i < binary_num.size(); ++i) {
        if(binary_num.at(i) == '1')
            result += (int) pow(2, binary_num.size() - i - 1);
    }

    return result;
}

// select binary numbers that have 0 or 1 as their initials only
vector<string> bin_nums_with_digit_at(vector<string> binary_nums, char initial, int pos) {
    vector<string> result;
    for(const auto &str : binary_nums) {
        if(str.at(pos) == initial)
            result.push_back(str);
    } 

    return result;
}

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

    vector<string> most_common_strings {binary_nums};
    vector<string> least_common_strings {binary_nums};

    // won't work with empty input lol
    int digit_to_find = 0;
    while(most_common_strings.size() > 1) {
        if(most_common_bit_at(most_common_strings, digit_to_find) >= 1)
            most_common_strings = bin_nums_with_digit_at(most_common_strings, '1', digit_to_find);
        else
            most_common_strings = bin_nums_with_digit_at(most_common_strings, '0', digit_to_find);

        ++digit_to_find;
    }

    digit_to_find = 0;
    while(least_common_strings.size() > 1) {
        int least_common_bit = most_common_bit_at(least_common_strings, digit_to_find);
        if(least_common_bit >= 1)
            least_common_bit = 0;
        else least_common_bit = 1;

        if(least_common_bit == 1)
            least_common_strings = bin_nums_with_digit_at(least_common_strings, '1', digit_to_find);
        else
            least_common_strings = bin_nums_with_digit_at(least_common_strings, '0', digit_to_find);

        ++digit_to_find;
    }

    int ox_rating = bin_str_to_int(most_common_strings[0]);
    int co_rating = bin_str_to_int(least_common_strings[0]);
    cout << "oxygen generator rating: " << ox_rating << "\tco2 scrubber rating: " << co_rating
        << "\n\tlife support rating: " << ox_rating * co_rating << endl;
}