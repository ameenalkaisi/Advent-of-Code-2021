#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

constexpr int BOARD_SIZE = 5;

struct board {
    vector<vector<int>> board_vals;

    void add_row(vector<int> const &row) {
        board_vals.push_back(row);
    }

    bool has_won(vector<int> const &bingo_nums) const {
        for(int i = 0; i < BOARD_SIZE; ++i) {
            bool has_row_won = true, has_column_won = true;

            // check if a full row or column has won
            // if both end early then break early
            for(int j = 0; j < BOARD_SIZE; ++j) {
                if(!is_num_marked(board_vals[i][j], bingo_nums))
                    has_row_won = false;

                if(!is_num_marked(board_vals[j][i], bingo_nums))
                    has_column_won = false;

                if(!has_row_won && !has_column_won)
                    break;
            }

            if(has_row_won || has_column_won)
                return true;
        }

        return false;
    }

    int get_unmarked_sum(vector<int> const &bingo_nums) const {
        int sum = 0;

        // go through each number, if it's not marked add it into sum
        for(auto const &row : board_vals)
            for(auto const &value : row) {
                if(!is_num_marked(value, bingo_nums))
                    sum += value;
            }


        return sum;
    }

private:
    bool is_num_marked(int num, vector<int> const &bingo_nums) const {
        return find(bingo_nums.begin(), bingo_nums.end(), num) != bingo_nums.end();
    }
};

int main(int argc, char **argv) {
    ifstream input{argv[1]};

    if(!input)
        exit(1);

    vector<int> bingo_nums{};

    // read first line from file
    string first_line;
    getline(input, first_line, '\n');

    // read integers into th vector
    stringstream ss(first_line);
    for (int i; ss >> i;) {
        bingo_nums.push_back(i);    
        if (ss.peek() == ',')
            ss.ignore();
    }

    // now read each board
    vector<board> boards{};

    // read all the boards
    int next_int;
    while(input >> next_int) {
        board new_board{};

        vector<int> first_row{};
        
        // grab rest of the first row
        first_row.push_back(next_int);
        for(int i = 0; i < BOARD_SIZE - 1; ++i) {
            input >> next_int;
            if(!input)
                exit(2);

            first_row.push_back(next_int);
        }

        new_board.add_row(first_row);

        // add rest of the other rowshas
        for(int i = 0; i < BOARD_SIZE - 1; ++i) {
            vector<int> next_row{};
            for(int j = 0; j < BOARD_SIZE; ++j) {
                input >> next_int;
                if(!input)
                    exit(2);

                next_row.push_back(next_int);
            }

            new_board.add_row(next_row);
        }


        boards.push_back(new_board);
    }

    /* output boards for testing
    for(auto i : boards) {
        for(auto row : i.board_vals) {
            for(auto value : row)
                cout << value << " ";
            cout << endl;
        }
        cout << endl;
    }
    */

    // next, simulate the game on each board until either a winner is drawn or all values are gone
    vector<int> cur_bingo_nums{};
    int result = -1;
    
    for(int const &bingo_val : bingo_nums) {
        // take the bingo number add it to list considered
        cur_bingo_nums.push_back(bingo_val);

        // go through all boards see if any won with this bingo num
        for(board const &cur_board : boards)
            if(cur_board.has_won(cur_bingo_nums)) {
                // looking to get ALL unmarked sum so use bingo_nums in this calculation
                result = bingo_val * cur_board.get_unmarked_sum(cur_bingo_nums);

                break;
            }

        if(result != -1)
            break;
    }

    if(result == -1)
        exit(3);

    cout << "score: " << result << endl;
}