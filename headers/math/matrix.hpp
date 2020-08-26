#ifndef __SIGABRT_NUMERIC_MATRIX__
#define __SIGABRT_NUMERIC_MATRIX__


#include <cstddef>
#include <exception>
#include <memory>
#include <vector>

#include <models.hpp>

using Sigabrt::Numeric::Models::Slice;

namespace Sigabrt {
    namespace Numeric {
        ///
        /// Matrix stricture. This represents a 2D matrix of elements of type `T` with some constrains. This structure has the copy
        /// constructor and assignment operators deleted to prevent extremely expensive operations.
        ///
        /// This structure defines methods for basic row operations:
        ///   - Linear combination
        ///   - Exchange
        ///   - Scalar multiplication
        ///   - Find next pivot element after a row. This is necessary in algorithms like Gauss Jordan, if the current pivot is 0.
        ///
        /// Matrix also has index operations defined. You can do `matrix[i][j]` on both const and non const objects/references
        ///
        template <typename T> class Matrix {
        private:
            std::size_t nrows;
            std::size_t ncols;
            std::unique_ptr<Slice<T>[]> rows;
            std::unique_ptr<T[]> storage;
            
            void initializeSlices() {
                for (std::size_t i = 0; i < nrows; i++) {
                    rows[i].start = storage.get()+i*ncols;
                    rows[i].size = ncols;
                }
            }

        public:
            ///
            /// Constructor.
            /// Arguments:
            ///   - nrows: Number of rows.
            ///   - ncols: Number of columns in matrix.
            /// Returns:
            ///   Matrix (rows x cols)
            ///
            Matrix(
                const std::size_t& nrows, 
                const std::size_t& ncols
            ): nrows {nrows}, 
                ncols {ncols}, 
                rows {std::make_unique<Slice<T>[]>(nrows)},
                storage {std::make_unique<T[]>(nrows * ncols)} {
                    for (std::size_t i = 0; i < nrows; i++) {
                        for (std::size_t j = 0; j < ncols; j++) {
                            storage[i*ncols + j] = static_cast<T>(0);
                        }
                    }
                    initializeSlices();
                }

            ///
            /// Constructor. This moves the input vector's ownership to itself.
            /// Arguments:
            ///   - Slice of vectors.
            /// Returns:
            ///   - Matrix
            ///
            Matrix(const std::vector<std::vector<T>>& vecs) {
                if (vecs.size() == 0) {
                    throw std::invalid_argument("Matrix cannot have 0 rows.");
                } else if (vecs[0].size() == 0) {
                    throw std::invalid_argument("Matrix cannot row with 0 elements.");
                } else {
                    nrows = vecs.size();
                    ncols = vecs[0].size();
                    rows = std::make_unique<Slice<T>[]>(nrows);
                    storage = std::make_unique<T[]>(nrows * ncols);
                    
                    for (std::size_t i = 0; i < nrows; i++) {
                        if (vecs[i].size() != ncols) {
                            throw std::invalid_argument("All rows of source matrix have to be of same length.");
                        }
                        for (std::size_t j = 0; j < ncols; j++) {
                            storage[i*ncols + j] = vecs[i][j];
                        }
                    }
                    initializeSlices();
                }
            }

            Matrix(const Matrix<T>& other)=delete;
            void operator=(const Matrix<T> other)=delete;

            Matrix(Matrix<T>&& other): nrows {other.nrows}, ncols {other.ncols}, rows {std::move(other.rows)}, storage {std::move(other.storage)} {}

            Slice<T>& operator[](const std::size_t& row) {
                if (row >= nrows) {
                    throw std::out_of_range("Array index out of range");
                }
                
                return rows[row];
            }

            const Slice<T>& operator[](const std::size_t& row) const {
                if (row >= nrows) {
                    throw std::out_of_range("Array index out of range");
                }
                return rows[row];
            }

            const Slice<T>* begin() const {
                return &rows[0];
            }

            const Slice<T>* end() const {
                return &rows[nrows];
            }

            Slice<T>* begin() {
                return &rows[0];
            }

            Slice<T>* end() {
                return &rows[nrows];
            }

            ///
            /// Identity matrix generator. This returns an `nxn` identity matrix.
            /// Arguments:
            ///   - Size if identity matrix
            /// Returns:
            ///   - Identity matrix of size `n`.
            ///
            static Matrix<T> identity(const std::size_t& sz) {
                Matrix<T> retval {sz, sz};
                for (std::size_t i = 0; i < sz; i++) {
                    retval[i][i] = static_cast<T>(1);
                }
                return retval;
            }
            
            ///
            /// Returns the number of rows in the matrix
            ///
            const std::size_t& getRows() const {
                return nrows;
            }
            
            ///
            /// Returns the number of columns in the matrix
            ///
            const std::size_t& getCols() const {
                return ncols;
            }
            
            ///
            /// This function replaces row R1 -> aR1 + bR2
            ///
            void linearCombRows(
                const std::size_t& r1,
                const T& a,
                const std::size_t& r2,
                const T& b
            ) {
                if (r1 > nrows || r2 > nrows) {
                    throw std::out_of_range("Row access out of range.");
                } else {
                    for (std::size_t i = 0; i < ncols; i++) {
                        rows[r1][i] = a*rows[r1][i] + b*rows[r2][i];
                    }
                }
            }
            
            ///
            /// This function exchanges 2 rows in the matrix.
            ///
            void exchangeRows(const std::size_t& r1, const std::size_t& r2) {
                if (r1 > nrows || r2 > nrows) {
                    throw std::out_of_range("Row access out of range.");
                } else {
                    std::swap(rows[r1].start, rows[r2].start);
                }
            }

            ///
            /// This function scales a row by `factor`
            ///
            void scale(const std::size_t& row, const T& factor) {
                if (row > nrows) {
                    throw std::out_of_range("Row access out of range.");
                }
                for (auto& elem : rows[row]) {
                    elem *= factor;
                }
            }
        };
    }
}

#endif
