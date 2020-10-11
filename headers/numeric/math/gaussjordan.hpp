#ifndef __SIGABRT_NUMERIC_GAUSSJORDAN__
#define __SIGABRT_NUMERIC_GAUSSJORDAN__

#include <optional>
#include <vector>

#include <numeric/types/models.hpp>
#include <numeric/types/matrix.hpp>


/**
 * \namespace Sigabrt
 * 
 * \brief The root namespace.
 * */
namespace Sigabrt {
    /**
     * \namespace Sigabrt::Numeric
     * 
     * \brief Sub namespace with all numeric classes and functions.
     * */
    namespace Numeric {
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
        template <typename T> Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> 
        gaussJordan(Sigabrt::Types::Matrix<T>& matrix) {
            // Step 0: Validate matrix in augmented form
            if (matrix.getRows() < matrix.getCols()-1) {
                return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                    Sigabrt::Types::OperationType::ERR,
                    Sigabrt::Types::Unit::unit,
                    Sigabrt::Types::ErrorCode::UNDERDETERMINED_SYSTEM,
                    std::string("The number of equations in the augmented matrix is less than the number of variables.")
                };
            }
            
            auto retval = rref(matrix);
            if (retval.type == Sigabrt::Types::OperationType::ERR) {
                
                if (retval.error == Sigabrt::Types::ErrorCode::FREE_COLUMNS_RREF) {
                    // We have free columns. We need to check for the case where we have NO solutions.
                    // If we cannot find one, then there will be infinitely many solutions.
                    for (std::size_t rowNum = 0; rowNum < matrix.getRows(); rowNum++) {
                        if (Sigabrt::Numeric::falseIndentityRow(matrix[matrix.getRows() - rowNum -1])) {
                            return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                                Sigabrt::Types::OperationType::ERR,
                                Sigabrt::Types::Unit::unit,
                                Sigabrt::Types::ErrorCode::NO_SOLUTIONS,
                                std::string("This system of equations has no solutions.")
                            };
                        }
                    }
                    return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                        Sigabrt::Types::OperationType::ERR,
                        Sigabrt::Types::Unit::unit,
                        Sigabrt::Types::ErrorCode::INFINITE_SOLUTIONS,
                        std::string("This system of equations has infinite solutions.")
                    };
                }
                else {
                    return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                        Sigabrt::Types::OperationType::ERR,
                        Sigabrt::Types::Unit::unit,
                        Sigabrt::Types::ErrorCode::UNKNOWN_ERROR,
                        std::nullopt
                    };
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
         * value less than the `zeroPrecision` parameter), to zero.
         * 
         * NOTE: In this version of the function, if you are using a non primitive type, it has to support conversion to double.
         * 
         * \param matrix: 
         *   Matrix<T> The **non const** reference to the input matrix.
         * 
         * \param zeroPrecision:
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
        template <typename T> Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> 
        gaussJordan(Sigabrt::Types::Matrix<T>& matrix, const double& zeroPrecision) {
            // Step 0: Validate matrix in augmented form
            if (matrix.getRows() < matrix.getCols()-1) {
                return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                    Sigabrt::Types::OperationType::ERR,
                    Sigabrt::Types::Unit::unit,
                    Sigabrt::Types::ErrorCode::UNDERDETERMINED_SYSTEM,
                    std::string("The number of equations in the augmented matrix is less than the number of variables.")
                };
            }
            
            auto retval = rref(matrix, zeroPrecision);
            if (retval.type == Sigabrt::Types::OperationType::ERR) {
                if (retval.error == Sigabrt::Types::ErrorCode::FREE_COLUMNS_RREF) {
                    // We have free columns. We need to check for the case where we have NO solutions.
                    // If we cannot find one, then there will be infinitely many solutions.
                    for (std::size_t rowNum = 0; rowNum < matrix.getRows(); rowNum++) {
                        if (Sigabrt::Numeric::falseIndentityRow(matrix[matrix.getRows() - rowNum -1])) {
                            return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                                Sigabrt::Types::OperationType::ERR,
                                Sigabrt::Types::Unit::unit,
                                Sigabrt::Types::ErrorCode::NO_SOLUTIONS,
                                std::string("This system of equations has no solutions.")
                            };
                        }
                    }
                    return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                        Sigabrt::Types::OperationType::ERR,
                        Sigabrt::Types::Unit::unit,
                        Sigabrt::Types::ErrorCode::INFINITE_SOLUTIONS,
                        std::string("This system of equations has infinite solutions.")
                    };
                }
                else {
                    return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                        Sigabrt::Types::OperationType::ERR,
                        Sigabrt::Types::Unit::unit,
                        Sigabrt::Types::ErrorCode::UNKNOWN_ERROR,
                        std::nullopt
                    };
                }
                
            } else {
                return retval;
            }
            
        }
    }
}

#endif
