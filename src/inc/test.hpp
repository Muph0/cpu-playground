#pragma once

#define _QUOTE(x) #x
#define QUOTE(x) _QUOTE(x)

#define TEST_SUITE(name)                                           \
    static void test_switcher(int, bool);                          \
    int test_##name(int argc, char** argv) {                              \
        int choice = 1;                                            \
        if (argc > 1) {                                            \
            if (sscanf(argv[1], "%d", &choice) != 1) {             \
                printf("Couldn't parse that input as a number\n"); \
                return -1;                                         \
            }                                                      \
            test_switcher(choice, false);                          \
        } else {                                                   \
            test_switcher(0, true);                                \
        }                                                          \
        return 0;                                                  \
    }                                                              \
    static void test_switcher(int choice, bool all)

#define SHOULD(msg)                                          \
    if (--choice == 0) printf("This test should " msg "\n"); \
    if (all || choice == 0)

#include <format>
#include <stdexcept>
#include <vector>

#define expect(expr, ...)                                         \
    do {                                                          \
        if (!(expr)) {                                            \
            std::string msg = __FILE__ ":" QUOTE(__LINE__) ":\n"; \
            std::format_to(std::back_inserter(msg),               \
                           "Expected " #expr __VA_ARGS__);        \
            throw std::logic_error(msg);                          \
        }                                                         \
    } while (0)

#define expect_eq(a, b, ...)                                         \
    do {                                                             \
        if (!((a) == (b))) {                                         \
            std::string msg = __FILE__ ":" QUOTE(__LINE__) ":\n";    \
            std::format_to(std::back_inserter(msg),                  \
                           "Expected " #a " == " #b "; got {}", a);  \
            std::format_to(std::back_inserter(msg), "" __VA_ARGS__); \
            throw std::logic_error(msg);                             \
        }                                                            \
    } while (0)

//
