#ifndef __SIGABRT_NUMERIC_VECTOR__
#define __SIGABRT_NUMERIC_VECTOR__

#include <cmath>
#include <exception>
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <vector>

#include <numeric/types/models.hpp>

/**
 * \namespace numeric
 * 
 * \brief The root namespace.
 * */
namespace numeric {
    /**
     * \namespace numeric::types
     * 
     * \brief The namespace containing some special types.
     * */
    namespace types {
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
            double magnitude {-1.0};
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
            
            void operator=(Vector<T>&& other){
                length = other.length;
                storage = std::move(other.storage); // Assignment will release and reset.
                other.length = 0;
                other.storage = nullptr;
            }
            
            // Delete the copy constructor and copy asignment as copying is usually a bad idea
            Vector(const Vector<T>& other)=delete;
            void operator=(const Vector<T>& other)=delete;
            
            
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
            
            //! \cond NO_DOC
            const std::size_t& size() {
                return length;
            }
            //! \endcond
            
            /**
             * \brief Modulus (length) of the vector
             * 
             * This function returns the magnitude (length) squared of the vector. Alailable for both const and non const objects / references.
             * 
             * \return: The magnitude.
             * */
            double mod() const {
                T acc {static_cast<T>(0)};
                for (std::size_t i = 0; i < length; i++) {
                    acc += storage[i]*storage[i];
                }
                return static_cast<double>(acc);
            }
            
            //! \cond NO_DOC
            double mod() {
                if(magnitude < 0) {
                    T acc {static_cast<T>(0)};
                    for (std::size_t i = 0; i < length; i++) {
                        acc += storage[i]*storage[i];
                    }
                    magnitude = static_cast<double>(acc);
                    
                } 
                return magnitude;
            }
            //! \endcond
            
            /**
             * \brief Scale the vector
             * 
             * This function multiplies each element of the vector by a scalar. In place scaling is less wsteful as compared to generating
             * a new object. Hence this is not provided as an override to multiplication operator.
             * 
             * \param scalar: The factor to scale by.
             * 
             * \return A reference to this.
             * */
            Vector<T>& scale(const T& scalar) {
                for (std::size_t i = 0; i < length; i++) {
                    storage[i] *= scalar;
                }
                return *this;
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
            
            template <typename U, typename V> friend Vector<U> operator*(const V& scalar, const Vector<U>& rhs);
            template <typename U, typename V> friend Vector<U> operator*(const Vector<U>& lhs, const V& scalar);
        };
        
        /**
         * \tparam T The vector type.
         * 
         * \brief Multiplication operator overload for 2 vectors.
         * 
         * This overloads the multiply operator to perform dot product of 2 vectors. You can do double `dotProd {v1*v2}`. Note 
         * that the vectors do not have to be of same type as long as they can be multiplied.
         * 
         * \param lhs v1 vector.
         * 
         * \param rhs v2 vector.
         * */
        template <typename T, typename U> T operator*(const Vector<T>& lhs, const Vector<U>& rhs) {
            if (lhs.size() != rhs.size()) {
                throw std::invalid_argument("Cannot compute dot product of vectors with different dimensions.");
            }
            T acc {static_cast<T>(0)};
            for (std::size_t i = 0; i < lhs.size(); i++) {
                acc += (lhs[i] * rhs[i]);
            }
            return acc;
        }
        
        /**
         * \tparam T The vector type.
         * 
         * \brief Multiplication operator overload for a vector with a scalar.
         * 
         * This overloads the multiply operator to perform an out of place scaling of a vector. You can do Vector<double> scaled {2*v1}
         * 
         * Take note that this returns a new vector. So for scenarios, where you have to update 
         * a vector like `v1 = 2 * v1` just use `v1.scale(2)` instead. That will scale v1 in place 
         * and not waste time copying.
         * 
         * \param scalar A scalar.
         * 
         * \param rhs v1 vector.
         * */
        template <typename T, typename U> Vector<T> operator*(const U& scalar, const Vector<T>& rhs) {
            Vector<T> res {rhs.size()};
            for (std::size_t i = 0; i < rhs.size(); i++) {
                res[i] = scalar*rhs[i];
            }
            res.magnitude = scalar*rhs.magnitude;
            return res;
        }
        
