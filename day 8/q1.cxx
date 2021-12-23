#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

constexpr int LINE_BUF = 512;

using namespace std;

int main(int argc, char **argv) {
    map<int, int> size_to_digit{{2, 1}, {4, 4}, {3, 7}, {7, 8}};

    ifstream finput{argv[1]};
    if(!finput)
        exit(1);

    string line;

    int count = 0;
    while(getline(finput, line)) {
        istringstream iss(line);

        string cur_string;

        // every entry is required to have a delimeter and output after, so 
        // now we just skip them and go to the output
        while(iss >> cur_string) {
            if(cur_string == "|")
                break;
        }

        while(iss >> cur_string) {
            if(size_to_digit.count(static_cast<int>(cur_string.size())))
                ++count;
        }
    }

    cout << count << endl;
}