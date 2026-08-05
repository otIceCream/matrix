#include <iostream>
#include "matrix.h"

int main()
{
    matrix<int> a;
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    matrix<int> u = { {2,1},
                {3,4},
                { 5, 6 }};
    matrix<int> i = { {2,1,5},
                {3,4,7} };
    matrix<int> l = i * u;
    matrix<double> o(10, 10);
    o.fill(1);
    o.insert_row(2);
    o.insert_col(2);
    o.resize(3, 3);
    std::cout << o << "\n\n";
    
    std::cout << u;

}