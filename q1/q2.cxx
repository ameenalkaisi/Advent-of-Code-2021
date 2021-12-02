#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

class window {
public:
    int window_sz;

    vector<int> values;
    window(int n) : window_sz {n} {
        values = vector<int>{};
    }

    void reset() {
        values.clear();
    }

    bool add(int n) {
        if(values.size() < window_sz)
            values.push_back(n);
        else return false;
        
        return true;
    }

    bool is_full() {
        return values.size() == window_sz;
    }

    int sum() {
        int n = 0;
        for(const auto& i : values)
            n += i;
        
        return n;
    }
};

void print_vec(vector<int> vec) {
    for(auto i : vec)
        cout << i << " ";
    cout << endl;
}

int main(int argc, char **argv) {
    auto t1 = chrono::high_resolution_clock::now();
    ifstream file{argv[1]}; 
    
    window prev{3}, cur{3}, next{3};
    int n;
    
    file >> n;
    prev.add(n);

    file >> n;
    prev.add(n);
    cur.add(n);

    int count = 0;
    while(file >> n) {
        prev.add(n);
        cur.add(n);
        next.add(n);

        if(cur.is_full()) {
            if(cur.sum() > prev.sum())
                ++count;

            // okay because it is using copy constructor
            prev = cur;
            cur = next;
            next.reset();

            next.add(n);
        }
    }

    cout << count << endl;
    auto t2 = chrono::high_resolution_clock::now();

    cout << chrono::duration_cast<chrono::microseconds>(t2 - t1).count() << endl;
}