#include <iostream>

#include "./board.h"

using namespace B;
using namespace std;

int main() {
    Board board;
    int move;
    for (int i = 0; i < 100; i++) {
        for (int a = 6; a >= 0; a--) {
            for (int b = 0; b < 7; b++) {
                cout << ((board.mask[(7 * b) + a] + board.position[(7 * b) + a]) == 2 ? "x" : ((board.mask[(7 * b) + a] + board.position[(7 * b) + a]) == 1 ? "o" : " "));
            }
            cout << endl;
        }
        cout << "1234567\n";
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        cin >> move;
        if (board.canMove(move - 1) == 1) {
            if (board.isWinningMove(move - 1) == 0) {
                board.move(move - 1);
                board.move(board.bestMove());
            } else {
                cout<<"\nYou win!";
                return 0;
            }
        } else {
            cout << endl;
        }
    }

    return 0;
}