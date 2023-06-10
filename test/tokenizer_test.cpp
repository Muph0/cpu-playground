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
}
