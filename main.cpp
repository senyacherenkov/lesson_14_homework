#include <iostream>
#include <cassert>
#include "matrix.h"

constexpr int DIAGONAL_LIMIT = 10;

int main()
{
    Matrix<int, 0> matrix; // бесконечная матрица int заполнена значениями 0

    for(int i = 0; i < DIAGONAL_LIMIT; i++) {
        matrix[i][i] = i;
    }

    int i = 0;
    int j = 9;
    for(; i < DIAGONAL_LIMIT; i++, j--) {
        matrix[i][j] = j;
    }

    assert(matrix.size() == 18);

    for(int i = 1; i < DIAGONAL_LIMIT - 1; i++) {
        for(int j = 1; j < DIAGONAL_LIMIT - 1; j++)
            std::cout << matrix[i][j] << " ";
        std::cout << std::endl;
    }

    std::cout << "Quantity of busy cells: " << matrix.size();

    for(auto c: matrix)
    {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }

    std::getchar();
    return 0;
}
