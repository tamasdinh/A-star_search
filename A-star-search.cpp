//
// Created by Tamás Dinh on 2019-05-25.
//
// The program takes in a series of binary values (an ASCII table of 0s and 1s) from a file and
// finds the shortest path between a user-defined starting point and an end-point using the A* algorithm,
// taking into consideration any cells that are marked as an obstacle.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using std::vector;
using std::cout;
using std::ifstream;
using std::string;
using std::istringstream;
using std::abs;
using std::sort;

enum class State {cellEmpty, cellObstacle, cellClosed, cellInPath, cellStart, cellFinish};

string cellString(State cell) {
    switch (cell) {
        case State::cellObstacle: return "⛰ ️";
        case State::cellStart: return "🚦  ";
        case State::cellFinish: return "🏁  ";
        case State::cellInPath: return "🚗   ";
        default : return " 0   ";
    }
}

vector<State> ParseLine(string line) {
    vector<State> lineVector;
    istringstream lineStream(line);
    int num;
    char comma;
    while (lineStream >> num >> comma && comma == ',') {
        if (num == 1) {
            lineVector.push_back(State::cellObstacle);
        } else {
            lineVector.push_back(State::cellEmpty);
        }
    }
    return lineVector;
}

vector<vector<State>> ReadBoard(string path) {
    ifstream boardfile(path);
    vector<vector<State>> board{};
    if (boardfile) {
        string line;
        vector<State> tempv;
        while (getline(boardfile, line)) {
            tempv = ParseLine(line);
            board.push_back(tempv);
        }
    } else {
        cout << "File stream creation failed" << std::endl;
    }
    return board;
}

void PrintBoard(const vector<vector<State>> a) {
    for (auto row: a) {
        for (State cell: row) {
            cout << cellString(cell);
        }
        cout << std::endl;
    }
}

// Implementing A* search algorithm functions

void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openVec, vector<vector<State>> &grid) {
    vector<int> node {x, y, g, h};
    openVec.push_back(node);
    grid[x][y] = State::cellClosed;
}

int Heuristic(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

bool Compare(const vector<int> node1, const vector<int> node2) {
    int node1f = node1[2] + node1[3];
    int node2f = node2[2] + node2[3];
    return node1f > node2f;
}

void CellSort(vector<vector<int>> *vect) {
    sort(vect->begin(), vect->end(), Compare);
}

bool CheckValidCell(int x, int y, vector<vector<State>> &board) {
    bool on_grid_x = (x >= 0 && x < board.size());
    bool on_grid_y = (y >= 0 && y < board[0].size());
    if (on_grid_x && on_grid_y)
        return (board[x][y] == State::cellEmpty || board[x][y] == State::cellFinish);
    return false;
}

void ExpandNeighbours(vector<int> &currentNode, vector<vector<int>> &open,
                      vector<vector<State>> &board, int goalCoord[2]) {
    int x = currentNode[0];
    int y = currentNode[1];
    const vector<vector<int>> neighbours {{x - 1, y - 0},
                                    {x - 0, y - 1},
                                    {x + 1, y - 0},
                                    {x - 0, y + 1}};

    for (auto i: neighbours) {
        if (CheckValidCell(i[0], i[1], board)) {
            vector<int> node {i[0], i[1], currentNode[3] + 1, 0};
            node[3] = Heuristic(node[0], node[1], goalCoord[0], goalCoord[1]);
            AddToOpen(node[0], node[1], node[2], node[3], open, board);
        }
    }
}

vector<vector<State>> Search(vector<vector<State>> board, int startCoord[2], int goalCoord[2]) {
    vector<vector<int>> open {};
    int x = startCoord[0];
    int y = startCoord[1];
    board[x][y] = State::cellStart;
    board[goalCoord[0]][goalCoord[1]] = State::cellFinish;
    int g = 0;
    int h = Heuristic(startCoord[0], startCoord[1], goalCoord[0], goalCoord[1]);
    AddToOpen(x, y, g, h, open, board);

    while (open.size() > 0) {
       CellSort(&open);
       vector<int> currentNode = open.back();
       open.pop_back();
       x = currentNode[0];
       y = currentNode[1];
       board[x][y] = State::cellInPath;
       if (x == goalCoord[0] && y == goalCoord[1]) {
           cout << "\nFoung a solution! Printing board...\n";
           return board;
       }
       ExpandNeighbours(currentNode, open, board, goalCoord);
    }
    cout << "No path found!" << std::endl;
    return board;
}

int main() {

    cout << "\nInitial board:\n";
    auto board = ReadBoard("/Users/tamasdinh/Dropbox/Data-Science_suli/0_NOTES/10_C++/1_A-star-search/1.board");
    PrintBoard(board);
    cout << std::endl;

    int startCoord[2] {0, 0};
    int goalCoord[2] {4, 5};
    auto solution = Search(board, startCoord, goalCoord);
    PrintBoard(solution);

}
