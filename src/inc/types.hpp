#pragma once

#include <cstdint>
#include <cstring>
#include <format>
#include <stdexcept>
#include <string>
#include <vector>

#define num_t unsigned long long

#define cnex constexpr

/** Get numeric value of a digit character */
constexpr static int todigit(char d) {
    switch (d) {
    case '0' ... '9': return d - '0';
    case 'A' ... 'Z': return d - 'A' + 10;
    case 'a' ... 'z': return d - 'a' + 10;
    default: return -1;
    }
}

class bits_t {
   private:
    int size_{ 0 };
    uint64_t value_{ 0 };

    cnex uint64_t mask(uint64_t x) const { return x & ((1ull << size_) - 1); }

   public:
    cnex auto value() const { return mask(value_); }

    /** Returns the size in bits. */
    cnex auto size() const { return size_; }

    /** Change the number of bits stored. */
    void resize(int new_size) {
        size_ = new_size;
        value_ = value();
    }

    cnex bits_t() : size_(0), value_(0) {}
    cnex bits_t(uint64_t x, int size) : size_(size), value_(0) { *this = x; }
    cnex bits_t(const char* bits) {
        using namespace std;
        string msg = "Invalid bits_t format: ";
        msg.append(bits);

        size_t len = std::strlen(bits);
        if (len < 2) throw logic_error(msg);

        int digit_bits;
        switch (bits[0]) {
        case 'b': digit_bits = 1; break;
        case 'o': digit_bits = 3; break;
        case 'x': digit_bits = 4; break;
        default: throw logic_error(msg); break;
        }

        int base = 1 << digit_bits;
        for (size_t i = 1; i < len; ++i) {
            int digit = todigit(bits[i]);
            if (digit < 0 || digit >= base) throw logic_error(msg);
            value_ <<= digit_bits;
            value_ |= digit;
            size_ += digit_bits;
        }
        value_ = value();
    }

    // assignments
    cnex bits_t operator=(uint64_t other) {
        value_ = mask(other);
        return *this;
    }

    // relations
    cnex bool operator==(uint64_t x) const { return value() == mask(x); }
    // cnex bool operator>(uint64_t x) const { return value() > mask(x); }
    // cnex bool operator<(uint64_t x) const { return value() < mask(x); }
    // cnex bool operator>=(uint64_t x) const { return value() >= mask(x);
    // } cnex bool operator<=(uint64_t x) const { return value() <= mask(x); }

    // arithmetic
    cnex bits_t operator+(uint64_t x) const { return { value() + x, size_ }; }
    cnex bits_t operator-(uint64_t x) const { return { value() - x, size_ }; }
    cnex bits_t operator*(uint64_t x) const { return { value() * x, size_ }; }
    cnex bits_t operator/(uint64_t x) const { return { value() / x, size_ }; }

    cnex bits_t operator&(uint64_t x) const { return { value() & x, size_ }; }
    cnex bits_t operator|(uint64_t x) const { return { value() | x, size_ }; }
    cnex bits_t operator<<(uint64_t x) const { return { value() << x, size_ }; }
    cnex bits_t operator>>(uint64_t x) const { return { value() >> x, size_ }; }

    cnex bits_t operator+=(uint64_t x) { return *this = value() + x; }
    cnex bits_t operator-=(uint64_t x) { return *this = value() - x; }
    cnex bits_t operator*=(uint64_t x) { return *this = value() * x; }
    cnex bits_t operator/=(uint64_t x) { return *this = value() / x; }

    cnex bits_t operator++() { return *this += 1; }
    cnex bits_t operator++(int) {
        bits_t now = *this;
        *this += 1;
        return now;
    }
    cnex bits_t operator--() { return *this -= 1; }
    cnex bits_t operator--(int) {
        bits_t now = *this;
        *this -= 1;
        return now;
    }

    /** Concatenate another bits_t to the right of this one. */
    void push_back(const bits_t& other) {
        size_ += other.size();
        *this = value_ << other.size() | other.value();
    }

    class ctor {
       private:
        uint64_t bits_;

       public:
        cnex ctor(uint64_t bits) : bits_(bits) {}
        cnex bits_t operator()(long value) { return bits_t(value, bits_); }
    };
};

namespace bits {
/** Concatenate vector of bits_t to one instance. */
static bits_t cat(const std::vector<bits_t>& results) {
    bits_t reduced;
    for (auto&& r : results) { reduced.push_back(r); }
    return reduced;
}
}  // namespace bits

class symbol {
   public:
    int bit_width;
    std::string name;

    symbol() {}
    symbol(const char* name) : name(name) {}
};

template <typename Tchar, Tchar... str>
cnex bits_t operator"" _bits() {
    const char str2[] = { str..., '\0' };
    return bits_t(str2);
}

cnex bits_t::ctor operator""_bit(uint64_t bits) { return bits_t::ctor{ bits }; }

template <>
struct std::formatter<bits_t, char> {
    constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        while (it != ctx.end() && *it != '}') { ++it; }
        return it;
    }

    auto format(const bits_t& b, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "b{:0{}b}", b.value(), b.size());
    }
};