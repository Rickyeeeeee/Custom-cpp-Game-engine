#include <iostream>

class base
{
public:
    base(int n = 0) : num(n) {}
    void print(int a)
    {
        std::cout << this->num << std::endl;
    }
    int num;
};

int main()
{
    base *b;
    b->print(1);


    return 0;
}

