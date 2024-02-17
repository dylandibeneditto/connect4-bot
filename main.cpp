#include <iostream>

#include "./board.h"

using namespace B;
using namespace std;

void visualize(Board board) {
    for (int a = board.HEIGHT; a >= 0; a--) {
        for (int b = 0; b < board.WIDTH; b++) {
            cout << ((board.mask[(board.WIDTH * b) + a] + board.position[(board.WIDTH * b) + a]) == 2 ? " \033[1;33m█\033[0m" : ((board.mask[(board.WIDTH * b) + a] + board.position[(board.WIDTH * b) + a]) == 1 ? " \033[1;31m█\033[0m" : "  "));
        }
        cout << endl;
        cout << endl;
    }
    cout << " 1 2 3 4 5 6 7\n";
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

int main() {
    Board board;
    int move;
    for (int i = 0; i < 100; i++) {
        visualize(board);
        cin >> move;
        if (board.canMove(move - 1)) {
            cout << "DEBUG";
            if (!board.isWinningMove(move - 1)) {
                board.move(move - 1);
                board.move(board.bestMove(board));
            } else {
                board.move(move - 1);
                visualize(board);
                cout << "You win!";
                return 0;
            }
        } else {
            cout << endl;
        }
    }

    return 0;
}