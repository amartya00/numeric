#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <exception>
#include <vector>
#include <iostream>

#include <catch2/catch.hpp>
#include <types/matrix.hpp>
#include <types/vector.hpp>

using Sigabrt::Types::Matrix;
using Sigabrt::Types::Vector;

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

SCENARIO("Matrix construction.") {
    
    GIVEN("I construct a 3x3 matrix from vectors.") {
        
        std::vector<std::vector<int>> srcMatrix {
            {1,2,3,100},
            {4,5,6,200},
            {7,8,9,300}
        };
        Matrix<int> destMatrix {srcMatrix};
        
        THEN("The source and destination matrix should be equal.") {
            
            REQUIRE(isEqual(destMatrix, srcMatrix));
        }
    }
    
    GIVEN("I construct a 3x3 matrix in row/col form.") {
        Matrix<int> myMatrix {3,3};
        
        THEN("The constructed matrix should be a 3x3 zero matrix.") {
            std::vector<std::vector<int>> expexted {
                {0,0,0},
                {0,0,0},
                {0,0,0}
            };
            
            REQUIRE(isEqual(myMatrix, expexted));
        }
    }
    
    GIVEN("I attempt to construct a matrix from an empty vector.") {
        
        THEN("I should recieve an invalid argument error.") {
            REQUIRE_THROWS_AS(Matrix(std::vector<std::vector<double>>()), std::invalid_argument);
        }
    }
    
    GIVEN("I attempt to construct a matrix from a vector containg another emoty vector (empty column).") {
        THEN("I should recieve an invalid argument error.") {
            std::vector<std::vector<double>> srcVector {
                {},
                {}
            };
            REQUIRE_THROWS_AS(Matrix {srcVector}, std::invalid_argument);
        }
    }
    
    GIVEN("I attempt to construct a matrix from a vector containg rows of unequal sizes.") {
        THEN("I should recieve an invalid argument error.") {
            std::vector<std::vector<double>> srcVector {
                {1.2, 2.2},
                {3.2, 1.1, 7.0}
            };
            REQUIRE_THROWS_AS(Matrix {srcVector}, std::invalid_argument);
        }
    }
}

SCENARIO("Matrix access and iterators.") {
    
    GIVEN("I have a 3x3 matrix.") {
        std::vector<std::vector<int>> srcMatrix {
            {1,2,3},
            {4,5,6},
            {7,8,9}
        };
        Matrix<int> destMatrix {srcMatrix};
        
        WHEN("I try to access some element in the matrix.") {
            int elem = destMatrix[1][2];
            
            THEN("I should get back the correct element.") {
                
                REQUIRE(srcMatrix[1][2] == elem);
            }
        }
        
        WHEN("I iterate through the matrix.") {
            
            THEN("I should be able to do so.") {
                std::size_t r {0};
                std::size_t c {0};
                
                for (const auto& row : destMatrix) {
                    for (const int& elem : row) {
                        
                        REQUIRE(srcMatrix[r][c] == elem);
                        c++;
                    }
                    c = 0;
                    r++;
                }
                
            }
        }
        
        WHEN("I try to access the wrong row.") {
            
            THEN("I should get a out of range error.") {
                
                REQUIRE_THROWS_AS(destMatrix[5][0], std::out_of_range);
            }
        }
        
        WHEN("I try to access the wrong col.") {
            
            THEN("I should get a out of range error.") {
                
                REQUIRE_THROWS_AS(destMatrix[5][0], std::out_of_range);
            }
        }
    }
    
    GIVEN("I have a 3x3 const matrix.") {
        const std::vector<std::vector<int>> srcMatrix {
            {1,2,3},
            {4,5,6},
            {7,8,9}
        };
        Matrix<int> destMatrix {srcMatrix};
        
        WHEN("I try to access some element in the matrix.") {
            int elem = destMatrix[1][2];
            
            THEN("I should get back the correct element.") {
                
                REQUIRE(srcMatrix[1][2] == elem);
            }
        }
        
        WHEN("I iterate through the matrix.") {
            
            THEN("I should be able to do so.") {
                std::size_t r {0};
                std::size_t c {0};
                
                for (const auto& row : destMatrix) {
                    for (const int& elem : row) {
                        
                        REQUIRE(srcMatrix[r][c] == elem);
                        c++;
                    }
                    c = 0;
                    r++;
                }
                
            }
        }
        
        WHEN("I try to access the wrong row.") {
            
            THEN("I should get a out of range error.") {
                
                REQUIRE_THROWS_AS(destMatrix[5][0], std::out_of_range);
            }
        }
        
        WHEN("I try to access the wrong col.") {
            
            THEN("I should get a out of range error.") {
                
                REQUIRE_THROWS_AS(destMatrix[5][0], std::out_of_range);
            }
        }
    }
}

