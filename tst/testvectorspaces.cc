#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <exception>
#include <vector>
#include <iostream>
#include <optional>

#include <catch2/catch.hpp>
#include <types/vector.hpp>
#include <types/models.hpp>
#include <math/vectorspaces.hpp>

using Sigabrt::Types::Vector;
using Sigabrt::Types::Result;
using Sigabrt::Types::ErrorCode;
using Sigabrt::Types::OperationType;
using Sigabrt::Numeric::areLinearlyDependent;

SCENARIO("Testing linear dependence of vectors.") {
    
    GIVEN("I have 3 vectors.") {
        
        Vector<double> v1 {{1.0, 2.0}};
        Vector<int> v2 {{-1, -2}};
        Vector<long> v3 {{1L, 55L}};
        
        WHEN("I test v1 and v2 for linear dependence.") {
            
            THEN("They should evaluate to dependent.") {
                
                Result<bool, ErrorCode> res {areLinearlyDependent(v1, v2)};
                REQUIRE(OperationType::OK == res.type);
                REQUIRE(*res.val);
                REQUIRE(std::nullopt == res.error);
                REQUIRE(std::nullopt == res.message);
            }
        }
        
        WHEN("I test v1 and v3 for linear dependence.") {
            
            THEN("They they should evaluate to linearly independent.") {
                
                Result<bool, ErrorCode> res {areLinearlyDependent(v1, v3)};
                REQUIRE(OperationType::OK == res.type);
                REQUIRE(!*res.val);
                REQUIRE(std::nullopt == res.error);
                REQUIRE(std::nullopt == res.message);
            }
        }
        
        WHEN("I test 2 vectors of unequal dimensions for linear dependence.") {
            
            Vector<int> v1 {{1,2,3}};
            Vector<int> v2 {{1,2}};
            Result<bool, ErrorCode> res {areLinearlyDependent(v1, v2)};
            
            THEN("The result should be an error.") {
                REQUIRE(OperationType::ERR == res.type);
                REQUIRE(std::nullopt == res.val);
                REQUIRE(ErrorCode::INCOMPATIBLE_VECTORS == *res.error);
                REQUIRE(
                    std::string(
                        "Cannot check linear independence of 2 vectors of unequal dimensions."
                    ).compare(*res.message) == 0
                );
            }
        }
    }
}
