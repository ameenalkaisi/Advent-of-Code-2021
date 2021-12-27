#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstdio>

using namespace std;

struct coord {
    int x, y;

    bool operator==(coord const &other) {
        return x == other.x && y == other.y;
    }

    bool operator<(coord const &other) {
        if(x == other.x)
            return y < other.y;
        return x < other.x;
    }
};

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");
    if(!input)
        exit(1);

    // set up the matrix
    vector<coord> coords;
    int x, y;
    while(fscanf(input, "%d,%d", &x, &y) > 0)
        coords.push_back(coord{x, y});

    // get each line
    char line[100];
    while(fgets(line, 100, input)) {
        // do folding logic differently if it's x or y
        int fold;
        if(sscanf(line, "fold along x=%d", &fold) > 0) {
            for(coord &i : coords) {
                if(i.x > fold)
                    i.x = 2*fold - i.x;
            }

        } else if(sscanf(line, "fold along y=%d", &fold) > 0) {
            for(coord &i : coords)
                if(i.y > fold)
                    i.y = 2*fold - i.y;
        }

        // after the fold remove all duplicates and check the number

        // std::unique requires sorted vector
        sort(coords.begin(), coords.end());

        // unique moves all the duplicates to the position last, then need to erase from last to the end
        auto last = unique(coords.begin(), coords.end());
        coords.erase(last, coords.end());
    }

    // print picture of coordinates
    for(int i = 0; i < 100; ++i) {
        for(int j = 0; j < 100; ++j) {
            cout << " "; // add a space for clarity
            
            // i represents y, j represnts x, so do it in this order
            if(find(coords.begin(), coords.end(), coord{j, i}) != coords.end())
                cout << "#";
            else cout << ".";
        }
        cout << endl;
    }
}