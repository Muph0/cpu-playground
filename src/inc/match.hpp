#pragma once

#include <format>
#include <functional>
#include <iostream>
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
        std::vector<Match> sequence_;
        Out (*reducer_)(const std::vector<Out>&) = bits::cat;

       public:
        Seq() = default;
        Seq(std::vector<Match>&& matches) : sequence_(matches) {}
        Seq(std::vector<Match>&& matches,
            Out (*reducer)(const std::vector<Out>&))
            : sequence_(matches), reducer_(reducer) {}
        bool parse(InPtr& start, InPtr end, Out& out) {
            auto it = start;
            std::vector<Out> results;

            for (auto&& item : sequence_) {
                Out itemResult;
                if (!item.parse(it, end, itemResult)) { return false; }
                results.push_back(itemResult);
            }
            if (reducer_ != nullptr) {
                out = reducer_(results);
            } else {
                out = bits::cat(results);
            }
            return true;
        }
    };

    // Useful factories
    Match() { parser = Seq(); }
    Match(Asm::Token::Type match) : Match(match, { 0, 0 }) {}
    Match(Asm::Token::Type match_, Out output) {
        auto match = match_;
        parser = One([=](const In& token, Out& out) {
            out = output;
            return token.type == match;
        });
    }
    /** Accept a token with specified text */
    Match(const char* match) : Match(match, { 0, 0 }) {}
    /** Accept a token with specified text and return output */
    Match(const char* match, const Out& output) {
        parser = One([=](const In& token, Out& out) {
            out = output;
            return token.text == match;
        });
    }

    static Match Integer(int bits) {
        return Seq(  //
            { Any({ "+", { "-", "b1" }, Match() }),
              One([=](const In& token, Out& out) {
                  out = bits_t(token.number, bits);
                  return token.type == Token::Type::INT;
              }) },
            [](const std::vector<Out>& parts) {
                auto sign = (parts[0].value() ? -1 : 1);
                return parts[1] * sign;
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