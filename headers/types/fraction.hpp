#ifndef __SIGABRT_TYPE_FRACTION__
#define __SIGABRT_TYPE_FRACTION__

#include <exception>
#include <iostream>
#include <type_traits>

/**
 * \namespace Sigabrt
 * 
 * \brief The root namespace.
 * */
namespace Sigabrt {
    
    /**
     * \namespace Sigabrt::Types
     * 
     * \brief The root namespace.
     * */
    namespace Types {
        namespace {
            long validateInputsAndReduce(const long& num, const long& den) {
                if (den == 0) {
                    throw std::invalid_argument("Denominator cannot be 0.");
                }
                if (num == 0) {
                    return 1;
                }
                if (num==den) {
                    return num;
                }
                long n1 {num < 0? -num : num};
                long n2 {den < 0? -den : den};

                while (n1 >= 1 && n2 >= 1) {
                    if (n2 > n1) {
                    std::swap(n1, n2);
                    }
                    if (n1%n2 == 0) {
                    return n2;
                    }
                    long tmp = n2;
                    n2 = n1 - n2;
                    n1 = tmp;
                }
                return n2;
            }
        }
        struct Fraction {
        private:
            Fraction(const long& _gcd, const long& num, const long& den): num {num/_gcd}, den {den/_gcd} {}
        public:
            const long num;
            const long den;
            Fraction(const long& num, const long& den): Fraction(validateInputsAndReduce(num, den), num, den) {}                
            
            operator double() {
                return static_cast<double>(num)/static_cast<double>(den);
            }
            
            operator double() const {
                return static_cast<double>(num)/static_cast<double>(den);
            }
        };
        
        std::ostream& operator<<(std::ostream& stream, const Fraction& f) {
            stream << f.num << '/' << f.den;
            return stream;
        }
        
        
        // Override Add operator
        template <typename T> Fraction operator+(const Fraction& lhs, const T& rhs) {
            static_assert(std::is_integral<T>::value, "Addition operation is supported only on fraction and integral types.");
            return Fraction {
                lhs.num + lhs.den*static_cast<long>(rhs),
                lhs.den
            };
        }
        
        template <typename T> Fraction operator+( const T& lhs, const Fraction& rhs) {
            static_assert(std::is_integral<T>::value, "Addition operation is supported only on fraction and integral types.");
            return rhs + lhs;
        }
        
        Fraction operator+(const Fraction& lhs, const Fraction& rhs) {
            return Fraction {
                lhs.num*rhs.den + rhs.num*lhs.den,
                lhs.den*rhs.den
            };
        }
        
        // Overload negate operator
        Fraction operator-(const Fraction& f) {
            return Fraction {
                -1*f.num,
                f.den
            };
        }
        
        // Override Subtract operator
        template <typename T> Fraction operator-(const Fraction& lhs, const T& rhs) {
            static_assert(std::is_integral<T>::value, "Addition operation is supported only on fraction and integral types.");
            return Fraction {
                lhs.num - lhs.den*static_cast<long>(rhs),
                lhs.den
            };
        }
        
        template <typename T> Fraction operator-( const T& lhs, const Fraction& rhs) {
            static_assert(std::is_integral<T>::value, "Addition operation is supported only on fraction and integral types.");
            return -rhs + lhs;
        }
        
        Fraction operator-(const Fraction& lhs, const Fraction& rhs) {
            return Fraction {
                lhs.num*rhs.den - rhs.num*lhs.den,
                lhs.den*rhs.den
            };
        }
        
        
        // Override Multiply operator
        template <typename T> Fraction operator*(const Fraction& lhs, const T& rhs) {
            static_assert(std::is_integral<T>::value, "Addition operation is supported only on fraction and integral types.");
            if (lhs.den == 0L) {
                throw std::invalid_argument("Multiplication with invalid fraction (0 denominator).");
            }
            return Fraction {
                lhs.num * static_cast<long>(rhs),
                lhs.den
            };
        }
        
        template <typename T> Fraction operator*( const T& lhs, const Fraction& rhs) {
            static_assert(std::is_integral<T>::value, "Addition operation is supported only on fraction and integral types.");
            if (rhs.den == 0L) {
                throw std::invalid_argument("Multiplication with invalid fraction (0 denominator).");
            }
            return rhs * lhs;
        }
        
        Fraction operator*(const Fraction& lhs, const Fraction& rhs) {
            if (rhs.den == 0L || lhs.den == 0) {
                throw std::invalid_argument("Multiplication with invalid fraction (0 denominator).");
            }
            return Fraction {
                lhs.num*rhs.num,
                lhs.den*rhs.den
            };
        }
        
        // Override Divide operator
        template <typename T> Fraction operator/(const Fraction& lhs, const T& rhs) {
            static_assert(std::is_integral<T>::value, "Addition operation is supported only on fraction and integral types.");
            if (rhs == static_cast<T>(0)) {
                throw std::invalid_argument("Attempt to divide by 0.");
            }
            return Fraction {
                lhs.num,
                lhs.den*static_cast<long>(rhs)
            };
        }
        
        template <typename T> Fraction operator/( const T& lhs, const Fraction& rhs) {
            static_assert(std::is_integral<T>::value, "Addition operation is supported only on fraction and integral types.");
            if (rhs.num == 0L) {
                throw std::invalid_argument("Attempt to divide by 0.");
            }
            return Fraction {
                rhs.den*lhs,
                rhs.num
            };
        }
        
        Fraction operator/(const Fraction& lhs, const Fraction& rhs) {
            if (rhs.num == 0L) {
                throw std::invalid_argument("Attempt to divide by 0.");
            }
            return Fraction {
                lhs.num*rhs.den,
                lhs.den*rhs.num
            };
        }
        
        
        // Override Equality operator
        template <typename T> bool operator==(const Fraction& lhs, const T& rhs) {
            return static_cast<double>(lhs) == static_cast<double>(rhs);
            
        }
        
        template <typename T> bool operator==(const T& lhs, const Fraction& rhs) {
            return static_cast<double>(lhs) == static_cast<double>(rhs);
        }
        
        
        bool operator==(const Fraction& lhs, const Fraction& rhs) {
            return lhs.num == rhs.num && lhs.den == rhs.den;
        }
        
        // Override Not Equals operator
        template <typename T> bool operator!=(const Fraction& lhs, const T& rhs) {
            return static_cast<double>(lhs) != static_cast<double>(rhs);
            
        }
        
        template <typename T> bool operator!=(const T& lhs, const Fraction& rhs) {
            return static_cast<double>(lhs) != static_cast<double>(rhs);
        }
        
        
        bool operator!=(const Fraction& lhs, const Fraction& rhs) {
            return lhs.num != rhs.num || lhs.den != rhs.den;
        }
        
        // Override Greater than  operator
        template <typename T> bool operator>(const Fraction& lhs, const T& rhs) {
            return static_cast<double>(lhs) > static_cast<double>(rhs);
            
        }
        
        // Override Less than  operator
        template <typename T> bool operator<(const Fraction& lhs, const T& rhs) {
            return static_cast<double>(lhs) < static_cast<double>(rhs);
            
        }
        
        // Override Greater than eq operator
        template <typename T> bool operator>=(const Fraction& lhs, const T& rhs) {
            return static_cast<double>(lhs) > static_cast<double>(rhs);
            
        }
        
        // Override Greater than eq operator
        template <typename T> bool operator<=(const Fraction& lhs, const T& rhs) {
            return static_cast<double>(lhs) < static_cast<double>(rhs);
        }
        
    }
}

#endif
