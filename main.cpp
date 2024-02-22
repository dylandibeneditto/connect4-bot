#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <vector>

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

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
 * copies board state
 */
vector<vector<int>> copyBoard(vector<vector<int>> b) {
    vector<vector<int>> b2(WIDTH, vector<int>(HEIGHT));
    for (unsigned int y = 0; y < HEIGHT; y++) {
        for (unsigned int x = 0; x < WIDTH; x++) {
            b2[x][y] = b[x][y];
        }
    }
    return b2;
}

/*
 * scores 2 in a row and three and a row potential
 * @param g - good points
 * @param b - bad points
 * @param z - empty points
 * @return - the score
 */
int heurFunction(unsigned int g, unsigned int b, unsigned int z) {
	int score = 0;
	if (g == 4) { score += 500001; } // preference to go for winning move vs. block
	else if (g == 3 && z == 1) { score += 5000; }
	else if (g == 2 && z == 2) { score += 500; }
	else if (b == 2 && z == 2) { score -= 501; } // preference to block
	else if (b == 3 && z == 1) { score -= 5001; } // preference to block
	else if (b == 4) { score -= 500000; }
	return score;
}

/*
 * function to find the score of a set of 4 spots
 * @param v - the row/column to check
 * @param p - the player to check against
 * @return - the score of the row/column
 */
int scoreSet(vector<unsigned int> v, unsigned int p) {
    unsigned int good = 0;                         // points in favor of p
    unsigned int bad = 0;                          // points against p
    unsigned int empty = 0;                        // neutral spots
    for (unsigned int i = 0; i < v.size(); i++) {  // just enumerate how many of each
        good += (v[i] == p) ? 1 : 0;
        bad += (v[i] == PLAYER || v[i] == COMPUTER) ? 1 : 0;
        empty += (v[i] == 0) ? 1 : 0;
    }
    bad -= good;
    return heurFunction(good, bad, empty);
}

/*
 * function to tabulate current board "value"
 * @param b - the board to evaluate
 * @param p - the player to check score of
 * @return - the board score
 */
int tabScore(vector<vector<int>> b, unsigned int p) {
    int score = 0;
    vector<unsigned int> rs(WIDTH);
    vector<unsigned int> cs(HEIGHT);
    vector<unsigned int> set(4);
    /**
     * horizontal checks, we're looking for sequences of 4
     * containing any combination of AI, PLAYER, and empty pieces
     */
    for (unsigned int y = 0; y < HEIGHT; y++) {
        for (unsigned int x = 0; x < WIDTH; x++) {
            rs[y] = b[x][y];  // this is a distinct row alone
        }
        for (unsigned int y = 0; y < HEIGHT - 3; y++) {
            for (int i = 0; i < 4; i++) {
                set[i] = rs[y + i];  // for each possible "set" of 4 spots from that row
            }
            score += scoreSet(set, p);  // find score
        }
    }
    // vertical
    for (unsigned int x = 0; x < WIDTH; x++) {
        for (unsigned int y = 0; y < HEIGHT; y++) {
            cs[y] = b[y][x];
        }
        for (unsigned int y = 0; y < HEIGHT - 3; y++) {
            for (int i = 0; i < 4; i++) {
                set[i] = cs[y + i];
            }
            score += scoreSet(set, p);
        }
    }
    // diagonals
    for (unsigned int y = 0; y < WIDTH - 3; y++) {
        for (unsigned int x = 0; x < HEIGHT; x++) {
            rs[x] = b[y][x];
        }
        for (unsigned int x = 0; x < WIDTH - 3; x++) {
            for (int i = 0; i < 4; i++) {
                set[i] = b[y + i][x + i];
            }
            score += scoreSet(set, p);
        }
    }
    for (unsigned int x = 0; x < HEIGHT - 3; x++) {
        for (unsigned int y = 0; y < WIDTH; y++) {
            rs[x] = b[x][y];
        }
        for (unsigned int y = 0; y < HEIGHT - 3; y++) {
            for (int i = 0; i < 4; i++) {
                set[i] = b[y + 3 - i][x + i];
            }
            score += scoreSet(set, p);
        }
    }
    return score;
}

/*
 * minimax implementation
 * @param b - the board to perform MM on
 * @param d - the current depth
 * @param alf - alpha
 * @param bet - beta
 * @param p - current player
 */
array<int, 2> miniMax(vector<vector<int>> &b, unsigned int d, int alf, int bet, unsigned int p) {
    /*
     * if we've reached minimal depth allowed by the program
     * we need to stop, so force it to return current values
     * since a move will never (theoretically) get this deep,
     * the column doesn't matter (-1) but we're more interested
     * in the score
     *
     * as well, we need to take into consideration how many moves
     * ie when the board is full
     */
    if (d == 0 || d >= (WIDTH * HEIGHT) - turns) {
        // get current score to return
        return array<int, 2>{tabScore(b, COMPUTER), -1};
    }
    if (p == COMPUTER) {                                                    // if AI player
        array<int, 2> moveSoFar = {INT_MIN, -1};                            // since maximizing, we want lowest possible value
        if (hasWinningMove(b, PLAYER) != -1) {                              // if player about to win
            return moveSoFar;                                               // force it to say it's worst possible score, so it knows to avoid move
        }                                                                   // otherwise, business as usual
        for (unsigned int c = 0; c < WIDTH; c++) {                          // for each possible move
            if (b[HEIGHT - 1][c] == 0) {                                    // but only if that column is non-full
                vector<vector<int>> newBoard = copyBoard(b);                // make a copy of the board
                play(newBoard, c, p);                                       // try the move
                int score = miniMax(newBoard, d - 1, alf, bet, PLAYER)[0];  // find move based on that new board state
                if (score > moveSoFar[0]) {                                 // if better score, replace it, and consider that best move (for now)
                    moveSoFar = {score, (int)c};
                }
                alf = max(alf, moveSoFar[0]);
                if (alf >= bet) {
                    break;
                }  // for pruning
            }
        }
        return moveSoFar;  // return best possible move
    } else {
        array<int, 2> moveSoFar = {INT_MAX, -1};  // since PLAYER is minimized, we want moves that diminish this score
        if (hasWinningMove(b, COMPUTER)) {
            return moveSoFar;  // if about to win, report that move as best
        }
        for (unsigned int c = 0; c < WIDTH; c++) {
            if (b[HEIGHT - 1][c] == 0) {
                vector<vector<int>> newBoard = copyBoard(b);
                play(newBoard, c, p);
                int score = miniMax(newBoard, d - 1, alf, bet, COMPUTER)[0];
                if (score < moveSoFar[0]) {
                    moveSoFar = {score, (int)c};
                }
                bet = min(bet, moveSoFar[0]);
                if (alf >= bet) {
                    break;
                }
            }
        }
        return moveSoFar;
    }
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
            int cMove = miniMax(board, MAX_DEPTH, 0 - INT_MAX, INT_MAX, COMPUTER)[1];
            if (winningMove(board, COMPUTER, cMove)) {
                play(board, cMove, COMPUTER);
                printBoard(board);
                cout << "CPU WINS!";
                break;
            }
            play(board, cMove, COMPUTER);

            printBoard(board);
            cout << "winning move: " << hasWinningMove(board, PLAYER) + 1 << "\n";
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