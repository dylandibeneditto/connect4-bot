#include <iostream>
using namespace std;

namespace B {
class Board {
   public:
    static const int WIDTH = 4;
    static const int HEIGHT = 3;

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

    mutable bool position[(HEIGHT + 1) * WIDTH] = {0};
    mutable bool mask[(HEIGHT + 1) * WIDTH] = {0};
    // 0 - x, 1 - o
    bool turn = 0;
    int moves = 0;
    bool canMove(int pos) const { return position[(WIDTH * pos) + HEIGHT] == 0 ? true : false; }

    void move(int pos) {
        if (canMove(pos) == 1) {
            int h = 0;
            while (position[(WIDTH * pos) + h] == 1) {
                h++;
            }
            position[(WIDTH * pos) + h] = 1;

            if (turn == 0) {  // if turn is x
                mask[(WIDTH * pos) + h] = 1;
            }

            turn = !turn;
            moves++;
        } else {
            cout << "invalid move!\n";
        }
    }

    /* FIX THE WHILE LOOPS CHECKING RESERVED TOP ROW */
    bool isWinningMove(int col) const {
        int h = 0;
        while (position[(WIDTH * col) + h] == 1) {
            h++;
        }
        int index = (WIDTH * col) + h;
        static const int kernel[4] = {6, 1, 7, 8};
        for (int i = 0; i < 4; i++) {
            int count = 0;
            int val = index;
            if (turn == 0) {  // if turn will be x
                while (mask[val + kernel[i]] == 1) {
                    if(val+kernel[i]>WIDTH*(HEIGHT+1))
                        break;
                    if(val)
                    val += kernel[i];
                    count++;
                }
                val = index;
                while (mask[val - kernel[i]] == 1) {
                    if(val-kernel[i]<0)
                        break;
                    val -= kernel[i];
                    count++;
                }
            } else {  // if turn will be o
                while (position[val + kernel[i]] - mask[val + kernel[i]] == 1) {
                    if(val+kernel[i]>WIDTH*(HEIGHT+1))
                        break;
                    val += kernel[i];
                    count++;
                }
                val = index;
                while (position[val + kernel[i]] - mask[val - kernel[i]] == 1) {
                    if(val-kernel[i]<0)
                        break;
                    val -= kernel[i];
                    count++;
                }
            }
            if (count >= 3) {  // count will be three in a row as the kernel center is never factored
                cout << kernel[i];
                return true;
            }
        }

        return false;
    }

    int nmax(const Board& P, int alpha, int beta) {
        if (P.moves == WIDTH * HEIGHT) // draw
            return 0;

        for (int x = 0; x < WIDTH; x++) { // win
            cout << P.moves;
            if (P.isWinningMove(x))
                return (WIDTH * HEIGHT + (1 - P.moves)) / 2;
        }

        int max = (WIDTH * HEIGHT - 1 - P.moves) / 2; // cant win immediately
        if (beta > max) {
            beta = max;                      
            if (alpha >= beta) return beta;  
        }

        for (int x = 0; x < WIDTH; x++) // compute all possible moves
            if (P.canMove(x)) {
                Board B2(P.position, P.mask, P.moves, P.turn);
                B2.move(x);
                int score = -nmax(B2, -beta, -alpha);

                if (score >= beta) return score;
                if (score > alpha) alpha = score;                                                   
            }
        cout << endl;
        return alpha;
    }

    bool canWinNext() const {
        for (int i = 0; i < WIDTH; i++) {
            if (isWinningMove(i)) return true;
        }
        return false;
    }

    int bestMove(Board& P) {
        if (P.canWinNext())
            return (WIDTH * HEIGHT + 1 - P.moves) / 2;
        int min = -(WIDTH * HEIGHT - P.moves) / 2;
        int max = (WIDTH * HEIGHT + 1 - P.moves) / 2;

        while (min < max) {
            int med = min + (max - min) / 2;
            if (med <= 0 && min / 2 < med)
                med = min / 2;
            else if (med >= 0 && max / 2 > med)
                med = max / 2;
            int r = nmax(P, med, med + 1);
            if (r <= med)
                max = r;
            else
                min = r;
        }
        return min;
    }

    Board(bool position[25] = {0}, bool mask[25] = {0}, int moves = 0, bool turn = 0) {
        for (int i = 0; i < 25; ++i) {
            this->position[i] = position[i];
            this->mask[i] = mask[i];
        }
        this->moves = moves;
        this->turn = turn;
    }
};
}  // namespace B