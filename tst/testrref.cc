#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <numeric/types/matrix.hpp>
#include <numeric/types/models.hpp>
#include <numeric/math/rref.hpp>
#include <numeric/math/errors.hpp>

using Sigabrt::Types::Matrix;
using Sigabrt::Numeric::rref;
using Sigabrt::Types::Slice;
using Sigabrt::Types::Result;
using Sigabrt::Types::Unit;
using Sigabrt::Numeric::ErrorCode;
using Sigabrt::Types::OperationType;

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

SCENARIO("RREF algorithm.") {
    
    GIVEN("I have 'wide' matrix.") {
        Matrix<double> testInput {
            {
                {11.0, 22.0, 17.0, 100.0},
                {0.0, 0.0, 22.0, 200.0},
                {19.0, 82.0, 67.0, 300.0}
            }  
        };
        
        WHEN("I run it through gauss rref algorithm.") {
            
            Result<Unit, ErrorCode> result = rref(testInput);
            roundOffMatrix(testInput);
            
            THEN("The result should be as expected."){
                
                REQUIRE(OperationType::OK == result.type);
                REQUIRE(Unit::unit == *result.val);
                REQUIRE(std::nullopt == result.error);
                REQUIRE(std::nullopt == result.message);
                
                REQUIRE(isEqual(
                    testInput,
                    {
                        {1.0, 0.0, 0.0,  4.80},
                        {0.0, 1.0, 0.0, -4.88},
                        {0.0, 0.0, 1.0,  9.09}
                    }
                    
                ));                    
            }
        }
    }
    
    GIVEN("I have a 'narrow' matrix.") {
        
        Matrix<double> testInput {{
            {1,10},
            {2,17},
            {5,11}
        }};
        
        WHEN("I run it through a rref algorithm.") {
            
            Result<Unit, ErrorCode> result {rref(testInput)};
            
            THEN("The result shoule be as expected.") {
                
                REQUIRE(OperationType::OK == result.type);
                REQUIRE(Unit::unit == *result.val);
                REQUIRE(std::nullopt == result.error);
                REQUIRE(std::nullopt == result.message);
                
                REQUIRE(isEqual(
                    testInput,
                    {
                        {1.0, 0.0},
                        {0.0, 1.0},
                        {0.0, 0.0}
                    }
                    
                ));
            }
        }
    }
}


