#ifndef __SIGABRT_NUMERIC_RREF__
#define __SIGABRT_NUMERIC_RREF__

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
            
            template<typename T> T roundOffToZero(const T& val, const double& zeroPrecision) {
                if (static_cast<double>(val) > -zeroPrecision && static_cast<double>(val) < zeroPrecision) {
                    return static_cast<T>(0.0);
                } else {
                    return val;
                }
            }
        }
        
        /**
         * \brief Function to see if a matrix row (slice) is a false identity row.
         * 
         * \tparam T Type of elements in the slice.
         * 
         * This function determines if a matrix row represents a false identity, that is an equation
         * of the form 1 = 0. The condition for this to hold is that all elements except the last in the 
         * slice are 0.
         * 
         * \param row The slice.
         * 
         * \return bool indicating whether the row is a false ID or not.
         * */
        template <typename T> bool falseIndentityRow(const Sigabrt::Types::Slice<T>& row) {
            const T& rightMost {row[row.size-1]};
            if (rightMost == 0) {
                return false;
            } else {
                for (std::size_t i = 0; i < row.size-1; i++) {
                    const T& elem {row[i]};
                    if (elem != static_cast<T>(0)) {
                        return false;
                    }
                }
                return true;
            }
        }
        
        /**
         * \brief Function to see if a matrix row (slice) is an identity row.
         * 
         * \tparam T Type of elements in the slice.
         * 
         * This function determines if a matrix row represents an identity, that is an equation
         * of the form 0=0 or 1=1. The condition for this to hold is that all elements in the slice 
         * are 0.
         * 
         * \param row The slice.
         * 
         * \return bool indicating whether the row is a false ID or not.
         * */
        template <typename T> bool identityRow(const Sigabrt::Types::Slice<T>& row) {
            const T& first {row[0]};
            for (const auto& elem : row) {
                if (elem != first) {
                    return false;
                }
            }
            return true;
        }
        
        /**
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
            std::size_t smallerDim {matrix.getRows() < matrix.getCols()? matrix.getRows(): matrix.getCols()};
            for (std::size_t i = 0; i < smallerDim; i++) {
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
            std::size_t smallerDim {matrix.getRows() < matrix.getCols()? matrix.getRows(): matrix.getCols()};
            for (std::size_t i = 0; i < smallerDim; i++) {
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
    }
}

#endif
