#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

int evaluate(const vector<vector<char>>& board) {
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == 'X') return 10;
            else if (board[row][0] == 'O') return -10;
        }
    }

    for (int col = 0; col < 3; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == 'X') return 10;
            else if (board[0][col] == 'O') return -10;
        }
    }
    
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == 'X') return 10;
        else if (board[0][0] == 'O') return -10;
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == 'X') return 10;
        else if (board[0][2] == 'O') return -10;
    }

    return 0;
}

bool isMovesLeft(const vector<vector<char>>& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == ' ') return true;
        }
    }
    return false;
}

int minimax(vector<vector<char>>& board, int depth, bool isMax) {
    int score = evaluate(board);

    if (score == 10 || score == -10) return score;

    if (!isMovesLeft(board)) return 0;

    if (isMax) {
        int best = -1000;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    best = max(best, minimax(board, depth + 1, !isMax));
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    } else {
        int best = 1000;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    best = min(best, minimax(board, depth + 1, !isMax));
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
}

pair<int, int> bestMove(vector<vector<char>>& board, char player) {
    int bestVal = (player == 'X') ? -1000 : 1000;
    pair<int, int> bestMove = {-1, -1};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = player;
                int moveVal = minimax(board, 0, player == 'O');
                board[i][j] = ' ';
                if ((player == 'X' && moveVal > bestVal) || (player == 'O' && moveVal < bestVal)) {
                    bestMove = {i, j};
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

int simulateGame(vector<vector<char>>& board, char player) {
    while (isMovesLeft(board) && evaluate(board) == 0) {
        auto move = bestMove(board, player);
        if (move.first != -1) {
            board[move.first][move.second] = player;
            player = (player == 'X') ? 'O' : 'X';
        } else {
            break;
        }
    }
    return evaluate(board);
}

int main() {
    ifstream file("dataset.txt");
    string line;
    int winsForX = 0, winsForO = 0, draw = 0, cpt = 0;
    auto start = chrono::high_resolution_clock::now();

    while (getline(file, line)) {
        cpt++;
        char player = line[0];
        vector<vector<char>> board(3, vector<char>(3));
        int index = 1;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = line[index++];
            }
        }

        int result = simulateGame(board, player);
        if (result == 10) winsForX++;
        else if (result == -10) winsForO++;
        else draw++;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Nombre total de parties gagnées par 'X' : " << winsForX <<" soit "<<((float)winsForX/cpt)*100<<"%"<<endl;
    cout << "Nombre total de parties gagnées par 'O' : " << winsForO <<" soit "<<((float)winsForO/cpt)*100<<"%"<<endl;
    cout << "Nombre total de parties nulles : " << draw <<" soit "<<((float)draw/cpt)*100<<"%"<<endl;
    cout << "Temps écoulé : " << elapsed.count() << "s" << endl;

    return 0;
}