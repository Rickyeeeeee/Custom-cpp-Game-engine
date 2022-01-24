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

class bool_struct
{
public:
    char flag1;
};

int main()
{
    std::cout << sizeof(bool_struct) << std::endl;
    return 0;
}

