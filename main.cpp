#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

const unsigned int WIDTH = 7;      // width of the board
const unsigned int HEIGHT = 6;     // height of the board
const bool PLAYER = 0;             // players turn
const bool COMPUTER = 1;           // computers turn
const unsigned int MAX_DEPTH = 5;  // depth of the move scoring algorithm

bool gameOver = false;   // whether game has been won (or lost)
unsigned int turns = 0;  // move count of the game
bool turn = 0;           // who's turn it is

// board will be called like coordinate grid system with origin in the top left
// cell reference -> board[X VALUE][Y VALUE]
/*
    [0][0].     .     .     .     .     .
    .     .     .     .     .     .     .
    .     .     .     .     .     .     .
    .     .     .     .     .     .     .
    .     .     .     .     .     .     .
    .     .     .     .     .     .     [6][5]
*/

vector<vector<unsigned int>> board(WIDTH, vector<int>(HEIGHT));  // 2D array of game board

/*
 * prints the passed board and outputs grid display
 * @param b - 2D board array
 */
void printBoard(vector<vector<unsigned int>> &b) {
    system("clear");
    for (unsigned int y = 0; y < HEIGHT; y++) {
        for (unsigned int x = 0; x < WIDTH; x++) {
            switch (b[y][x]) {
                case 0:
                    cout << " ";
                    break;  // empty cell
                case 1:
                    cout << "x";
                    break;  // x cell (player with first move)
                case 2:
                    cout << "o";
                    break;  // o cell (player with second move)
            }
            cout << " ";
        }
        cout << endl;
    }
    cout << "1 2 3 4 5 6 7\n";
}

/*
 * finds valid height for next piece to be dropped
 * @param b - 2D board array
 * @param col - column to inspect
 * @return - integer value of the y position of the empty space
 * 
 * ASSUMES POSITION IS VALID
 */
unsigned int colValidHeight(vector<vector<unsigned int>> &b, unsigned int col) {
    unsigned int h = HEIGHT-1;  // value that starts at height
    while (b[col][h] != 0) {  // decreases the value every time the
        h--;
    }

    return h;
}

/*
 * "drops" players piece into the desired column
 * @param b - 2D board array
 * @param col - column to drop
 * @param t - the current turn
 *
 * ASSUMES POSITION IS VALID
 */
void play(vector<vector<unsigned int>> &b, unsigned int col, bool t) {
    b[col][colValidHeight(b,col)] = t + 1;  // sets the found value which is empty to the players turn
}

/*
 * checks if the column of the board is full and able to recieve a new piece
 * @param b - 2D board array
 * @param col - column which is being checked
 * @return - TRUE if column is able to have another element dropped, FALSE if column is full
 */
bool colValid(vector<vector<unsigned int>> &b, int col) {
    return b[col][0] == 0;
}


/*
 * runs gameloop of plays
 */
void gameLoop() {
    printBoard(board);
    while (!gameOver) {  // while not in terminal position
        if (turn == PLAYER) {  // if the turn is the player
            int move;
            cout << "move: ";
            cin >> move;
            if (colValid(board, move-1))  // checks if the column inputted is open
                play(board, move - 1, PLAYER);
            else {  // feedback
                cout << "move is invalid\n";
                continue;  // skips over this index of the loop
            }
            int cMove = rand()%WIDTH;  // random number over width, placeholder until actual algorithm
            while(!colValid(board, cMove)) {  // if the random number is valid, if not re-randomize it
                cMove = rand()%WIDTH;
            }
            play(board, cMove, COMPUTER);

            printBoard(board);
        }
    }
}

/*
 * main driver
 */
int main() {
    gameLoop();
    return 0;
}