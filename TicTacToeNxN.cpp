#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

// Taille de la grille, à définir en fonction de votre dataset
const int N = 6;

// Évalue l'état actuel du plateau de jeu pour déterminer un score
int evaluate(const vector<vector<char>>& board) {
    // Vérifie chaque ligne et chaque colonne pour une victoire
    for (int i = 0; i < N; i++) {
        bool rowWin = true;
        bool colWin = true;
        for (int j = 1; j < N; j++) {
            rowWin &= (board[i][j] == board[i][0] && board[i][0] != ' ');
            colWin &= (board[j][i] == board[0][i] && board[0][i] != ' ');
        }
        if (rowWin || colWin) {
            return (board[i][0] == 'X' || board[0][i] == 'X') ? 10 : -10;
        }
    }

    // Vérifie les deux diagonales pour une victoire
    bool diagWin1 = true, diagWin2 = true;
    for (int i = 1; i < N; i++) {
        diagWin1 &= (board[i][i] == board[0][0] && board[0][0] != ' ');
        diagWin2 &= (board[N-i-1][i] == board[N-1][0] && board[N-1][0] != ' ');
    }
    if (diagWin1 || diagWin2) {
        return (board[0][0] == 'X' || board[N-1][0] == 'X') ? 10 : -10;
    }

    // Retourne 0 si aucun gagnant n'est trouvé
    return 0;
}

// Vérifie s'il reste des mouvements possibles sur le plateau
bool isMovesLeft(const vector<vector<char>>& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == ' ') return true;
        }
    }
    return false;
}

// Implémente l'algorithme minimax avec alpha-beta pruning
int minimax(vector<vector<char>>& board, int depth, bool isMax, int alpha, int beta) {
    int score = evaluate(board);

    // Retourne le score si le jeu est fini
    if (score == 10 || score == -10) return score;

    // Match nul si aucun mouvement n'est possible
    if (!isMovesLeft(board)) return 0;

    if (isMax) {
        int best = -1000;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    best = max(best, minimax(board, depth + 1, !isMax, alpha, beta));
                    board[i][j] = ' ';
                    alpha = max(alpha, best);
                    if (beta <= alpha) break;
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    best = min(best, minimax(board, depth + 1, !isMax, alpha, beta));
                    board[i][j] = ' ';
                    beta = min(beta, best);
                    if (beta <= alpha) break;
                }
            }
        }
        return best;
    }
}

// Trouve le meilleur mouvement pour le joueur actuel
pair<int, int> bestMove(vector<vector<char>>& board, char player) {
    int bestVal = (player == 'X') ? -1000 : 1000;
    pair<int, int> bestMove = {-1, -1};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = player;
                int moveVal = minimax(board, 0, player == 'O', -1000, 1000);
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
    ifstream file("dataset6.txt");
    string line;
    int winsForX = 0, winsForO = 0, draw = 0, cpt = 0;
    auto start = chrono::high_resolution_clock::now();
    while (getline(file, line)) {
        cpt++;
        char player = line[0];
        vector<vector<char>> board(N, vector<char>(N));
        int index = 1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
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
    cout << "Nombre total de parties gagnées par 'X' : " << winsForX << " soit " << ((float)winsForX / cpt) * 100 << "%" << endl;
    cout << "Nombre total de parties gagnées par 'O' : " << winsForO << " soit " << ((float)winsForO / cpt) * 100 << "%" << endl;
    cout << "Nombre total de parties nulles : " << draw << " soit " << ((float)draw / cpt) * 100 << "%" << endl;
    cout << "Temps écoulé : " << elapsed.count() << "s" << endl;

    return 0;
}