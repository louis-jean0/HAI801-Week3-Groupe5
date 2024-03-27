import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashSet;
import java.util.Set;

public class TicTacToeNDGenerator {
    public static void main(String[] args) {
        int dimension = 4; // Dimension du morpion
        generateDatasetToFile(dimension, "dataset.txt");
    }

    public static void generateDatasetToFile(int dimension, String filename) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename))) {
            char[][] board = new char[dimension][dimension];
            for (int i = 0; i < dimension; i++) {
                for (int j = 0; j < dimension; j++) {
                    board[i][j] = ' '; // Initialiser le tableau avec des espaces vides
                }
            }
            Set<String> visitedStates = new HashSet<>();
            generateStates(board, 'X', visitedStates, writer);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void generateStates(char[][] board, char currentPlayer, Set<String> visitedStates, BufferedWriter writer) throws IOException {
        String currentState = getState(board);
        if (visitedStates.contains(currentState)) {
            return; // État déjà visité, arrêter la génération
        }
        visitedStates.add(currentState); // Ajouter l'état actuel à l'ensemble des états visités
        writer.write(currentState);
        writer.newLine();

        for (int i = 0; i < board.length; i++) {
            for (int j = 0; j < board[i].length; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = currentPlayer; // Jouer le coup pour le joueur actuel
                    char nextPlayer = (currentPlayer == 'X') ? 'O' : 'X'; // Changer de joueur
                    generateStates(board, nextPlayer, visitedStates, writer); // Générer les états à partir de ce coup
                    board[i][j] = ' '; // Annuler le coup pour explorer les autres possibilités
                }
            }
        }
    }

    private static String getState(char[][] board) {
        StringBuilder sb = new StringBuilder();
        for (char[] row : board) {
            for (char cell : row) {
                sb.append(cell);
            }
        }
        return sb.toString();
    }
}