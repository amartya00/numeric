#ifndef __SIGABRT_NUMERIC_VECTOR__
#define __SIGABRT_NUMERIC_VECTOR__

#include <cmath>
#include <exception>
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <vector>

#include <types/models.hpp>

/**
 * \namespace Sigabrt
 * 
 * \brief The root namespace.
 * */
namespace Sigabrt {
    /**
     * \namespace Sigabrt::Types
     * 
     * \brief The namespace containing some special types.
     * */
    namespace Types {
        /**
         * \class Vector
         * \tparam T Some numeric type
         * 
         * \brief A class representing a mathematical vector.
         * 
         * This class represents a vector (as in linear algebra). This has a lot of differences from `std::vector` both structurally
         * and in interfaces.
         * As far as interfaces are concerned, this does not have any push functionalities and it's size is fixed at create time.
         * Structurally, as this does not support resize, all the table doubling logic is missing. 
         * 
         * NOTE: Some restrictions on using this class are are:
         *   - The size is fixed at construction time.
         *   - Type `T` has to be default constructible, all elements are supposed to be initialized at create time.
         *   - There is no copy constructor and assignment operator. They have been deleted. You do have move constructor and move assignment.
         * 
         * Functionality:
         *   - A `mod` function that gives the root sum of square size.
         *   - The usual `size` function.
         *   - Defined add, subtract and multiply (dot product) operators.
         *   - The `Matrix` class defines multiplication rules with this class.
         *   - Defined index ([]) operator with range check.
         * 
         * */
        template <typename T> class Vector {
            static_assert(std::is_default_constructible<T>::value, "Type T has to be default constructible.");
        private:
            std::size_t length;
            std::unique_ptr<T[]> storage;
        public:
            Vector(const std::size_t& length): length {length}, storage {std::make_unique<T[]>(length)} {}
            Vector(const std::vector<T>& elems): length {elems.size()}, storage {std::make_unique<T[]>(elems.size())} {
                auto it {elems.begin()};
                for (std::size_t i = 0; i < elems.size(); i++, it++) {
                    storage[i] = *it;
                }
            }
            Vector(Vector<T>&& other): length {other.length}, storage {std::move(other.storage)} {
                other.length = 0;
                other.storage = nullptr;
            }
            
            /**
             * \brief Length of the vector
             * 
             * This function returns the length (number of dimensions) of the vector. Alailable for both const and non const objects / references.
             * Calling this dims would be better but I went for parity with `std::vector` for some reason.
             * 
             * \return: Length of the vector (std::size_t)
             * */
            const std::size_t& size() const {
                return length;
            }
            
            const std::size_t& size() {
                return length;
            }
            
            /**
             * \brief Modulus (length) of the vector
             * 
             * This function returns the modulus (length) of the vector. Alailable for both const and non const objects / references.
             * 
             * \return: Length of the vector (std::size_t)
             * */
            double mod() const {
                T acc {static_cast<T>(0)};
                for (std::size_t i = 0; i < length; i++) {
                    acc += storage[i]*storage[i];
                }
                return std::sqrt(static_cast<double>(acc));
            }
            
            /**
             * \brief Scale the vector
             * 
             * This function multiplies each element of the vector by a scalar. In place scaling is less wsteful as compared to generating
             * a new object. Hence this is not provided as an override to multiplication operator.
             * 
             * \param scalar: The factor to scale by.
             * 
             * \return: A reference to this.
             * */
            Vector<T>& scale(const T& scalar) {
                for (std::size_t i = 0; i < length; i++) {
                    storage[i] *= scalar;
                }
                return *this;
            }
            
            double mod() {
                T acc {static_cast<T>(0)};
                for (std::size_t i = 0; i < length; i++) {
                    acc += storage[i]*storage[i];
                }
                return std::sqrt(static_cast<double>(acc));
            }
            
            const T& operator[](const std::size_t& index) const {
                if (index >= length) {
                    throw std::out_of_range("Vector index out of range.");
                }
                return storage[index];
            }
            
            T& operator[](const std::size_t& index) {
                if (index >= length) {
                    throw std::out_of_range("Vector index out of range.");
                }
                return storage[index];
            }
            
            const T* begin() const {
                return &storage[0];
            }
            
            T* begin() {
                return &storage[0];
            }
            
            const T* end() const {
                return &storage[length];
            }
            
            T* end() {
                return &storage[length];
            }
        };
        
        template <typename T> T operator*(const Vector<T>& lhs, const Vector<T>& rhs) {
            if (lhs.size() != rhs.size()) {
                throw std::invalid_argument("Cannot compute dot product of vectors with different dimensions.");
            }
            T acc {static_cast<T>(0)};
            for (std::size_t i = 0; i < lhs.size(); i++) {
                acc += (lhs[i] * rhs[i]);
            }
            return acc;
        }
        
        template <typename T> Vector<T> operator+(const Vector<T>& lhs, const Vector<T>& rhs) {
            if (lhs.size() != rhs.size()) {
                throw std::invalid_argument("Cannot compute dot product of vectors with different dimensions.");
            }
            Vector<T> retval {lhs.size()};
            for (std::size_t i = 0; i < lhs.size(); i++) {
                retval[i] = lhs[i] + rhs[i];
            }
            return retval;
        }
        
        template <typename T> Vector<T> operator-(const Vector<T>& lhs, const Vector<T>& rhs) {
            if (lhs.size() != rhs.size()) {
                throw std::invalid_argument("Cannot compute dot product of vectors with different dimensions.");
            }
            Vector<T> retval {lhs.size()};
            for (std::size_t i = 0; i < lhs.size(); i++) {
                retval[i] = lhs[i] - rhs[i];
            }
            return retval;
        }
    };
}

#endif

