#include <string>
#include <vector>
#include <iostream>

#include "formatters.hpp"
#include "instr.hpp"
#include "test.hpp"
#include "token.hpp"

using namespace Asm;
using Tok = Token::Type;

static bits_t parse_whole(Match parser, const char* text) {
    bits_t result;
    auto tokens = tokenize(text);

    std::cout << std::format("Tokens: {}", tokens) << std::endl;

    InPtr start = &tokens.front();
    InPtr end = &tokens.back() + 1;

    bool ok = parser.parse(start, end, result);

    if (!ok) throw std::logic_error(std::string("Parsing failed:\n") + text);

    return result;
}

TEST_SUITE(test_parser) {
    SHOULD("match token by token type") {  //
        Match ident(Token::Type::IDENT, "b101");
        test_assert(parse_whole(ident, "mov") == 5, "Fail");
    }
    SHOULD("match token by token text") {  //
        Match hello("hello", "b111");
        test_assert(parse_whole(hello, "hello ") == 7, "Fail");
    }
}