SCENARIO("Matrix row operations happy case.") {
    
    GIVEN("I have a simple matrix") {
        
        const std::vector<std::vector<int>> srcMatrix {
            {1,2,3},
            {4,5,6},
            {7,8,9}
        };
        Matrix<int> destMatrix {srcMatrix};
        
        WHEN("I do row linear combinations on the matrix.") {
            destMatrix.linearCombRows(0, 2, 1, 3);
            
            const std::vector<std::vector<int>> expectedMatrix {
                {14, 19, 24},
                { 4,  5,  6},
                { 7,  8,  9}
            };
            
            THEN("The resulting matrix sgould be as expected.") {
                
                REQUIRE(isEqual(destMatrix, expectedMatrix));
            }
        }
        
        WHEN("I do exchange rows on the matrix.") {
            destMatrix.exchangeRows(0, 1);
            
            const std::vector<std::vector<int>> expectedMatrix {
                { 4, 5, 6},
                { 1, 2, 3},
                { 7, 8, 9}
            };
            
            THEN("The resulting matrix sgould be as expected.") {
                
                REQUIRE(isEqual(destMatrix, expectedMatrix));
            }
        }

        WHEN("I scale a matrix row.") {

            destMatrix.scaleRow(0, 5);
            const std::vector<std::vector<int>> expectedMatrix {
                { 5, 10, 15},
                { 4, 5, 6},
                { 7, 8, 9}
            };

            THEN("The resulting matrix sgould be as expected.") {
                
                REQUIRE(isEqual(destMatrix, expectedMatrix));
            }
        }
        
        WHEN("I scale the whole matrix.") {

            destMatrix.scale(5);
            const std::vector<std::vector<int>> expectedMatrix {
                {  5, 10, 15},
                { 20, 25, 30},
                { 35, 40, 45}
            };

            THEN("The resulting matrix sgould be as expected.") {
                
                REQUIRE(isEqual(destMatrix, expectedMatrix));
            }
        }
        
        WHEN("I do composite row operations") {
            
            destMatrix.linearCombRows(0, 1, 1, 2).scaleRow(0, 3).exchangeRows(0, 1);
            const std::vector<std::vector<int>> expected {
                { 4, 5, 6},
                {27,36,45},
                { 7, 8, 9}
            };
            
            THEN("The resulting matrix should be as expected.") {
                
                REQUIRE(isEqual(destMatrix, expected));
            }
        }
    }
    
    GIVEN("I have a simple matrix") {
        const std::vector<std::vector<int>> srcMatrix {
            {1,2,3},
            {4,5,6},
            {7,8,9}
        };
        Matrix<int> destMatrix {srcMatrix};
        
        WHEN("I do row linear combinations on the matrix.") {
            
            THEN("I should expect an exception.") {
                
                REQUIRE_THROWS_AS(destMatrix.linearCombRows(5, 1, 0, 2), std::out_of_range);
                REQUIRE_THROWS_AS(destMatrix.linearCombRows(0, 1, 7, 2), std::out_of_range);
                REQUIRE_THROWS_AS(destMatrix.linearCombRows(7, 1, 7, 2), std::out_of_range);
            }
        }
        
        WHEN("I exchange rows in the matrix with invalid rows.") {
            
            THEN("I should expect an exception.") {
                
                REQUIRE_THROWS_AS(destMatrix.exchangeRows(10, 0), std::out_of_range);
                REQUIRE_THROWS_AS(destMatrix.exchangeRows(0, 10), std::out_of_range);
                REQUIRE_THROWS_AS(destMatrix.exchangeRows(10, 10), std::out_of_range);
            }
        }
        
        WHEN("I scale an invalid row.") {
            
            THEN("I should expect an exception.") {
                
                REQUIRE_THROWS_AS(destMatrix.scaleRow(10, 0), std::out_of_range);
            }
        }
    }
}

