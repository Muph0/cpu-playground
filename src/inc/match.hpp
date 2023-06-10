#pragma once

#include <functional>
#include <string>
#include <variant>

#include "token.hpp"
#include "types.hpp"

namespace Asm {

using In = Asm::Token;
using InPtr = const Asm::Token*;
using Out = bits_t;

class Match {
   public:
    class One {
        std::function<bool(const In&, Out&)> condition_;

       public:
        One(std::function<bool(const In&, Out&)> condition)
            : condition_(condition) {}

        bool parse(InPtr& start, InPtr end, Out& out) {
            if (start == end) return false;
            Out result;

            if (condition_(*start, result)) {
                start++;
                out = result;
                return true;
            }
            return false;
        }
    };
    class Any {
        std::vector<Match> options;

       public:
        Any(std::vector<Match>&& matches) : options(matches) {}
        bool parse(InPtr& start, InPtr end, Out& out) {
            Out result;

            for (auto&& opt : options) {
                auto it = start;

                if (opt.parse(it, end, result)) {
                    start = it;
                    out = result;
                    return true;
                }
            }
            return false;
        }
    };
    class Seq {
        std::vector<Match> sequence;

       public:
        Seq() = default;
        Seq(std::vector<Match>&& matches) : sequence(matches) {}
        bool parse(InPtr& start, InPtr end, Out& out) {
            auto it = start;
            Out result;

            for (auto&& item : sequence) {
                Out itemResult;
                if (!item.parse(it, end, itemResult)) { return false; }
                result.push_back(itemResult);
            }
            out = result;
            return true;
        }
    };

    Match() { parser = Seq(); }
    Match(Asm::Token::Type match) : Match(match, { 0, 0 }) {}
    Match(Asm::Token::Type match, Out output) {
        parser = Match::One([&](const In& token, Out& out) {
            out = output;
            return token.type() == match;
        });
    }
    Match(const char* match) : Match(match, { 0, 0 }) {}
    Match(const char* match, Out output) {
        parser = Match::One([&](const In& token, Out& out) {
            out = output;
            return token.text() == match;
        });
    }

    // conversions
    Match(const Seq& p) : parser(p) {}
    Match(const One& p) : parser(p) {}
    Match(const Any& p) : parser(p) {}

    bool parse(InPtr& start, InPtr end, Out& out) {
        return std::visit(
            [&](auto&& arg) { return arg.parse(start, end, out); }, parser);
    }

   private:
    std::variant<Seq, One, Any> parser{ Seq() };
};

}  // namespace Asm