#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

unsigned int WIDTH = 7; // width of the board
unsigned int HEIGHT = 6; // height of the board
bool PLAYER = 0; // players turn
bool COMPUTER = 1; // computers turn
unsigned int MAX_DEPTH = 5; // depth of the move scoring algorithm

bool gameOver = false; // whether game has been won (or lost)
unsigned int turns = 0; // move count of the game
bool turn = 0; // who's turn it is

// board will be called like coordinate grid system with origin in the top left
// cell reference -> board[Y VALUE][X VALUE]
/*
    [0][0].     .     .     .     .     .     
    .     .     .     .     .     .     .     
    .     .     .     .     .     .     .     
    .     .     .     .     .     .     .     
    .     .     .     .     .     .     .     
    .     .     .     .     .     .     [5][6]
*/

vector<vector<int>> board(HEIGHT, vector<int>(WIDTH)); // 2D array of game board

void printBoard(vector<vector<int>> &b) {
    system("clear");
    b[0][1] = 1;
    for(int x = 0; x < WIDTH; x++) {
        for(int y = 0; y < HEIGHT; y++) {
            switch(b[x][y]) {
                case 0: cout<<" "; break; // empty cell
                case 1: cout<<"x"; break; // x cell (player with first move)
                case 2: cout<<"o"; break; // o cell (player with second move)
            }
            cout<<" ";
        }
        cout<<endl;
    }
}

int main() {
    printBoard(board);
    return 0;
}