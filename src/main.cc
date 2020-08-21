#include <iostream>
#include <exception>
#include <type_traits>
#include <vector>

#include <math/matrix.hpp>


using Sigabrt::Numeric::Matrix;

int main() {
    const std::vector<std::vector<int>> matrix {
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };
    const Matrix<int> myMatrix(matrix);
    
    std::cout << myMatrix[0][0] << "\n";
}
