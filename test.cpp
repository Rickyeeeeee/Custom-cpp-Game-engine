#include <vector>
#include <string>
#include <iostream>

using namespace std;

int main()
{
    vector<string> string_stack;
    unsigned int index = 0;
    string_stack.emplace(string_stack.begin(), "pushlayer 1");
    index++;
    string_stack.emplace_back("pushOVERlayer 1");
    string_stack.emplace(string_stack.begin() + index, "pushlayer 2");
    index++;
    string_stack.emplace_back("pushOVERlayer 2");
    for ( auto& string : string_stack)
    {
        cout << string << endl;       
    }
}