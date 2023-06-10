#include "token.hpp"

#include <cctype>

using namespace Asm;

std::vector<Token> Asm::tokenize(const std::string& text) {
    std::vector<Token> result;

    const char* it = &text.front();
    const char* end = &text.back() + 1;

    while (it != end) {
        auto tok = next_token(it, end);
        if (tok.has_value()) {  //
            result.emplace_back(std::move(tok.value()));
        }
    }

    return result;
}

constexpr static int base_spec(int b) {
    switch (b) {
    case '0' ... '9': return 8;
    case 'b': return 2;
    case 'x': return 16;
    default: return 10;
    }
}

std::optional<Token> Asm::next_token(const char*& it, const char* end) {
    using Type = Token::Type;
    constexpr Type NONE = (Type)-1;

    Type type = NONE;
    std::string text;
    uint64_t value = 0;

#define finish()                                                   \
    do {                                                           \
        if (text.size() != 0)                                      \
            return std::move(Token(std::move(text), type, value)); \
        else                                                       \
            return {};                                             \
    } while (0)

/** Move to the next character, watching for EOF. */
#define next()                   \
    do {                         \
        it++;                    \
        if (it == end) finish(); \
    } while (0)

/** Accept character to the token and move to the next. */
#define accept()             \
    do {                     \
        text.push_back(*it); \
        next();              \
    } while (0)

    while (type == NONE) {
        while (isspace(*it)) { next(); }

        switch (*it) {
        case '_':
        case 'A' ... 'Z':
        case 'a' ... 'z': {
            type = Type::IDENT;
            do { accept(); } while (isalnum(*it) || *it == '_');
            finish();
        } break;

        case '0': {
            type = Type::INT;
            accept();
            int base = base_spec(*it);
            if (base != 8) accept();

            int d = todigit(*it);
            while (0 <= d && d < base) {
                value = value * base + d;
                accept();
                d = todigit(*it);
            }
            finish();
        } break;

        case '1' ... '9': {
            type = Type::INT;
            do {
                value = value * 10 + (*it - '0');
                accept();
            } while (isdigit(*it));
            finish();
        } break;

        case ';': {
            do { next(); } while (*it != '\n');
        } break;

        case '(': type = Type::OPEN_PAREN; break;
        case ')': type = Type::CLOSE_PAREN; break;
        case '[': type = Type::OPEN_BRACE; break;
        case ']': type = Type::CLOSE_BRACE; break;
        case ',': type = Type::COMMA; break;
        default: type = Type::SYMBOL; break;
        }
    }

    accept();
    finish();
}