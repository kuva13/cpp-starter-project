#include "catch.hpp"
#include "lib.hpp"
#include "doctest.h"



TEST_CASE("DelayTest") { 
    REQUIRE( Delay == Delay);
    }

TEST_CASE("InitGameTest") { 
    REQUIRE( InitGame == InitGame);
    }

TEST_CASE("AddFrontTest") { 
    REQUIRE( AddFront == AddFront);
    }