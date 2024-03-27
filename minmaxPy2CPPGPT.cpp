#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

// Évalue l'état actuel du plateau de jeu pour déterminer un score
int evaluate(const vector<vector<char>>& board) {
    // Vérifie chaque ligne pour une victoire
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == 'X') return 10;
            else if (board[row][0] == 'O') return -10;
        }
    }

    // Vérifie chaque colonne pour une victoire
    for (int col = 0; col < 3; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == 'X') return 10;
            else if (board[0][col] == 'O') return -10;
        }
    }
    
    // Vérifie les deux diagonales pour une victoire
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == 'X') return 10;
        else if (board[0][0] == 'O') return -10;
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == 'X') return 10;
        else if (board[0][2] == 'O') return -10;
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

// Implémente l'algorithme minimax pour trouver le meilleur score possible
int minimax(vector<vector<char>>& board, int depth, bool isMax) {
    int score = evaluate(board);

    // Retourne le score si le jeu est fini
    if (score == 10 || score == -10) return score;

    // Match nul si aucun mouvement n'est possible
    if (!isMovesLeft(board)) return 0;

    // Maximiseur
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
    }
    // Minimiseur
    else {
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

// Trouve le meilleur mouvement pour le joueur actuel
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

// Simule un jeu jusqu'à son terme et retourne le résultat
int simulateGame(vector<vector<char>>& board, char player) {
    while (isMovesLeft(board) && evaluate(board) == 0) {
        auto move = bestMove(board, player);
        if (move.first != -1) {
            board[move.first][move.second] = player;
            // Change le tour au prochain joueur
            player = (player == 'X') ? 'O' : 'X';
        } else {
            // Aucun mouvement n'est possible, termine la boucle
            break;
        }
    }
    // Évalue le plateau final pour déterminer le résultat du jeu
    return evaluate(board);
}

int main() {
    ifstream file("dataset.txt"); // Ouvre le fichier de dataset pour lecture
    string line;  // Pour stocker chaque ligne lue du fichier
    int winsForX = 0, winsForO = 0, draw = 0, cpt = 0; // Compteurs pour les résultats
    auto start = chrono::high_resolution_clock::now();  // Démarre le chronomètre pour le temps d'exécution

    // Lit chaque ligne du fichier jusqu'à la fin
    while (getline(file, line)) {
        cpt++; // Incrémente le compteur de jeux
        char player = line[0]; // Le premier caractère indique le joueur actuel
        vector<vector<char>> board(3, vector<char>(3)); // Crée un plateau n*n
        int index = 1;
        // Remplit le plateau de jeu à partir des caractères de la ligne
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = line[index++];
            }
        }

        int result = simulateGame(board, player); // Simule le jeu et récupère le résultat
        // Incrémente les compteurs en fonction du résultat
        if (result == 10) winsForX++;
        else if (result == -10) winsForO++;
        else draw++;
    }

    auto end = chrono::high_resolution_clock::now(); // Arrête le chronomètre
    chrono::duration<double> elapsed = end - start; // Calcule le temps écoulé
    // Affiche les résultats et le temps d'exécution
    cout << "Nombre total de parties gagnées par 'X' : " << winsForX <<" soit "<<((float)winsForX/cpt)*100<<"%"<<endl;
    cout << "Nombre total de parties gagnées par 'O' : " << winsForO <<" soit "<<((float)winsForO/cpt)*100<<"%"<<endl;
    cout << "Nombre total de parties nulles : " << draw <<" soit "<<((float)draw/cpt)*100<<"%"<<endl;
    cout << "Temps écoulé : " << elapsed.count() << "s" << endl;

    return 0;
}