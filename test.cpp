#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Base
{
public:
    inline Base()
    {
        cout << "Base Constructor Called!" << endl;
    }
    inline Base(const Base& rhs)
        : name(rhs.name)
    {
        cout << "Base Move Constructor Called" << endl;
    }
    inline ~Base()
    {
        cout << "Base Destructor Called!" << endl;
    }

    Base operator+ (const Base rhs)
    {
        Base r;
        r.name = name + rhs.name;
        return r;
    }
private:
    string name;
};

class Derived : public Base
{
public:
    Derived()
    {
        cout << "Derived Constructor Called!" << endl;
    }
    ~Derived()
    {
        cout << "Derived Destructor Called!" << endl;
    }
private:
    string id;
};

int main()
{
    Base a;
    Base b;
    Base c;
    Base d;
    d = a + b + c;
}