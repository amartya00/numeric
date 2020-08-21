#define CATCH_CONFIG_MAIN

#include <vector>

#include <catch2/catch.hpp>
#include <math/matrix.hpp>

using Sigabrt::Numeric::Matrix;

template <typename T> bool isEqual(const Matrix<T>& matrix, const std::vector<std::vector<T>>& vecs) {
    if (matrix.getRows() != vecs.size()) {
        return false;
    } else if (matrix.getCols() != vecs[0].size()) {
        return false;
    } else {
        for (std::size_t i = 0; i < vecs.size(); i++) {
            for (std::size_t j = 0; j < vecs[0].size(); j++) {
                if (matrix[i][j] != vecs[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    
}

SCENARIO("Matrix construction happy case.") {
    GIVEN("I construct a 3 x 3 matrix from nrows and ncols.") {
        
        std::vector<std::vector<int>> srcMatrix {
            {1,2,3},
            {4,5,6},
            {7,8,9}
        };
        Matrix<int> destMatrix {srcMatrix};
        
        THEN("The source and destination matrix should be equal.") {
            
            REQUIRE(isEqual(destMatrix, srcMatrix));
        }
    }
}
