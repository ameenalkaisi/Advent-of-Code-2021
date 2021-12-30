#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

template<typename T>
bool vector_contains(vector<T> const& v, T const& value) {
	if(find(v.begin(), v.end(), value) == v.end())
		return false;

	return true;
}

template<typename T>
void append_vectors(vector<T>& destination, const vector<T>& source) {
	for(auto const &i : source)
		destination.push_back(i);
}

long calculate_cost(vector<int> const &path) {
    long cost = 0;
    for(auto const &i : path)
        cost += i;

    return cost;
}

// needed for unordered map for pairs to something
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

vector<int> path_ucs(vector<vector<int>> const &matrix, pair<int,int> const &start, pair<int, int> const &end) {
    // coordinate to path to get to that coordinate pair
    unordered_map<pair<int, int>, vector<int>, hash_pair> visited;

    // for final result formulation purposes, have every matrix's coordinate be already visited with its own value
    for(int i = 0; i < matrix.size(); ++i)
        for(int j = 0; j < matrix[i].size(); ++j) 
            visited[{i, j}].push_back(matrix[i][j]);

    // prioritizes cost initially, otherwise prioritizes the value pairs according to how (x, y) is usually sorted
    auto comp = [](pair<long, pair<int, int>> a, pair<long, pair<int, int>> b) {
        if(a.first == b.first) {
            if(a.second.first == b.second.first)
                return b.second.second < a.second.second;

            return b.second.first < a.second.first;
        }

        return b.first < a.first;
    };

    // (cost, value) priority queue
    // this type of declaration needed since priority queue doesn't
    // support (long, (int, int)) pairs right away
    priority_queue<pair<long, pair<int, int>>, vector<pair<long, pair<int, int>>>, decltype(comp)> que(comp);

    que.push({0, start});

    while(!que.empty()) {
        auto [cost, coord] = que.top();
        que.pop();

        // if already visited, and the visisted's path was better, don't consider this
        if(visited[coord].size() > 1 && calculate_cost(visited[coord]) < cost)
            continue;

        if(coord == end) {
            return visited[coord];
        } else {
            // consider adjacents
            vector<pair<int, int>> children;

            if(coord.first + 1 < matrix.size())
                children.push_back({coord.first + 1, coord.second});

            if(coord.first - 1 >= 0)
                children.push_back({coord.first - 1, coord.second});

            if(coord.second + 1 < matrix[0].size())
                children.push_back({coord.first, coord.second + 1});

            if(coord.second - 1 >= 0)
                children.push_back({coord.first, coord.second - 1});

            for(auto &[i, j] : children) {
                vector<int> curpath;
                curpath.push_back(matrix[i][j]);

                append_vectors(curpath, visited[coord]);

                long curcost = calculate_cost(curpath);

                if(visited[{i, j}].size() > 1) {
                    long visited_cost = calculate_cost(visited[{i, j}]);

                    if(curcost < visited_cost) {
                        visited[{i, j}].clear();
                        visited[{i, j}].push_back(matrix[i][j]);
                    }
                    else {
                        que.push({curcost, {i, j}});
                        continue;
                    }
                }

                que.push({cost, {i, j}});
                append_vectors(visited[{i, j}], visited[coord]);
            }
        }

    }

    return {};
}

vector<vector<int>> expand_matrix(vector<vector<int>> matrix, int times) {
    int og_msize = matrix.size();
    int og_mzsize = matrix[0].size();

    if(og_msize == 0 || og_mzsize == 0)
        exit(2);

    int new_row_sz = times*matrix[0].size();

    // resize first
    matrix.resize(times*matrix.size());
    for(auto &i : matrix)
        i.resize(new_row_sz, -1);

    for(int i = 0; i < matrix.size(); ++i) {
        for(int j = 0; j < matrix[0].size(); ++j) {
            // i%og_msize and j%og_msize are the i and j if they went through original matrix
            // for the addition: if element is at the middle sector in a 3x3 matrix, (i/og_msize) will be 1, (j/og_msize) will be 1, so it is equal to 2
            // it will have been wrapped twice to get to that point and so 2 is the correct addition
            int new_val = matrix[i%og_msize][j%og_mzsize] + (i/og_msize + j/og_mzsize);

            // only works if times < 9 like this
            // too tired to make it work work for >= 9
            if(new_val >= 10)
                new_val -= 9;

            matrix[i][j] = new_val;
        }
    }

    return matrix;
}

int main(int argc, char **argv) {
    ifstream input{argv[1]};

    if(!input)
        exit(1);

    // get input
    vector<vector<int>> matrix;
    
    string line;
    while(getline(input, line)) {
        matrix.push_back(vector<int>{});

        istringstream iss{line};
        char next_char;
        while(iss >> next_char) {
            matrix.back().push_back(next_char - '0');
        }
    }

    // expand the matrix
    vector<vector<int>> full_matrix = expand_matrix(matrix, 5);

    // do ucs on the new matrix
    vector<int> best_path = path_ucs(full_matrix, {0, 0}, {full_matrix.size() - 1, full_matrix[0].size() - 1});

    cout << calculate_cost(best_path) - full_matrix[0][0] << endl;
}