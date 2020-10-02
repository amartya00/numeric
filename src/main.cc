#include <types/models.hpp>
#include <types/vector.hpp>
#include <math/rref.hpp>
#include <types/fraction.hpp>
#include <benchmark/benchmark.hpp>
#include <math/vectorspaces.hpp>
#include <iostream>
#include <vector>
#include <iterator>
#include <type_traits>

using Sigabrt::Types::Fraction;
using Sigabrt::Types::Vector;
using Sigabrt::Types::IsScalarType;

int main() {
    Vector<int> x {{-4, -2, 7}};
    Vector<int> y {{6, -1, -10}};
    Vector<int> z {{3, -2, 0}};
    
    int res {3*x*(-2*y -z)};
    
    
    std::cout << res << "\n";
    
    std::cout << std::boolalpha << IsScalarType<int>::value << "\n";
    std::cout << std::boolalpha << IsScalarType<Fraction>::value << "\n";
    std::cout << std::boolalpha << IsScalarType<Vector<int>>::value << "\n";
    std::cout << std::boolalpha << std::is_arithmetic<int>::value << "\n";
}
