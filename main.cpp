#include <iostream>

#include "./board.h"

using namespace B;
using namespace std;

void visualize(Board board) {
            for (int a = 6; a >= 0; a--) {
            for (int b = 0; b < 7; b++) {
                cout << ((board.mask[(7 * b) + a] + board.position[(7 * b) + a]) == 2 ? " \033[1;43m \033[0m" : ((board.mask[(7 * b) + a] + board.position[(7 * b) + a]) == 1 ? " \033[1;41m \033[0m" : "  "));
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
        if (board.canMove(move - 1) == 1) {
            if (board.isWinningMove(move - 1) == 0) {
                board.move(move - 1);
                board.move(board.bestMove());
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