
#include "test.hpp"
#include "types.hpp"

TEST_SUITE(bits_test) {
    SHOULD("mask down excess bits") {
        bits_t b = 4_bit(0);
        b = 17;
        expect_eq(b, 1);
    }
    SHOULD("overflow postfix") {
        bits_t b = 4_bit(15);
        expect_eq(b++, 15);
        expect_eq(b, 0);
    }
    SHOULD("underflow prefix") {
        bits_t b = 4_bit(0);
        expect_eq(--b, 15);
        expect_eq(--b, 14);
    }
    SHOULD("map negative to 2s complement") {
        expect_eq(4_bit(-1), 15);
        expect_eq(4_bit(1) * -1, 15);
        expect_eq(8_bit(-1), 255);
        expect_eq(8_bit(1) * -1, 255);
        expect_eq(8_bit(-10), -10);
        expect_eq(8_bit(-77), -77);
    }
}