#include <iostream>
#include <fstream>

using namespace std;

struct pos_move {
    string direction;
    int value;
};

istream& operator>>(istream& in, pos_move& cur__move) {
    if(in >> cur__move.direction)
        in >> cur__move.value; // only get value if direction worked
    
    return in;
}

int main(int argc, char **argv) {
    ifstream input{argv[1]};

    if(!input)
        exit(1);

    pos_move cur_move;
    int hor = 0, ver = 0;
    while(input >> cur_move) {
        if(cur_move.direction == "up")
            ver -= cur_move.value;
        else if(cur_move.direction == "down")
            ver += cur_move.value;
        else if(cur_move.direction == "forward")
            hor += cur_move.value;
        else exit(2);
    }

    cout << "hor * ver = " << hor * ver << endl;
}