#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <exception>
#include <vector>
#include <iostream>

#include <catch2/catch.hpp>
#include <types/vector.hpp>

using Sigabrt::Types::Vector;

SCENARIO("Vector instantiation.") {
    
    GIVEN("I want to instantiate vector by various methods.") {
        
        WHEN("I initialize a vector from size.") {
            
            THEN("I should be able to do so.") {
                
                Vector<double> v1 {10};
                REQUIRE(10 == v1.size());
                
                const Vector<double> v2 {10};
                REQUIRE(10 == v2.size());
            }
        }
        
        WHEN("I initialize a vector from std::vector.") {
            
            THEN("I should be able to do so.") {
                
                Vector<double> v1 {{1,2,3,4,5}};
                REQUIRE(5 == v1.size());
                
                Vector<double> v2 {{1,2,3,4,5}};
                REQUIRE(5 == v2.size());
            }
        }
    }
}

SCENARIO("Vector access.") {
    
    GIVEN("I have a vector.") {
        
        const Vector<int> v0 {{1,2,3,4,5,6}};
        Vector<int> v1 {{1,2,3,4,5,6}};
        std::vector<int> v2 {1,2,3,4,5,6};
        
        WHEN("I attempt to access it's elements with index operator.") {
            
            THEN("I should be able to do so.") {
                for (std::size_t i = 0; i < v1.size(); i++) {
                    REQUIRE(v0[i] == v2[i]);
                    REQUIRE(v1[i] == v2[i]);
                }
            }
        }
        
        WHEN("I attempt to access it's elements with iterators.") {
            
            THEN("I should be able to do so.") {
                
                REQUIRE(std::equal(v0.begin(), v0.end(), v2.begin()));
                REQUIRE(std::equal(v1.begin(), v1.end(), v2.begin()));
            }
        }
    }
}

SCENARIO("Vector operations.") {
    
    GIVEN("I have a vector.") {
        
        Vector<double> v1 {{3,4}};
        
        WHEN("I do a mod operation.") {
            
            double mod {v1.mod()};
            
            THEN("I should get the correct mod.") {
                
                REQUIRE(5 == mod);
            }
        }
        
        WHEN("I scale the vector.") {
            
            THEN("I should get the correct result.") {
                
                std::vector<double> expected {300, 400};
                v1.scale(100);
                REQUIRE(std::equal(v1.begin(), v1.end(), expected.begin()));
                REQUIRE(500 == v1.mod());
            }
        }
    }
    
    GIVEN("I have 2 compatible vectors.") {
        
        Vector<double> v1 {{1,1}};
        Vector<double> v2 {{2,2}};
        
        WHEN("I add them.") {
            
            THEN("I get the expected result.") {
                
                std::vector<double> expected {{3,3}};
                Vector<double> v3 {v1 + v2};
                REQUIRE(std::equal(v3.begin(), v3.end(), expected.begin()));
            }
        }
        
        WHEN("I subtract them.") {
            
            THEN("I get the expected result.") {
                
                std::vector<double> expected {{-1,-1}};
                Vector<double> v3 {v1 - v2};
                REQUIRE(std::equal(v3.begin(), v3.end(), expected.begin()));
            }
        }
        
        WHEN("I dot product them.") {
            
            THEN("I get the expected result.") {
                
                REQUIRE(4 == v1*v2);
            }
        }
    }
    
    GIVEN("I have 2 incompatible vectors.") {
        
        Vector<double> v1 {{1,1}};
        Vector<double> v2 {{2,2, 2}};
        
        WHEN("I add them.") {
            
            THEN("I get an exception.") {
                
                REQUIRE_THROWS_AS(v1 + v2, std::invalid_argument);
            }
        }
        
        WHEN("I subtract them.") {
            
            THEN("I get an exception.") {
                
                REQUIRE_THROWS_AS(v1 - v2, std::invalid_argument);
            }
        }
        
        WHEN("I dot product them.") {
            
            THEN("I get an exception.") {
                
                REQUIRE_THROWS_AS(v1 * v2, std::invalid_argument);
            }
        }
    }
}

