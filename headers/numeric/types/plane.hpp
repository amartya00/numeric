#ifndef __SIGABRT_NUMERIC_PLANE__
#define __SIGABRT_NUMERIC_PLANE__

#include <exception>
#include <tuple>

#include <numeric/types/vector.hpp>

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
        
        template <typename T> class Plane {
        private:
            Sigabrt::Types::Vector<T> normal;
            Sigabrt::Types::Vector<T> point;
            std::tuple<T, T, T, T> coefficients;
        public:
            Plane(const T& a, const T& b, const T& c, const T& k) : normal {3}, point {3}, coefficients {std::make_tuple(a,b,c,k)} {
                if (a == static_cast<T>(0) && b == static_cast<T>(0) && c == static_cast<T>(0)) {
                    throw std::invalid_argument("A plane of the form ax + by + cz = K, cannot have a=0 and b=0 and c=0.");
                }
                normal[0] = a;
                normal[1] = b;
                normal[2] = c;
                
                if(a != static_cast<T>(0)) {
                    point[0] = k/a;
                    point[1] = static_cast<T>(0);
                    point[2] = static_cast<T>(0);
                } else if (b != static_cast<T>(0)) {
                    point[0] = static_cast<T>(0);
                    point[1] = k/b;
                    point[2] = static_cast<T>(0);
                } else if (c != static_cast<T>(0)) {
                    point[0] = static_cast<T>(0);
                    point[1] = static_cast<T>(0);
                    point[2] = k/c;
                }
            }
            
            template <typename U, typename V> 
            Plane(const Sigabrt::Types::Vector<U>& normal, const Sigabrt::Types::Vector<V>& point): normal {3}, point {3} {
                if (normal.size() != 3 || point.size() != 3) {
                    throw std::invalid_argument("Vectors representing planes normals and points have to be of dimension 3.");
                }
                this -> normal[0] = normal[0];
                this -> normal[1] = normal[1];
                this -> normal[2] = normal[2];
                
                this -> point[0] = point[0];
                this -> point[1] = point[1];
                this -> point[2] = point[2];
                
                coefficients = std::make_tuple(
                    static_cast<T>(normal[0]),
                    static_cast<T>(normal[1]),
                    static_cast<T>(normal[2]),
                    static_cast<T>(normal*point)
                );
            }
            
            const Sigabrt::Types::Vector<T>& getNormal() const {
                return normal;
            }
            
            const Sigabrt::Types::Vector<T>& getNormal() {
                return normal;
            }
            
            const Sigabrt::Types::Vector<T>& getPoint() const {
                return point;
            }
            
            const Sigabrt::Types::Vector<T>& getPoint() {
                return point;
            }
            
            const std::tuple<T,T,T,T>& getCoefficients() const {
                return coefficients;
            }
            
            const std::tuple<T,T,T,T>& getCoefficients() {
                return coefficients;
            }
        };
        
    }
}

#endif
