#include <vector>
#include <iostream>
using namespace std;

namespace B
{
    class Board
    {
    public:
        bool position[49] = {0};
        bool mask[49] = {0};
        //0 - yellow, 1 - red
        bool turn = 0;
        bool canMove(int pos) {
            return 1;
        }

        void move(int pos) {
            if(canMove(pos)==1) {
                int h = 0;
                while(mask[(7*pos)+h]==1) {
                    h++;
                }
                mask[(7*pos)+h] = 1;

                if(turn==0) { // if turn is yellow
                    position[(7*pos)+h] = 1;
                }

                turn=!turn;
            }
        }
    };
}