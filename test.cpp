#include <iostream>

using namespace std;

int main()
{
    while (true)
    {
        static float frame = 0;
        cout << frame << endl;
        frame++;
    }
}