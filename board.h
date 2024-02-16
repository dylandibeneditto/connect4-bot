#include <cstdlib>
#include <iostream>
using namespace std;

namespace B {
class Board {
   public:
    static const int WIDTH = 7;
    static const int HEIGHT = 6;

    /* BITMAP REFERENCE MAP */
    /*
        .  .  .  .  .  .  .
        5 12 19 26 33 40 47
        4 11 18 25 32 39 46
        3 10 17 24 31 38 45
        2  9 16 23 30 37 44
        1  8 15 22 29 36 43
        0  7 14 21 28 35 42
    */

    /* EXAMPLE */
    /*            mask      position
                  0000000   0000000
        .......   0000000   0000000
        ...o...   0000000   0001000
        ..xx...   0011000   0011000
        ..ox...   0001000   0011000
        ..oox..   0000100   0011100
        ..oxxo.   0001100   0011110
    */

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
            cout << (turn == 0 ? "x's turn\n" : "o's turn\n");
        } else {
            cout << "invalid move!\n";
        }
    }

    bool isWinningMove(int col) {
        int h = 0;
        while (position[(WIDTH * col) + h] == 1) {
            h++;
        }
        int index = (WIDTH * col) + h;
        static const int kernel[4] = {6, 1, 7, 8};
        for (int i = 0; i < 4; i++) {
            int count = 0;
            int val = index;
            while (mask[val + kernel[i]] == 1) {
                val += kernel[i];
                count++;
            }
            val = index;
            while (mask[val - kernel[i]] == 1) {
                val -= kernel[i];
                count++;
            }
            if (count >= 4) {
                return 1;
            }
        }

        return 0;
    }

    int bestMove() {
        int r = 0;
        while (r > WIDTH == 0) {
            if (isWinningMove(r) == 1) {
                return r;
            } else {
                r++;
            }
        }
        return rand()%WIDTH;
    }
};
}  // namespace B