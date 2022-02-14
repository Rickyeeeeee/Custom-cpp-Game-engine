#include <vector>
#include <iostream>
#include <utility>


struct Vector
{
    float a;
    float b;
    float c;
};

struct Vertex
{
    Vector x;
    Vector y;
    Vertex(Vector x, Vector y)
        : x(x), y(y) {}
};
int main()
{
}