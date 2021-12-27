#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

struct coord {
    int x, y;
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

    // only doing first fold instruction for now
    int fold;
    fscanf(input, "fold along ");

    if(fscanf(input, "x=%d", &fold) > 0) {
        // do the folding logic
        for(coord &i : coords) {
            if(i.x > fold)
                i.x = 2*fold - i.x;
        }

    } else if(fscanf(input, "y=%d", &fold) > 0) {
        for(coord &i : coords)
            if(i.y > fold)
                i.y = 2*fold - i.y;
    }

    // after the fold remove all duplicates and check the number
    sort(coords.begin(), coords.end(), [](coord i, coord j) {
        if(i.x == j.x)
            return i.y < j.y;
        
        return i.x < j.x;
    });

    auto last = unique(coords.begin(), coords.end(), [](coord const &i, coord const &j) {
        return (i.x == j.x && i.y == j.y);
    });
    coords.erase(last, coords.end());

    cout << coords.size() << endl;
}