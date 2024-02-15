#include <iostream>
#include "./board.h"

using namespace B;
using namespace std;

int main()
{
    Board board;
    int move;
    for (int i = 0; i < 100; i++)
    {
        for (int a = 6; a >= 0; a--)
        {
            for (int b = 0; b < 7; b++)
            {
                cout << ((board.mask[(7 * b) + a] + board.position[(7 * b) + a]) == 2 ? "x" : ((board.mask[(7 * b) + a] + board.position[(7 * b) + a]) == 1 ? "o" : "."));
            }
            cout << endl;
        }
        cout << "0123456\n";
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        cin >> move;
        board.move(move);
    }

    return 0;
}