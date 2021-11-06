
#include <algorithm>
#include <functional>

#include <iostream>
#include <vector>
#include <numeric>

int main()
{
    std::vector<int> v(100), v1;

    std::iota(v.begin(), v.end(), 1);
    std::transform(v.begin(), v.end(), v.begin(),
    [] (int i) { return i * i; }
    );

    std::copy_if(v.begin(), v.end(), std::back_inserter(v1), 
    [] (int i) { return i % 4 == 0;}
    );

    for (int i : v1)
        std::cout << i << " ";
    std::cout << std::endl;
}
