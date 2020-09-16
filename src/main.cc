#include <models.hpp>
#include <math/rref.hpp>
#include <types/fraction.hpp>
#include <benchmark/benchmark.hpp>
#include <iostream>
#include <vector>
#include <iterator>

int testF(int x) {
    int acc {0};
    for (int i = 0; i < x; i++) {
        acc += x*x;
    }
    return acc;
}

int inputGen(unsigned long inputSize) {
    return static_cast<int>(inputSize);
}

int main() {
}
