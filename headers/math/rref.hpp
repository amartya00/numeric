#ifndef __SIGABRT_NUMERIC_RREF__
#define __SIGABRT_NUMERIC_RREF__

#include <optional>
#include <vector>
#include <models.hpp>

#include <math/matrix.hpp>

using Sigabrt::Numeric::Models::Result;
using Sigabrt::Numeric::Models::OperationType;
using Sigabrt::Numeric::Models::ErrorCode;
using Sigabrt::Numeric::Models::Unit;

namespace Sigabrt {
    namespace Numeric {
        template <typename T>
        std::optional<std::size_t> findNextPivot(
            const Matrix<T>& matrix, 
            const std::size_t& startRow, 
            const std::size_t& startCol) {
            for (std::size_t i = startRow+1; i < matrix.getRows(); i++) {
                if  (matrix[i][startCol] != 0) {
                    return i;
                }
            }
            return std::nullopt;
        }
        
        /**
         * \fn gsuaaJordan
         * 
         * This is the Gauss Jordan elimination algorithm to sole a system of linear equations. It takes a non const
         * matrix (of type `Matrix<T>`) as input ans reduces it to RREF form. It does not return a vector of solutions. 
         * The caller is responsible for parsing the last column to construct the solution. It however does return a
         * `Result<Unit, ErrorCode>` to indicate the status of the computation.
         * 
         * \param matrix: 
         *   Matrix<T> The **non const** reference to the input matrix.
         * \return result: 
         *   Result<Unit, ErrorCode> Result to indicate the operation status.
         *   Possible error codes:
         *   - `UNDERDETERMINED_SYSTEM`: If the matrix has less equations than variables in its augmented form.
         *   - `FREE_COLUMNS_RREF`: If free columns are detected during reduction. This indicates a lack of a unique solution.
         * 
         * */
        template <typename T> Result<Unit, ErrorCode> gaussJordan(Matrix<T>& matrix) {
            // Step 0: Validate matrix in augmented form
            if (matrix.getRows() < matrix.getCols()-1) {
                return Result<Unit, ErrorCode> {
                    OperationType::ERR,
                    Unit::unit,
                    ErrorCode::UNDERDETERMINED_SYSTEM,
                    std::string("The number of equations in the augmented matrix is less than the number of variables.")
                };
            }
            
            for (std::size_t i = 0; i < matrix.getRows(); i++) {
                // If pivot element is zero, we need to make it non zero
                if (matrix[i][i] == static_cast<T>(0)) {
                    std::optional<T> nextPivot = findNextPivot(matrix, i, i);
                    if (nextPivot == std::nullopt) {
                        return Result<Unit, ErrorCode> {
                            OperationType::ERR,
                            Unit::unit,
                            ErrorCode::FREE_COLUMNS_RREF,
                            std::string("Free columns detected in RREF form. this system does not have a unique solution.")
                        };
                    } else {
                        matrix.exchangeRows(i, *nextPivot);
                    }
                }
                
                // Normalize pivot element
                matrix.scaleRow(i, static_cast<T>(1)/matrix[i][i]);
                
                // Operate on subsequent rows.
                // TODO: Parallelize this
                for (std::size_t other_rows = 0; other_rows < matrix.getRows(); other_rows++) {
                    if (matrix[other_rows][i] == static_cast<T>(0) || other_rows == i) {
                        continue;
                    }
                    T div = -matrix[other_rows][i];
                    matrix.linearCombRows(other_rows, static_cast<T>(1), i,  div);
                }
                
            }
            
            return Result<Unit, ErrorCode> {
                OperationType::OK,
                Unit::unit,
                std::nullopt,
                std::nullopt
            };
        }
    }
}

#endif