        /**
         * \tparam T The vector type.
         * 
         * \brief Multiplication operator overload for a vector with a scalar.
         * 
         * This overloads the multiply operator to perform an out of place scaling of a vector. You can do Vector<double> scaled {v1*2}
         * 
         * Take note that this returns a new vector. So for scenarios, where you have to update 
         * a vector like `v1 = v1*2` just use `v1.scale(2)` instead. That will scale v1 in place 
         * and not waste time copying.
         * 
         * \param scalar A scalar.
         * 
         * \param lhs v1 vector.
         * */
        template <typename T,typename U> Vector<T> operator*(const Vector<T>& lhs, const U& scalar) {
            Vector<T> res {lhs.size()};
            for (std::size_t i = 0; i < lhs.size(); i++) {
                res[i] = scalar*lhs[i];
            }
            res.magnitude = scalar*lhs.magnitude;
            return res;
        }
        
        /**
         * \tparam T The vector type.
         * 
         * \brief Addition operator overload for 2 vectors.
         * 
         * This overloads the addition operator to perform sum of 2 vectors. You can do double `Vector<double> sum {v1+v2}`.
         * 
         * \param lhs v1 vector.
         * 
         * \param rhs v2 vector.
         * */
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
        
        /**
         * \tparam T The vector type.
         * 
         * \brief Subtraction operator overload for 2 vectors.
         * 
         * This overloads the subtraction operator to perform subtraction of 2 vectors. You can do double `Vector<double> diff {v1-v2}`.
         * 
         * \param lhs v1 vector.
         * 
         * \param rhs v2 vector.
         * */
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
        
        /**
         * \tparam T The vector type.
         * 
         * \brief Negation operator overload for a vector.
         * 
         * This overloads the negation operator negate vectors. You can do double `Vector<double> neg {-v1}`.
         * 
         * \param lhs v1 vector.
         * */
        template <typename T> Vector<T> operator-(const Vector<T>& lhs) {
            Vector<T> retval {lhs.size()};
            for (std::size_t i = 0; i < lhs.size(); i++) {
                retval[i] = -lhs[i];
            }
            return retval;
        }
        
        /**
         * \tparam T One of the vector types.
         * 
         * \tparam U The other vector type.
         * 
         * \brief Equality operator overload for a vector.
         * 
         * This overloads the equality operator to compare of 2 vectors. You can do double `v1 == v2`. Note that the vectors
         * do not have to be of the same type, as long as the elements can be compared.
         * 
         * \param lhs v1 vector.
         * 
         * \param rhs v2 vector.
         * */
        template <typename T, typename U> bool operator==(const Vector<T>& lhs, const Vector<U>& rhs) {
            if (lhs.size() == rhs.size()){
                auto it1 {lhs.begin()};
                auto it2 {rhs.begin()};
                for (; it1 < lhs.end(); it1++, it2++) {
                    if(*it1 != *it2) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }
        
        /**
         * \tparam T One of the vector types.
         * 
         * \tparam U The other vector type.
         * 
         * \brief Inequality operator overload for a vector.
         * 
         * This overloads the inequality operator to compare of 2 vectors. You can do double `v1 != v2`. Note that the vectors
         * do not have to be of the same type, as long as the elements can be compared.
         * 
         * \param lhs v1 vector.
         * 
         * \param rhs v2 vector.
         * */
        template <typename T, typename U> bool operator!=(const Vector<T>& lhs, const Vector<U>& rhs) {
            return !(lhs == rhs);
        }
        
    }
}

#endif
