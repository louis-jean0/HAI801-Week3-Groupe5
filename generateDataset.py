from typing import List, Union
from io import TextIOWrapper
import numpy as np


class TicTacToeBoard:
    # def __init__(self, board=None, turn=1):
    #     if board is None:
    #         self.board = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
    #     else:
    #         self.board = board
    #     self.turn = turn

    # def make_move(self, x: int, y: int):
    #     new_board = [row[:] for row in self.board]
    #     new_board[x][y] = self.turn
    #     return TicTacToeBoard(new_board, 1 if self.turn == 2 else 2)
    def __init__(self, n=4, board=None, turn=1):
            self.n = n
            if board is None:
                self.board = [[0 for _ in range(n)] for _ in range(n)]
            else:
                self.board = board
            self.turn = turn

    def make_move(self, x: int, y: int):
        new_board = [row[:] for row in self.board]
        new_board[x][y] = self.turn
        return TicTacToeBoard(self.n, new_board, 1 if self.turn == 2 else 2)

    def printToFile(self, file: TextIOWrapper):
        file.write(str(self) + '\n')

    def __str__(self):
        s = f'{"X" if self.turn == 1 else "O"}'
        for row in self.board:
            for cell in row:
                s += f'{"X" if cell == 1 else "O" if cell == 2 else " "}'
        return s

    def getChildren(self) -> List['TicTacToeBoard']:
        moves = []
        for i in range(self.n):
            for j in range(self.n):
                if self.board[i][j] == 0:
                    moves.append(self.make_move(i, j))
        return moves

    def isFinal(self) -> bool:
        # test if board is full
        if np.array(self.board).flatten().tolist().count(0) == 0:
            return True

        # test if there is a winner
        arr = np.array(self.board, dtype=int)
        for i in range(self.n):
            if arr[i].tolist().count(1) == self.n or arr[:, i].tolist().count(1) == self.n:
                return True
            if arr[i].tolist().count(2) == self.n or arr[:, i].tolist().count(2) == self.n:
                return True

        if arr.diagonal().tolist().count(1) == self.n or np.fliplr(arr).diagonal().tolist().count(1) == self.n:
            return True
        if arr.diagonal().tolist().count(2) == self.n or np.fliplr(arr).diagonal().tolist().count(2) == self.n:
            return True

        return False

    def __eq__(self, other: 'TicTacToeBoard'):
        return self.board == other.board and self.turn == other.turn

    def __hash__(self):
        return hash(str(self))


def generateDataset(board: TicTacToeBoard, s: set[TicTacToeBoard]):
    if board.isFinal():
        return
    for child in board.getChildren():
        if child not in s:
            s.add(child)
            generateDataset(child, s)



with open('dataset.txt', 'w') as file:
    s = set()
    generateDataset(TicTacToeBoard(), s)
    for item in s:
        item.printToFile(file)
