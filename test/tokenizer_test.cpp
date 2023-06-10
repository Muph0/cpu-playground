#include <string>
#include <vector>

#include "formatters.hpp"
#include "test.hpp"
#include "token.hpp"

using namespace std;
using namespace Asm;
using Type = Token::Type;

static void run_test(const string& input, vector<Type> expected) {
    auto tokens = Asm::tokenize(input);
    bool same = tokens.size() == expected.size();

    for (size_t i = 0; same && i < tokens.size(); ++i) {
        same = tokens[i].type == expected[i];
    }

    expect(same, "Expected {},\ngot {}.", expected, tokens);
}

static void match_integer(const string& input, uint64_t expected) {
    run_test(input, { Type::INT });
    auto token = Asm::tokenize(input).front();
    expect_eq(token.number, expected);
}

TEST_SUITE(tokenizer_test) {
    SHOULD("parse basic instruction") {
        run_test("add 1, 1",
                 { Type::IDENT, Type::INT, Type::COMMA, Type::INT });
    }
    SHOULD("parse brackets") {
        run_test("hello[1](1)1",
                 { Type::IDENT, Type::OPEN_BRACE, Type::INT, Type::CLOSE_BRACE,
                   Type::OPEN_PAREN, Type::INT, Type::CLOSE_PAREN, Type::INT });
    }
    SHOULD("skip whitespace") {  //
        run_test("        ", {});
    }
    SHOULD("match decimal integer") {
        match_integer("0", 0);
        match_integer("10", 10);
        match_integer("12345", 12345);
    }
    SHOULD("match binary integer") {
        match_integer("0b0", 0b0);
        match_integer("0b10", 0b10);
        match_integer("0b1101", 0b1101);
    }
    SHOULD("match hex integer") {
        match_integer("0x0", 0x0);
        match_integer("0x10", 0x10);
        match_integer("0xdeadbeef", 0xdeadbeef);
    }
    SHOULD("match oct integer") {
        match_integer("00", 0);
        match_integer("07", 07);
        match_integer("0100", 64);
        match_integer("012345", 012345);
    }
}
