#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char **argv) {
    // argc = 1
    
    ifstream file{argv[1]}; 
    
    int prev, cur;
    file >> prev;


    int count = 0;
    while(file >> cur) {
        if(cur > prev)
            ++count;
        prev = cur;
    }

    cout << count << endl;
}
