#include <iostream>
#include "./board.h"

using namespace B;
using namespace std;

int main()
{
    Board board;
    for (int i = 0; i < 100; i++)
    {
        int move;
        cin >> move;
        board.move(move);
        for (int a = 6; a >= 0; a--)
        {
            for (int b = 0; b < 7; b++)
            {
                cout << ((board.mask[(7 * b) + a] + board.position[(7 * b) + a])==2 ? "x" : ((board.mask[(7 * b) + a] + board.position[(7 * b) + a])==1 ? "o" : "."));
            }
            cout << endl;
        }
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    }

    return 0;
}