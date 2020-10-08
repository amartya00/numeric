#include <numeric/types/models.hpp>
#include <numeric/types/vector.hpp>
#include <numeric/math/rref.hpp>
#include <numeric/types/fraction.hpp>
#include <numeric/types/plane.hpp>
#include <numeric/benchmark/benchmark.hpp>
#include <numeric/math/vectorspaces.hpp>

#include <iostream>
#include <vector>
#include <iterator>
#include <type_traits>

using Sigabrt::Types::Fraction;
using Sigabrt::Types::Vector;
using Sigabrt::Types::Plane;
using Sigabrt::Types::IsScalarType;
using Sigabrt::Numeric::isNormalToPlane;
using Sigabrt::Numeric::cross;

int main() {
    Plane<double> p1 {3,5,9,-26};
    Vector<int> v1 {{3,5,9}};
    Vector<int> v2 {{-3,-5,9}};
    Vector<int> v3 {{3,-5,-9}};
    Vector<int> v4 {{-3,-5,-9}};
    
    std::cout << std::boolalpha << *isNormalToPlane(p1, v1).val << "\n";
    std::cout << std::boolalpha << *isNormalToPlane(p1, v2).val << "\n";
    std::cout << std::boolalpha << *isNormalToPlane(p1, v3).val << "\n";
    std::cout << std::boolalpha << *isNormalToPlane(p1, v4).val << "\n";
    
    Plane<double> p2 {Vector<double>{{-4,-3,9}}, Vector<double>{{-5,3,-3}}};
    auto [a,b,c,k] = p2.getCoefficients();
    std::cout << a << "x + " << b << "y + " << c << "z = " << k << "\n"; 

    Vector<double> v11 {{6,7,-5}};
    Vector<double> v12 {{8, 7, -11}};
    Vector<double> v13 {*cross(v11, v12).val};
    std::cout << v13[0] << "," << v13[1] << "," << v13[2] << "\n";
}
