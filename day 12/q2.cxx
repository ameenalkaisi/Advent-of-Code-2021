#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_map>

using namespace std;

struct node {
    vector<node*> adj_nodes;
    string value;

    node() {};

    node(string value) : value {value} {}

    bool is_upper() const {
        return (value[0] >= 'A' && value[0] <= 'Z');
    }

    bool operator==(node const &other) {
        return value==other.value;
    }
};

void set_linked(node &a, node &b) {
    a.adj_nodes.push_back(&b);
    b.adj_nodes.push_back(&a);
}

template<typename T>
void append_lists(vector<T> &a, vector<T> const &b) {
    a.insert(a.end(), b.begin(), b.end());
}

template <typename T>
bool contains(vector<T> vec, T elem) {
    return find(vec.begin(), vec.end(), elem) != vec.end();
}

// brute force method for part 2
void add_paths(node start, node end, node actual_start, vector<vector<node>> &paths, unordered_map<string, int> visited = {}, vector<node> cur_path = {}) {
    ++visited[start.value];
    cur_path.push_back(start);

    // terminate early if number of elements at 2 exceeds 1
    int small_gt2_count = 0;
    for(auto [i, j] : visited) {
        // kinda hacky here since node is supposed to have this functionality
        // if it's not upper case and it's occurrence 2 or more
        if((i[0] < 'A' || i[0] > 'Z') && visited[i] == 2)
            ++small_gt2_count;
    }

    if(small_gt2_count > 1)
        return;

    // only proper path if end and actual start have been visited once
    // this also implicitly includes condition that at most one small element has up to 2 visits
    if(start == end && visited[end.value] == 1 && visited[actual_start.value] == 1) {
        paths.push_back(cur_path);
    }

    for(auto i : start.adj_nodes) {
        if(visited[i->value] >= 2 && !i->is_upper())
            continue;

        add_paths(*i, end, actual_start, paths, visited, cur_path);
    }
}

vector<vector<node>> all_paths(node start, node end) {
    vector<vector<node>> paths;

    add_paths(start, end, start, paths);

    return paths;
}

int main(int argc, char **argv) {
    ifstream input{argv[1]};

    if(!input)
        exit(1);

    // initialize all the nodes and their adjacency lists
    unordered_map<string, node> nodes{};

    string line;
    while(getline(input, line)) {
        // parse the string
        // add to the lists as necessary
        string left_side, right_side;

        istringstream iss{line};
        getline(iss, left_side, '-');
        iss >> right_side; // if I use getline again will have problems with \r\n

        // if left and right aren't already in nodes, initialize their values
        if(!nodes.count(left_side))
            nodes[left_side].value = left_side;
        
        if(!nodes.count(right_side))
            nodes[right_side].value = right_side;

        set_linked(nodes[left_side], nodes[right_side]);
    }

    cout << all_paths(nodes["start"], nodes["end"]).size() << endl;
}