SCENARIO("Multi matrix operations.") {
    
    GIVEN("I have 2 matrices of same dims.") {
        
        Matrix<double> m1 {
            {
                {1,2,3,4},
                {1,2,3,4},
                {1,2,3,4}
            }
        };
        Matrix<double> m2 {
            {
                {1,2,3,4},
                {1,2,3,4},
                {1,2,3,4}
            }
        };
        
        WHEN("I add them.") {
            
            Matrix<double> added = m1 + m2;
            
            THEN("The resultant matrix should be as expected.") {
                
                std::vector<std::vector<double>> expected {
                    {2,4,6,8},
                    {2,4,6,8},
                    {2,4,6,8}
                };
                
                REQUIRE(isEqual(added, expected));
            }
        }
        
        WHEN("I subtract them.") {
            
            Matrix<double> res = m1 - m2;
            
            THEN("The resultant matrix should be as expected.") {
                
                std::vector<std::vector<double>> expected {
                    {0,0,0,0},
                    {0,0,0,0},
                    {0,0,0,0}
                };
                
                REQUIRE(isEqual(res, expected));
            }
        }
    }
    
    GIVEN("I have 2 matrices of different dims.") {
        
        Matrix<double> m1 {
            {
                {1,2,3,4},
                {1,2,3,4},
                {1,2,3,4}
            }
        };
        Matrix<double> m2 {
            {
                {1,2,3,4,5},
                {1,2,3,4,5},
                {1,2,3,4,5}
            }
        };
        
        WHEN("I add them.") {
            
            THEN("The operation should throw an error.") {
                
                REQUIRE_THROWS_AS(m1 + m2, std::invalid_argument);
            }
        }
        
        WHEN("I subtract them.") {
            
            THEN("The operation should throw an error.") {
                
                REQUIRE_THROWS_AS(m1 - m2, std::invalid_argument);
            }
        }
    }
    
    GIVEN("I have 2 compatible matrices.") {
        
        Matrix<double> m1 {
            {
                {1,2,3},
                {4,5,6}
            }
        };
        Matrix<double> m2 {
            {
                {7,8},
                {9,10},
                {11,12}
            }
        };
        
        WHEN("I multiply them.") {
            
            Matrix<double> product = m1 * m2;
            
            THEN("The resultant matrix should be as expected.") {
                std::vector<std::vector<double>> expected {
                    {
                        {58, 64},
                        {139, 154}
                    }
                };
                REQUIRE(isEqual(product, expected));
            }
        }
    }
    
    GIVEN("I have 2 incompatible matrices.") {
        
        Matrix<double> m1 {
            {
                {1,2,3},
                {4,5,6}
            }
        };
        Matrix<double> m2 {
            {
                {7,8},
                {9,10},
                {11,12},
                {13,14}
            }
        };
        
        WHEN("I multiply them.") {
            
            THEN("The operation should throw an exception.") {
                
                REQUIRE_THROWS_AS(m1 * m2, std::invalid_argument);
            }
        }
    }
    
    GIVEN("I have a matrix and a compatible vector.") {
        Matrix<double> m1 {
            {
                {1,1,1,1},
                {1,1,1,1},
                {1,1,1,1}
            }
        };
        Vector<double> v1 {{1,1,1,1}};
        
        WHEN("I multiply the 2") {
            
            Vector<double> result {m1 * v1};
            
            THEN("The result shoule be expected.") {
                
                std::vector<double> expected {4,4,4};
                
                REQUIRE(3 == result.size());
                REQUIRE(std::equal(expected.begin(), expected.end(), result.begin()));
            }
        }
    }
    
    GIVEN("I have a matrix and a incompatible vector.") {
        Matrix<double> m1 {
            {
                {1,1,1,1},
                {1,1,1,1},
                {1,1,1,1}
            }
        };
        Vector<double> v1 {{1,1,1,1,1}};
        
        WHEN("I multiply the 2") {
            
            THEN("The operation should throw an exception.") {
                
                REQUIRE_THROWS_AS(m1 * v1, std::invalid_argument);
            }
        }
    }
    
    GIVEN("I have a vector and a compatible matrix.") {
        Matrix<double> m1 {
            {
                {1,1,1,1},
                {1,1,1,1},
                {1,1,1,1}
            }
        };
        Vector<double> v1 {{1,1,1,1}};
        
        WHEN("I multiply the 2") {
            
            Vector<double> result {v1 * m1};
            
            THEN("The result shoule be expected.") {
                
                std::vector<double> expected {4,4,4};
                
                REQUIRE(3 == result.size());
                REQUIRE(std::equal(expected.begin(), expected.end(), result.begin()));
            }
        }
    }
    
    GIVEN("I have a vector and a incompatible matrix.") {
        Matrix<double> m1 {
            {
                {1,1,1,1},
                {1,1,1,1},
                {1,1,1,1}
            }
        };
        Vector<double> v1 {{1,1,1,1,1}};
        
        WHEN("I multiply the 2") {
            
            THEN("The operation should throw an exception.") {
                
                REQUIRE_THROWS_AS(v1 * m1, std::invalid_argument);
            }
        }
    }
    
    GIVEN("I have 2 compatible vectors.") {
        Vector<double> v0 {{1,1,1,1}};
        Vector<double> v1 {{1,1,1,1}};
        
        WHEN("I multiply the 2") {
            
            double result {v0 * v1};
            
            THEN("The result shoule be expected.") {
                
            
                REQUIRE(4.0 == result);
            }
        }
    }
    
    GIVEN("I have 2 incompatible vectors.") {
        Vector<double> v0 {{1,1,1,1,1,1,1,1,1,1}};
        Vector<double> v1 {{1,1,1,1,1}};
        
        WHEN("I multiply the 2") {
            
            THEN("The operation should throw an exception.") {
                
                REQUIRE_THROWS_AS(v0 * v1, std::invalid_argument);
            }
        }
    }
}

SCENARIO("Composite matrix operations.") {
    
    GIVEN("I have some matrices and vectors.") {
        Matrix<double> M {
            {
                {1,1,1,1},
                {1,1,1,1},
                {1,1,1,1}
            }  
        };
        Matrix<double> I {Matrix<double>::identity(4)};
        Vector<double> v1 {{1,1,1,1}};
        Vector<double> v2 {{1,1,1}};
        
        WHEN("I do a chain multiplication of these.") {
            double result {M * I * v1 * v2};
            
            THEN("The result should be as expected") {
                
                REQUIRE(12 == result);
            }
        }
    }
}
