#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct point {
    int x, y;

    point(int x, int y) : x {x}, y {y} {}
};

struct line {
    point p1, p2;

    line(int p1x, int p1y, int p2x, int p2y) : p1 {p1x, p1y}, p2 {p2x, p2y} {}
};

struct graph {
    vector<vector<int>> values;

    // intiailize the graph's values as 0
    graph(int x_size, int y_size) {
        for(int i = 0; i < x_size; ++i) {
            values.push_back(vector<int>{});

            for(int j = 0; j < y_size; ++j)
                values[i].push_back(0);
        }
    }

    void add_line(line const &line_) {
        add_line(line_.p1, line_.p2);
    }

    void add_line(point p1, point p2) {
        bool is_x_varying = true;

        // ensure that p1's x is lesser than p2's
        if(p1.x > p2.x)
            swap(p1, p2);

        float slope_y = 0;
        if(p1.x != p2.x) 
            slope_y = ((float)p2.y - p1.y)/((float) p2.x - p1.x); 

        if(p1.x != p2.x)
            // works if both points or only x varies
            for(point point = p1; point.x != p2.x; ++point.x, point.y += (int) floor(slope_y))
                ++values[point.x][point.y];
        else {
            // works if only y varies
            if(p1.y > p2.y)
                swap(p1, p2);
            
            for(point point = p1; point.y != p2.y; ++point.y) 
                ++values[point.x][point.y];
        }
        

        ++values[p2.x][p2.y];
    }
};

int main(int argc, char **argv) {
    vector<line> lines;

    // read input
    FILE *input = fopen(argv[1], "r");

    if(!input)
        exit(1);

    int p1x, p1y, p2x, p2y;

    // also determine max graph sizes while reading the lines
    int x_size, y_size;
    x_size = y_size = 0;

    while(fscanf(input, "%d,%d -> %d,%d", &p1x, &p1y, &p2x, &p2y) > 0) {
        if(p1x > x_size)
            x_size = p1x;

        if(p2x > x_size)
            x_size = p2x;

        if(p1y > y_size)
            y_size = p1y;
        
        if(p2y > y_size)
            y_size = p2y;

        lines.push_back(line{p1x, p1y, p2x, p2y});
    }

    ++x_size;
    ++y_size;

    fclose(input);

    graph graph_{x_size, y_size};
    for(line &line_ : lines)
        graph_.add_line(line_);

    int points_intersected = 0;
    for(int i = 0; i < x_size; ++i) {
        for(int j = 0; j < y_size; ++j) {
            if(graph_.values[i][j] >= 2)
                ++points_intersected;
        }
    }
    /*
    while(!lines.empty() || lines.size() > 1) {
        // get last line, compare it with every other line
        line last_line = *(lines.end() - 1);

        for(int i = 0; i < lines.size() - 1; ++i)
            if(last_line.overlaps(lines[i]))
                ++points_intersected;

        lines.pop_back();
    */

    cout << points_intersected << endl;
}