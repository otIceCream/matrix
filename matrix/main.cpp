#include <iostream>
#include "matrix.h"

int main()
{
    matrix<int> u = { {2,1},
                {3,4},
                { 5, 6 }};
    matrix<int> i = { {2,1,5},
                {3,4,7} };
    matrix<int> l = i * u;
    matrix<double> o(10, 10);
    for (int i = 0; i< o.rows(); ++i) {
        for (int j = 0; j < o.cols(); ++j) {
            o.at(i, j) = (i+j) * -1,1;
            std::cout << o << "\n\n";
        } 
    }
    
    std::cout << u;

}