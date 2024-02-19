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

vector<vector<int>> board(WIDTH, vector<int>(HEIGHT));  // 2D array of game board

/*
 * prints the passed board and outputs grid display
 * @param b - 2D board array
 */
void printBoard(vector<vector<int>> &b) {
    system("clear");
    for (unsigned int y = 0; y < HEIGHT; y++) {
        for (unsigned int x = 0; x < WIDTH; x++) {
            switch (b[x][y]) {
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
    cout << "-------------\n1 2 3 4 5 6 7\n";
}

/*
 * finds valid height for next piece to be dropped
 * @param b - 2D board array
 * @param col - column to inspect
 * @return - integer value of the y position of the empty space
 *
 * ASSUMES POSITION IS VALID
 */
unsigned int colValidHeight(vector<vector<int>> &b, unsigned int col) {
    unsigned int h = HEIGHT - 1;  // value that starts at height
    while (b[col][h] != 0) {      // decreases the value every time the
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
void play(vector<vector<int>> &b, unsigned int col, bool t) {
    b[col][colValidHeight(b, col)] = t + 1;  // sets the found value which is empty to the players turn
}

/*
 * checks if the column of the board is full and able to recieve a new piece
 * @param b - 2D board array
 * @param col - column which is being checked
 * @return - TRUE if column is able to have another element dropped, FALSE if column is full
 */
bool colValid(vector<vector<int>> &b, int col) {
    return b[col][0] == 0;
}

/*
 * finds if the player has a winning move in the specified column
 * @param b - 2D board array
 * @param p - player which will be searched for
 * @return TRUE if player will win after piece is dropped, FALSE if not
 *
 * ASSUMES POSITION IS VALID
 */
bool winningMove(vector<vector<int>> &b, bool p, int col) {
    const unsigned int SEARCH = p + 1;                           // converted value for the searched element in the array
    const int kernel[4][2] = {{0, 1}, {1, 0}, {1, 1}, {-1, 1}};  // velocity of search around searched index
    int h = colValidHeight(b, col);                              // valid position for drop

    for (unsigned int i = 0; i < 2; i++) {                      // looping through all possible cases
        unsigned int count = 1;                                 // counter for how many are in a row (starts at one because it counts the current square)
        int index[2] = {col + kernel[i][0], h + kernel[i][1]};  // current inspected index

        if (index[0] < WIDTH && index[1] < HEIGHT) {   // if kernel isnt going to push out of bounds
            while (b[index[0]][index[1]] == SEARCH) {  // while the index in the board array is equal to the player
                index[0] = index[0] + kernel[i][0];    // move along with search
                index[1] = index[1] + kernel[i][1];
                count++;           // increase count
                if (count >= 4) {  // if there are 4 in a row
                    return true;
                }
                if (index[0] >= WIDTH || index[1] >= HEIGHT)
                    break;
            }
        }

        if (col - kernel[i][0] >= 0 && h - kernel[i][1] >= 0) {  // if kernel isnt going to push under 0
            index[0] = col - kernel[i][0];                       // move along with search in other direction
            index[1] = h - kernel[i][1];

            while (b[index[0]][index[1]] == SEARCH) {  // while the index in the other direction is equal to the player
                index[0] = index[0] - kernel[i][0];    // move along with search in other direction
                index[1] = index[1] - kernel[i][1];
                count++;           // increase count
                if (count >= 4) {  // if there are 4 in a row
                    return true;
                }
                if (index[0] < 0 || index[1] < 0)
                    break;
            }
        }
    }
    return false;  // not a winning move
}

/*
 * finds if the the player has any winning moves
 * @param b - 2D board array
 * @param p - player which will be searched for
 * @return - column with winning move, -1 if there isn't a winning move
 *
 * ASSUMES POSITION IS VALID
 */
int hasWinningMove(vector<vector<int>> &b, bool p) {
    for (unsigned int i = 0; i < WIDTH; i++) {  // loops through all columns
        if (winningMove(b, p, i)) {
            return i;
        }
    }
    return -1;  // no winning moves in position
}

/*
 * runs gameloop of plays
 */
void gameLoop() {
    printBoard(board);
    while (!gameOver) {        // while not in terminal position
        if (turn == PLAYER) {  // if the turn is the player
            int move;
            cout << "move: ";
            cin >> move;
            if (colValid(board, move - 1)) {  // checks if the column inputted is open
                if (winningMove(board, PLAYER, move - 1)) {
                    play(board, move - 1, PLAYER);
                    printBoard(board);
                    cout << "YOU WIN!";
                    break;
                }
                play(board, move - 1, PLAYER);
            } else {  // feedback
                cout << "move is invalid\n";
                continue;  // skips over this index of the loop
            }
            int cMove = rand() % WIDTH;        // random number over width, placeholder until actual algorithm
            while (!colValid(board, cMove)) {  // if the random number is valid, if not re-randomize it
                cMove = rand() % WIDTH;
            }
            if (winningMove(board, COMPUTER, cMove)) {
                play(board, cMove, COMPUTER);
                printBoard(board);
                cout << "CPU WINS!";
                break;
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