#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <exception>
#include <vector>
#include <iostream>
#include <map>
#include <random>

#include <catch2/catch.hpp>
#include <numeric/types/models.hpp>
#include <numeric/benchmark/benchmark.hpp>

using Sigabrt::Types::RunInfo;
using Sigabrt::Benchmark::Benchmark;

double max(const std::vector<double>& arr) {
    double maxItem {arr[0]};
    for (const double& elem : arr) {
        if (elem > maxItem) {
            maxItem = elem;
        }
    }
    return maxItem;
}
        
std::vector<double> genInput(const unsigned long& inputSize) {
    std::random_device rd;
    std::mt19937 mt(rd());  
    std::uniform_real_distribution<double> dist(1.0, 1000.0);
    std::vector<double> retval {};
    retval.reserve(inputSize);
    for (unsigned long i = 0UL; i < inputSize; i++) {
        retval.push_back(dist(mt));
    }
    return retval;
}

SCENARIO("Benchmark runner.") {
    
    GIVEN("I have a function that I want to benchmark, and an input generator.") {
        
        AND_GIVEN("I have a list of run infos.") {
            
            std::vector<RunInfo> inputs {
                {100UL, 1000UL},
                {200UL, 1000UL},
                {300UL, 1000UL},
                {400UL, 2000UL}
            };
            
            WHEN("I create a benchmark instance and run it.") {
                
                Benchmark<std::vector<double>, double, decltype(inputs.begin())> bm {
                    genInput,
                    max,
                    inputs.begin(),
                    inputs.end()
                };
                
                bm.run();
                
                THEN("I should get back the run times in a map.") {
                    
                    std::map map = bm.getRunInfos();
                    
                    REQUIRE(inputs.size() == map.size());
                    
                    auto it1 = inputs.begin();
                    auto it2 = map.begin();
                    for (; it1 < inputs.end(); it1 ++, it2++) {
                        REQUIRE(it1->inputSize == it2->first);
                        REQUIRE(it1->iterations == it2->second.iterations);
                        REQUIRE(it2->second.runTimeInMillis > 0);
                    }
                }
            }
        }
    }
}
