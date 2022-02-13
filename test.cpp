#include <vector>
#include <iostream>

using namespace std;

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
    vector<Vertex> vertices;
    vertices.reserve(4);
    Vector v = { 1.0f, 0.0f, 0.5f };
    vertices.emplace_back(v, v);
    vertices.emplace_back(v, v);
    vertices.emplace_back(v, v);
    vertices.emplace_back(v, v);

    auto ptr = (float*)vertices.begin().base();

    cout << ptr[0] << ", " <<  ptr[1] << endl;
}