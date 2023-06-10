#pragma once

#include <format>
#include <optional>
#include <string>
#include <vector>

#include "types.hpp"

namespace Asm {

class Token {
   public:
    enum class Type {
        IDENT,
        INT,
        OPEN_BRACE,
        CLOSE_BRACE,
        OPEN_PAREN,
        CLOSE_PAREN,
        COMMA,
        SYMBOL
    };

   public:
    Type type{ Type::IDENT };
    std::string text;
    /** Numeric value for INT token. */
    num_t number{ 0 };

    Token(std::string&& _text, Type _type, num_t value)
        : text(_text), type(_type), number(value) {}
    Token(const std::string& _text, Type _type, num_t value)
        : text(_text), type(_type), number(value) {}
};

std::vector<Token> tokenize(const std::string& text);
std::optional<Token> next_token(const char*& input, const char* end);
};  // namespace Asm

template <>
struct std::formatter<Asm::Token::Type, char> {
    constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        while (it != ctx.end() && *it != '}') { ++it; }
        return it;
    }

    auto format(const Asm::Token::Type& type, std::format_context& ctx) const {
        using Type = Asm::Token::Type;
        using namespace std;
        auto out = ctx.out();

        switch (type) {
        case Type::IDENT: return format_to(out, "IDENT", 0);
        case Type::INT: return format_to(out, "INT");
        case Type::OPEN_BRACE: return format_to(out, "OPEN_BRACE");
        case Type::CLOSE_BRACE: return format_to(out, "CLOSE_BRACE");
        case Type::OPEN_PAREN: return format_to(out, "OPEN_PAREN");
        case Type::CLOSE_PAREN: return format_to(out, "CLOSE_PAREN");
        case Type::COMMA: return format_to(out, "COMMA");
        case Type::SYMBOL: return format_to(out, "SYMBOL");
        }

        return format_to(out, "Type<{}>", (int)type);
    }
};
//*
template <>
struct std::formatter<Asm::Token, char> {
    constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        while (it != ctx.end() && *it != '}') { ++it; }
        return it;
    }

    auto format(const Asm::Token& token, std::format_context& ctx) const {
        using namespace std;
        return format_to(ctx.out(), "{}(\"{}\")", token.type,
                         token.text.c_str());
    }
};
//*/