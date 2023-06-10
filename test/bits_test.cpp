
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
    SHOULD("parse bit literals with correct length") {
        expect_eq(bits_t("b110011").size(), 6);
        expect_eq(bits_t("b0").size(), 1);
        expect_eq(bits_t("x1").size(), 4);
        expect_eq(bits_t("x10").size(), 8);
        expect_eq(bits_t("xFFFF").size(), 16);
        expect_eq(bits_t("o1").size(), 3);
        expect_eq(bits_t("o11").size(), 6);
        expect_eq(bits_t("o111").size(), 9);
        expect_eq(bits_t("o1111").size(), 12);
    }
}