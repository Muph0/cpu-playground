
#include "test.hpp"
#include "types.hpp"

TEST_SUITE(bits_test) {
    SHOULD("mask down excess bits") {
        bits_t b(0, 4);
        b = 17;
        expect_eq(b, 1);
    }
}