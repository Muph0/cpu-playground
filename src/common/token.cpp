#include "token.hpp"

#include <cctype>

using namespace Asm;

Token::Token(const std::string& text, Type type) : text_(text), type_(type) {}
Token::Token(std::string&& text, Type type) : text_(text), type_(type) {}

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

std::optional<Token> Asm::next_token(const char*& it, const char* end) {
    using Type = Token::Type;
    constexpr Type NONE = (Type)-1;

    Type type = NONE;
    std::string text;

#define finish()                                            \
    do {                                                    \
        if (text.size() != 0)                               \
            return std::move(Token(std::move(text), type)); \
        else                                                \
            return {};                                      \
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

        case '0' ... '9': {
            type = Type::INT;
            do { accept(); } while (isdigit(*it));
            finish();
        } break;

        case ';': {
            do { next(); } while (*it != '\n');
        } break;

        case '(': type = Type::OPEN_PAREN; break;
        case ')': type = Type::CLOSE_PAREN; break;
        case '[': type = Type::OPEN_BRACE; break;
        case ']': type = Type::CLOSE_BRACE; break;
        default: type = Type::SYMBOL; break;
        }
    }

    accept();
    finish();
}