#include <vector>
using namespace std;

namespace B
{
    class Board
    {
    public:
        vector<int> position;
        void move(int pos) {
            position.push_back(pos);
        }
    };
}