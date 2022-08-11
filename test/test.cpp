#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <vector>
#include <iostream>
// #include <chrono>

using namespace std;
using namespace glm;

class String;

ostream& operator << (ostream& os, String strn);

class String
{
public:
    // factory
    template <typename T>
    static String Create(T&& str)
    {
        return String(std::forward<T>(str));
    }
    // template <int L>
    String(const char* rhs)
    {
        size = strlen(rhs);
        str = new char[size];
        strcpy(str, rhs);

    }
    String(const String& rhs)
    {
        size = rhs.size;
        str = new char[size];
        strcpy(str, rhs.str);
        cout << "Copy Constructor" << endl;
    }
    String(String&& rhs)
    {
        size = rhs.size;
        str = rhs.str;
        cout << "Move Constructor" << endl;
    }

    String operator+(const String& rhs)
    {
        auto newStr = new char[size + rhs.size];
        strncpy(newStr, str, size);
        strncpy(newStr + size, rhs.str, rhs.size);
        return String(newStr);
    }
private:
    uint32_t size = 0;
    char* str = nullptr;
friend ostream& operator << (ostream& os, String strn);
};

ostream& operator << (ostream& os, String strn)
{
    for (int i = 0; i < strn.size; i++)
        os << strn.str[i];
    return os;
}

struct Logger
{
    void operator() (){
        cout << "Logging...";
    }
};

void Log()
{
    cout << endl;
}

template <typename T, typename ...Args>
void Log(T&& arg, Args&&... args)
{
    cout << arg << " ";
    Log(args...);
}



int main()
{
    cout << endl;
    auto str = "apple";
    String mystring("hello ");
    String mystring2("world");

    auto newStr2 = String::Create(mystring + mystring2);
    
    // cout << newStr2 << endl;
    vector<int> a;
    a.reserve(8);
    a.emplace_back(4);
    a.push_back(3);
    for (const auto& ele : a)
           cout << ele << endl;
}