#ifndef __SIGABRT_NUMERIC_GAUSSJORDAN__
#define __SIGABRT_NUMERIC_GAUSSJORDAN__

#include <optional>
#include <vector>

#include <numeric/types/models.hpp>
#include <numeric/types/matrix.hpp>
#include <numeric/math/errors.hpp>

#include <thesoup/types/types.hpp>


/**
 * \namespace numeric
 * 
 * \brief The root namespace.
 * */
namespace numeric {
    /**
     * \namespace numeric::functions
     * 
     * \brief Sub namespace with all numeric classes and functions.
     * */
    namespace functions {
        /**
         * \brief Function to solve a system of linear equations.
         * 
         * This is the Gauss Jordan elimination algorithm to sole a system of linear equations. It takes a non const
         * matrix (of type `Matrix<T>`) as input. The matrix is expected to be an augmented matrix with proper number
         * of equations. Under-represented matrix will result in error. 
         * 
         * The matrix is reduces to RREF form. It does not return a vector of solutions. The caller is responsible for 
         * parsing the last column to construct the solution. It however does return a `Result<Unit, ErrorCode>` to 
         * indicate the status of the computation.
         * 
         * The reduction is done in place to avoid copies.
         * 
         * \param matrix: 
         *   Matrix<T> The **non const** reference to the input matrix.
         * 
         * \return result: 
         *   Result<Unit, ErrorCode> Result to indicate the operation status.
         * 
         *   Possible error codes:
         *   - `INFINITE_SOLUTIONS`: If the matrix has less equations than variables in its augmented form.
         *   - `NO_SOLUTIONS`: If free columns are detected during reduction. This indicates a lack of a unique solution.
         * 
         * */
        template <typename T> thesoup::types::Result<thesoup::types::Unit, numeric::ErrorCode>
        gauss_jordan(numeric::types::Matrix<T>& matrix) {
            // Step 0: Validate matrix in augmented form
            if (matrix.getRows() < matrix.getCols()-1) {
                return thesoup::types::Result<thesoup::types::Unit, numeric::ErrorCode>::failure(numeric::ErrorCode::UNDERDETERMINED_SYSTEM);
            }
            
            auto retval = rref(matrix);
            if (retval) {
                if (retval.error() == numeric::ErrorCode::FREE_COLUMNS_RREF) {
                    // We have free columns. We need to check for the case where we have NO solutions.
                    // If we cannot find one, then there will be infinitely many solutions.
                    for (std::size_t rowNum = 0; rowNum < matrix.getRows(); rowNum++) {
                        if (numeric::functions::false_identity_row(matrix[matrix.getRows() - rowNum - 1])) {
                            return thesoup::types::Result<thesoup::types::Unit, numeric::ErrorCode>::failure(numeric::ErrorCode::NO_SOLUTIONS);
                        }
                    }
                    return thesoup::types::Result<thesoup::types::Unit, numeric::ErrorCode>::failure(numeric::ErrorCode::INFINITE_SOLUTIONS);
                }
                else {
                    return thesoup::types::Result<thesoup::types::Unit, numeric::ErrorCode>::failure(numeric::ErrorCode::UNKNOWN_ERROR);
                }
            } else {
                return retval;
            }
            
        }
        
        /**
         * \brief Function to solve a system of linear equations.
         * 
         * This is the Gauss Jordan elimination algorithm to sole a system of linear equations. It takes a non const
         * matrix (of type `Matrix<T>`) as input. The matrix is expected to be an augmented matrix with proper number
         * of equations. Under-represented matrix will result in error. 
         * 
         * The matrix is reduces to RREF form. It does not return a vector of solutions. The caller is responsible for 
         * parsing the last column to construct the solution. It however does return a `Result<Unit, ErrorCode>` to 
         * indicate the status of the computation.
         * 
         * The reduction is done in place to avoid copies.
         * 
         * During computation, this function handles precision errors by rounding off very small numbers (with an absolute 
         * value less than the `zero_precision` parameter), to zero.
         * 
         * NOTE: In this version of the function, if you are using a non primitive type, it has to support conversion to double.
         * 
         * \param matrix: 
         *   Matrix<T> The **non const** reference to the input matrix.
         * 
         * \param zero_precision:
         *   The double value which is considered to be the threshold to be 0. Example if this is set to 1e-10, all numbers
         *   between -(1e-10 ) and 1e-10 will be considered 0.
         * 
         * \return result: 
         *   Result<Unit, ErrorCode> Result to indicate the operation status.
         * 
         *   Possible error codes:
         *   - `UNDERDETERMINED_SYSTEM`: If the matrix has less equations than variables in its augmented form.
         *   - `FREE_COLUMNS_RREF`: If free columns are detected during reduction. This indicates a lack of a unique solution.
         * 
         * */
        template <typename T> thesoup::types::Result<thesoup::types::Unit, numeric::ErrorCode>
        gauss_jordan(numeric::types::Matrix<T>& matrix, const double& zero_precision) {
            // Step 0: Validate matrix in augmented form
            if (matrix.getRows() < matrix.getCols()-1) {
                return thesoup::types::Result<thesoup::types::Unit, numeric::ErrorCode>::failure(numeric::ErrorCode::UNDERDETERMINED_SYSTEM);
            }
            
            auto retval {rref(matrix, zero_precision)};
            if (!retval) {
                if (retval.error() == numeric::ErrorCode::FREE_COLUMNS_RREF) {
                    // We have free columns. We need to check for the case where we have NO solutions.
                    // If we cannot find one, then there will be infinitely many solutions.
                    for (std::size_t rowNum = 0; rowNum < matrix.getRows(); rowNum++) {
                        if (numeric::functions::false_identity_row(matrix[matrix.getRows() - rowNum - 1])) {
                            return thesoup::types::Result<thesoup::types::Unit, numeric::ErrorCode>::failure(numeric::ErrorCode::NO_SOLUTIONS);
                        }
                    }
                    return thesoup::types::Result<thesoup::types::Unit, numeric::ErrorCode>::failure(numeric::ErrorCode::INFINITE_SOLUTIONS);
                }
                else {
                    return thesoup::types::Result<thesoup::types::Unit, numeric::ErrorCode>::failure(numeric::ErrorCode::UNKNOWN_ERROR);
                }
                
            } else {
                return retval;
            }
            
        }
    }
}

#endif
