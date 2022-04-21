#ifndef __SIGABRT_NUMERIC_PLANE__
#define __SIGABRT_NUMERIC_PLANE__

#include <exception>
#include <tuple>

#include <numeric/types/vector.hpp>

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
         * \class Plane
         * 
         * \tparam T The type of variable.
         * 
         * \brief The type representing a plane.
         * 
         * This class represents a plane (as in linear algebra), of type `T`. While planes where the variables x,y,z and k can 
         * only tage integral values do not make sense in convention, this is templatized to make it more generic. This class
         * provides for easy translation between the linear form `ax + by + cz = k` and the point-normal form `N.(X-X0) = 0`
         * 
         * */
        template <typename T> class Plane {
        private:
            numeric::types::Vector<T> normal;
            numeric::types::Vector<T> point;
            std::tuple<T, T, T, T> coefficients;
        public:
            /**
             * 
             * */
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
            Plane(const numeric::types::Vector<U>& normal, const numeric::types::Vector<V>& point): normal {3}, point {3} {
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
            
            const numeric::types::Vector<T>& get_normal() const {
                return normal;
            }

            const numeric::types::Vector<T>& get_point() const {
                return point;
            }
            
            const std::tuple<T,T,T,T>& get_coefficients() const {
                return coefficients;
            }
        };
        
    }
}

#endif
