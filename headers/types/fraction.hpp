#ifndef __SIGABRT_TYPE_FRACTION__
#define __SIGABRT_TYPE_FRACTION__

#include <exception>

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
        class Fraction {
        private:
            long num {1L};
            long den {1L};
        public:
            Fraction(const long& num, const long& den): num {num}, den {den} {
                if (den == 0) {
                    throw std::invalid_argument("Denominator cannot be 0.");
                }
            }
            
            Fraction(const long& num): num {num}, den {1L} {}
            
            // Overloaded arithmetic operators
            Fraction operator+(const Fraction& rhs) const {
                return Fraction {
                    num*rhs.den + rhs.num*den,
                    den*rhs.den
                };
            }
            Fraction operator+(const Fraction& rhs) {
                return Fraction {
                    num*rhs.den + rhs.num*den,
                    den*rhs.den
                };
            }
            Fraction operator*(const Fraction& rhs) const {
                return Fraction {
                    num*rhs.num,
                    den*rhs.den
                };
            }
            Fraction operator*(const Fraction& rhs) {
                return Fraction {
                    num*rhs.num,
                    den*rhs.den
                };
            }
            Fraction operator/(const Fraction& rhs) const {
                if (rhs.num == 0) {
                    throw std::invalid_argument("RHS cannot be 0.");
                }
                return Fraction {
                    num*rhs.num,
                    den*rhs.den
                };
            }
            Fraction operator/(const Fraction& rhs) {
                if (rhs.num == 0) {
                    throw std::invalid_argument("RHS cannot be 0.");
                }
                return Fraction {
                    num*rhs.num,
                    den*rhs.den
                };
            }
            
            // Overloaded equality operator
            bool operator==(const Fraction& rhs) const {
                return num == rhs.num && den == rhs.den;
            }
            bool operator==(const Fraction& rhs) {
                return num == rhs.num && den == rhs.den;
            }
            bool operator!=(const Fraction& rhs) const {
                return num != rhs.num || den != rhs.den;
            }
            bool operator!=(const Fraction& rhs) {
                return num != rhs.num || den != rhs.den;
            }
            
            // Overloaded inequality operators
            bool operator>(const Fraction& rhs) const {
                return static_cast<double>(num)/static_cast<double>(den) > static_cast<double>(rhs.num)/static_cast<double>(rhs.den);
            }
            bool operator>(const Fraction& rhs) {
                return static_cast<double>(num)/static_cast<double>(den) > static_cast<double>(rhs.num)/static_cast<double>(rhs.den);
            }
            
            bool operator<(const Fraction& rhs) const {
                return static_cast<double>(num)/static_cast<double>(den) < static_cast<double>(rhs.num)/static_cast<double>(rhs.den);
            }
            bool operator<(const Fraction& rhs) {
                return static_cast<double>(num)/static_cast<double>(den) < static_cast<double>(rhs.num)/static_cast<double>(rhs.den);
            }
            
            bool operator>=(const Fraction& rhs) const {
                return static_cast<double>(num)/static_cast<double>(den) >= static_cast<double>(rhs.num)/static_cast<double>(rhs.den);
            }
            bool operator>=(const Fraction& rhs) {
                return static_cast<double>(num)/static_cast<double>(den) >= static_cast<double>(rhs.num)/static_cast<double>(rhs.den);
            }
            
            bool operator<=(const Fraction& rhs) const {
                return static_cast<double>(num)/static_cast<double>(den) <= static_cast<double>(rhs.num)/static_cast<double>(rhs.den);
            }
            bool operator<=(const Fraction& rhs) {
                return static_cast<double>(num)/static_cast<double>(den) <= static_cast<double>(rhs.num)/static_cast<double>(rhs.den);
            }
        };
        
        
    }
}

#endif
