#include <iostream>
#include <vector>
using namespace std;

namespace B {
class Board {
   public:
    static const int WIDTH = 7;
    static const int HEIGHT = 6;
    bool position[(HEIGHT + 1) * WIDTH] = {0};
    bool mask[(HEIGHT + 1) * WIDTH] = {0};
    // 0 - yellow, 1 - red
    bool turn = 0;
    bool canMove(int pos) { return mask[(WIDTH * pos) + HEIGHT] == 0 ? 1 : 0; }

    void move(int pos) {
        if (canMove(pos) == 1) {
            int h = 0;
            while (mask[(WIDTH * pos) + h] == 1) {
                h++;
            }
            mask[(WIDTH * pos) + h] = 1;

            if (turn == 0) {  // if turn is yellow
                position[(WIDTH * pos) + h] = 1;
            }

            turn = !turn;
            cout << (turn == 0 ? "yellow's turn\n" : "red's turn\n");
        } else {
            cout << "invalid move!\n";
        }
    }

    bool isWinningMove(int col) {}
};
}  // namespace B