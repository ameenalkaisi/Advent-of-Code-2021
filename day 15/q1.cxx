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

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

vector<int> path_astar(vector<vector<int>> const &matrix, pair<int,int> const &start, pair<int, int> const &end) {
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

    vector<int> best_path = path_astar(matrix, {0, 0}, {matrix.size() - 1, matrix[0].size() - 1});

    cout << calculate_cost(best_path) - matrix[0][0] << endl;
}