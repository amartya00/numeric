#include <types/models.hpp>
#include <math/rref.hpp>
#include <types/fraction.hpp>
#include <benchmark/benchmark.hpp>
#include <iostream>
#include <vector>
#include <iterator>

using Sigabrt::Types::Fraction;

int main() {
    Fraction f1 {1,2};
    Fraction f2 {1,2};
    Fraction sum {f1 + 5};
    Fraction sum2 {f1 + f2};
    Fraction sum3 {5 + f1};
    std::cout << sum << "\n";
    std::cout << sum2 << "\n";
    std::cout << sum3 << "\n";
    
    std::cout << std::boolalpha << (f1 == 0.5) << "\n";
    std::cout << std::boolalpha << (f1 > f2) << "\n";
    std::cout << static_cast<double>(f1 * f2) << "\n";
}
