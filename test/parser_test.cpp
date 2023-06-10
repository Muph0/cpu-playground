#include <iostream>
#include <string>
#include <vector>

#include "formatters.hpp"
#include "instr.hpp"
#include "test.hpp"
#include "token.hpp"

using namespace Asm;
using Tok = Token::Type;

static bits_t assert_parses(Match parser, const char* text) {
    bits_t result;
    auto tokens = tokenize(text);

    std::cout << std::format("Tokens: {}", tokens) << std::endl;

    InPtr start = &tokens.front();
    InPtr end = &tokens.back() + 1;

    bool ok = parser.parse(start, end, result);

    if (!ok) throw std::logic_error(std::string("Parsing failed:\n") + text);

    return result;
}

template <typename Exc>
static void assert_parsing_throws(Match parser, const char* text) {}

TEST_SUITE(parser_test) {
    SHOULD("match token by token type") {
        Match ident(Tok::IDENT, "b101");
        expect(assert_parses(ident, "mov") == 5, "Fail");
    }
    SHOULD("match token by token text") {
        Match hello("hello", "b111");
        expect(assert_parses(hello, "hello ") == 7, "Fail");
    }
    SHOULD("match one of") {
        Match any = Match::Any({ "ax", "bx", "cx", "dx" });
        assert_parses(any, "ax");
        assert_parses(any, "bx");
        assert_parses(any, "cx");
        assert_parses(any, "dx");
    }
    SHOULD("match integer") {
        Match any = Match::Integer(8);
        expect_eq(assert_parses(any, "0"), 0);
        expect_eq(assert_parses(any, "100"), 100);
        expect_eq(assert_parses(any, "255"), 255);
        expect_eq(assert_parses(any, "65535"), 255);
        expect_eq(assert_parses(any, "-1"), 255);
    }
    SHOULD("match instruction") {
        auto reg = Match::Any({ { "ax", 2_bit(0) },
                                { "bx", 2_bit(1) },
                                { "cx", 2_bit(2) },
                                { "dx", 2_bit(3) } });

        auto mov = Match::Seq({ Match(Tok::IDENT, "b110011"), "[", reg, "]",
                                ",", Match::Integer(8) });

        expect(assert_parses(mov, "mov [ax], 100") == 0b110011'00 << 8 | 100);
        expect(assert_parses(mov, "mov [bx], -1") == 0b110011'01 << 8 | 0xffff);
        expect(assert_parses(mov, "mov [cx],  85") == 0b110011'10 << 8 | 85);
        expect(assert_parses(mov, "mov [dx],   0") == 0b110011'11 << 8);
    }
}
