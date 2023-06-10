#pragma once

#include <format>
#include <string>
#include <type_traits>
#include <vector>

struct Pair {
    std::string key;
    int value;
};

template <>
struct std::formatter<Pair, char> {
    constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        while (it != ctx.end() && *it != '}') { ++it; }
        return it;
    }

    auto format(const Pair& pair, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({}: {})", pair.key, pair.value);
    }
};

template <typename T, typename Alloc>
struct std::formatter<std::vector<T, Alloc>, char> {
    constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        while (it != ctx.end() && *it != '}') { ++it; }
        return it;
    }

    auto format(const std::vector<T, Alloc>& vec, std::format_context& ctx) const {
        auto out = ctx.out();
        std::format_to(out, "[");

        auto it = vec.begin();
        if (it != vec.end())  //
            std::format_to(out, "{}", *it++);

        for (; it != vec.end(); ++it) {  //
            std::format_to(ctx.out(), ", {}", *it);
        }
        return std::format_to(out, "]");
    }
};