#ifndef __SIGABRT_NUMERIC_RREF__
#define __SIGABRT_NUMERIC_RREF__

#include <optional>
#include <vector>

#include <types/models.hpp>
#include <types/matrix.hpp>

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
        namespace {
            template <typename T>
            std::optional<std::size_t> findNextPivot(
                const Sigabrt::Types::Matrix<T>& matrix, 
                const std::size_t& startRow, 
                const std::size_t& startCol) {
                for (std::size_t i = startRow+1; i < matrix.getRows(); i++) {
                    if  (matrix[i][startCol] != 0) {
                        return i;
                    }
                }
                return std::nullopt;
            }
            
            template<typename T> bool identityRow(const Sigabrt::Types::Slice<T>& row) {
                const T& first {row[0]};
                for (const auto& elem : row) {
                    if (elem != first) {
                        return false;
                    }
                }
                return true;
            }
            
            template<typename T> T roundOffToZero(const T& val, const double& zeroPrecision) {
                if (static_cast<double>(val) > -zeroPrecision && static_cast<double>(val) < zeroPrecision) {
                    return static_cast<T>(0.0);
                } else {
                    return val;
                }
            }
        }
        
        /**
         * \function rref
         * 
         * \brief Function to perform RREF on a matrix.
         * 
         * This function performs a vanilla RREF reduction on a matrix. There is no restriction on the shape of the matrix.
         * Existence of free elements after the computation is complete will restult in a `FREE_COLUMNS_RREF` error. Note,
         * that this function does not throw exceptions. It returns a `Result<T,E>` to indicate the results of computation.
         * 
         * \param matrix: 
         *   Matrix<T> The **non const** reference to the input matrix.
         * 
         * \return result: 
         *   Result<Unit, ErrorCode> Result to indicate the operation status.
         * 
         *   Possible error codes:
         *   - `FREE_COLUMNS_RREF`: If free columns are detected during reduction. This indicates a lack of a unique solution.
         * */
        template <typename T> Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> 
        rref(Sigabrt::Types::Matrix<T>& matrix) {
            bool freeElements {false};
            for (std::size_t i = 0; i < matrix.getRows(); i++) {
                // If pivot element is zero, we need to make it non zero
                if (matrix[i][i] == static_cast<T>(0)) {
                    std::optional<T> nextPivot = findNextPivot(matrix, i, i);
                    if (nextPivot == std::nullopt) {
                        freeElements = true;
                        continue;
                    } else {
                        matrix.exchangeRows(i, *nextPivot);
                    }
                }
                
                // Operate on subsequent rows.
                // TODO: Parallelize this
                for (std::size_t other_rows = 0; other_rows < matrix.getRows(); other_rows++) {
                    if (matrix[other_rows][i] == static_cast<T>(0) || other_rows == i) {
                        continue;
                    }
                    T div = -(matrix[other_rows][i]/matrix[i][i]);
                    matrix.linearCombRows(other_rows, static_cast<T>(1), i,  div);
                    matrix[other_rows][i] = static_cast<T>(0);
                }
                
                // Normalize pivot element
                matrix.scaleRow(i, static_cast<T>(1)/matrix[i][i]);
                
            }
            
            if (freeElements) {
                return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                    Sigabrt::Types::OperationType::ERR,
                    Sigabrt::Types::Unit::unit,
                    Sigabrt::Types::ErrorCode::FREE_COLUMNS_RREF,
                    std::nullopt
                };
            } else {
                return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                    Sigabrt::Types::OperationType::OK,
                    Sigabrt::Types::Unit::unit,
                    std::nullopt,
                    std::nullopt
                };
            }
            
        }
        
        /**
         * \function rref
         * 
         * \brief Function to perform RREF on a matrix.
         * 
         * This function performs a vanilla RREF reduction on a matrix. There is no restriction on the shape of the matrix.
         * Existence of free elements after the computation is complete will restult in a `FREE_COLUMNS_RREF` error. Note,
         * that this function does not throw exceptions. It returns a `Result<T,E>` to indicate the results of computation.
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
         *   - `FREE_COLUMNS_RREF`: If free columns are detected during reduction. This indicates a lack of a unique solution.
         * */
        template <typename T> Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> 
        rref(Sigabrt::Types::Matrix<T>& matrix, const double& zeroPrecision) {
            bool freeElements {false};
            for (std::size_t i = 0; i < matrix.getRows(); i++) {
                // If pivot element is zero, we need to make it non zero
                if (matrix[i][i] == static_cast<T>(0)) {
                    std::optional<T> nextPivot = findNextPivot(matrix, i, i);
                    if (nextPivot == std::nullopt) {
                        freeElements = true;
                        continue;
                    } else {
                        matrix.exchangeRows(i, *nextPivot);
                    }
                }
                
                // Operate on subsequent rows.
                // TODO: Parallelize this.
                for (std::size_t otherRow = 0; otherRow < matrix.getRows(); otherRow++) {
                    if (matrix[otherRow][i] == static_cast<T>(0) || otherRow == i) {
                        continue;
                    }
                    T div = -(matrix[otherRow][i]/matrix[i][i]);
                    // Operate on the row
                    matrix.linearCombRows(otherRow, static_cast<T>(1), i,  div);
                    
                    // Round off the row
                    for (auto& elem : matrix[otherRow]) {
                        elem = roundOffToZero(elem, zeroPrecision);
                    }
                }
                
                // Normalize pivot row, and round off.
                matrix.scaleRow(i, static_cast<T>(1)/matrix[i][i]);
                for (auto& elem : matrix[i]) {
                    elem = roundOffToZero(elem, zeroPrecision);
                }
                
            }
            
            if (freeElements) {
                return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                    Sigabrt::Types::OperationType::ERR,
                    Sigabrt::Types::Unit::unit,
                    Sigabrt::Types::ErrorCode::FREE_COLUMNS_RREF,
                    std::nullopt
                };
            } else {
                return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                    Sigabrt::Types::OperationType::OK,
                    Sigabrt::Types::Unit::unit,
                    std::nullopt,
                    std::nullopt
                };
            }
            
        }
        
        /**
         * \function gsuaaJordan
         * 
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
         *   - `UNDERDETERMINED_SYSTEM`: If the matrix has less equations than variables in its augmented form.
         *   - `FREE_COLUMNS_RREF`: If free columns are detected during reduction. This indicates a lack of a unique solution.
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
                    for (std::size_t rowNum = 0; rowNum < matrix.getRows(); rowNum++) {
                        if (identityRow(matrix[matrix.getRows() - rowNum -1])) {
                            return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                                Sigabrt::Types::OperationType::ERR,
                                Sigabrt::Types::Unit::unit,
                                Sigabrt::Types::ErrorCode::INFINITE_SOLUTIONS,
                                std::string("This system of equations has infinite solutions.")
                            };
                        }
                    }
                    return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                        Sigabrt::Types::OperationType::ERR,
                        Sigabrt::Types::Unit::unit,
                        Sigabrt::Types::ErrorCode::NO_SOLUTIONS,
                        std::string("This system of equations has no solutions.")
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
         * \function gsuaaJordan
         * 
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
                    for (std::size_t rowNum = 0; rowNum < matrix.getRows(); rowNum++) {
                        if (identityRow(matrix[matrix.getRows() - rowNum -1])) {
                            return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                                Sigabrt::Types::OperationType::ERR,
                                Sigabrt::Types::Unit::unit,
                                Sigabrt::Types::ErrorCode::INFINITE_SOLUTIONS,
                                std::string("This system of equations has infinite solutions.")
                            };
                        }
                    }
                    return Sigabrt::Types::Result<Sigabrt::Types::Unit, Sigabrt::Types::ErrorCode> {
                        Sigabrt::Types::OperationType::ERR,
                        Sigabrt::Types::Unit::unit,
                        Sigabrt::Types::ErrorCode::NO_SOLUTIONS,
                        std::string("This system of equations has no solutions.")
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
