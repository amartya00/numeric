#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <numeric/types/matrix.hpp>
#include <numeric/types/models.hpp>
#include <numeric/math/rref.hpp>
#include <numeric/math/gaussjordan.hpp>
#include <numeric/math/errors.hpp>

using numeric::types::Matrix;
using numeric::functions::gauss_jordan;
using thesoup::types::Slice;
using thesoup::types::Result;
using thesoup::types::Unit;
using numeric::ErrorCode;


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

void roundOffMatrix(Matrix<double>& matrix) {
    for (auto& row : matrix) {
        for (double& elem : row) {
            double rounded = static_cast<double>(static_cast<int>(elem*100))/100.0;
            elem = rounded;
        }
    }
}

SCENARIO("Gauss Jordan elimination.") {
    
    GIVEN("I have a augmented matrix which I know can be solved.") {
        Matrix<double> testInput {
            {
                {11.0, 22.0, 17.0, 100.0},
                {0.0, 0.0, 22.0, 200.0},
                {19.0, 82.0, 67.0, 300.0}
            }  
        };
        
        WHEN("I run it through gauss jordan algorithm.") {
            
            Result<Unit, ErrorCode> result = gauss_jordan(testInput);
            roundOffMatrix(testInput);
            
            REQUIRE(result);
            REQUIRE(Unit::unit == result.unwrap());
            
            REQUIRE(4.80 == testInput[0][3]);
            REQUIRE(-4.88 == testInput[1][3]);
            REQUIRE(9.09 == testInput[2][3]);
        }
    }
    
    GIVEN("I have a matrix that I know has no solutions.") {
        Matrix<double> testInput {
            {
                {11, 22, 17, 100, 100},
                {11, 22, 99, 123, 145},
                {1,  2, 36,  45, 123},
                {2,  4, 63,  98, 1413}
            }
        };
        
        WHEN("I run it through gauss jordan algorithm.") {
            
            Result<Unit, ErrorCode> result = gauss_jordan(testInput);
            
            THEN("I should get back an error in the result.") {
                
                REQUIRE_FALSE(result);
                REQUIRE(ErrorCode::NO_SOLUTIONS == result.error());
            }
        }
    }
    
    GIVEN("I have a matrix that I know has infinite solutions.") {
        Matrix<double> testInput {
            {
                {11, 22, 17, 100, 100},
                {13, 22, 99, 123, 145},
                {11, 22, 17, 100, 100},
                {2,  4, 63,  98, 1413}
            }
        };
        
        WHEN("I run it through gauss jordan algorithm.") {
            
            Result<Unit, ErrorCode> result = gauss_jordan(testInput);
            
            THEN("I should get back an error in the result.") {
                
                REQUIRE_FALSE(result);
                REQUIRE(ErrorCode::INFINITE_SOLUTIONS == result.error());
            }
        }
    }
    
    GIVEN("I have a matrix that I know has infinite solutions.") {
        Matrix<double> testInput {
            {
                {9, 22, 17, 100, 11},
                {13, 22, 99, 123, 145},
                {9, 22, 17, 100, 11},
                {2,  4, 63,  98, 1413}
            }
        };
        
        WHEN("I run it through gauss jordan algorithm with rounding off.") {
            
            Result<Unit, ErrorCode> result = gauss_jordan(testInput, 1e-10);
            
            THEN("I should get back an error in the result.") {
                
                REQUIRE_FALSE(result);
                REQUIRE(ErrorCode::INFINITE_SOLUTIONS == result.error());
            }
        }
    }
    
    GIVEN("I have a under represented system.") {
        Matrix<double> testInput {
            {
                {11, 22, 17, 100, 100},
                {11, 22, 99, 123, 145},
                {1,  2, 36,  45, 123}
            }
        };
        
        WHEN("I run it through gauss jordan algorithm.") {
            
            Result<Unit, ErrorCode> result = gauss_jordan(testInput);
            
            THEN("I should get back an error in the result.") {
                
                REQUIRE_FALSE(result);
                REQUIRE(ErrorCode::UNDERDETERMINED_SYSTEM == result.error());
            }
        }
    }
}


