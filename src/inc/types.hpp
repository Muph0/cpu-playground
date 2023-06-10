#pragma once

#include <cstdint>
#include <cstring>
#include <format>
#include <stdexcept>
#include <string>
#include <vector>

#define num_t unsigned long long

#define cnex constexpr

class bits_t {
   private:
    int size_{ 0 };
    uint64_t value_{ 0 };

    cnex uint64_t mask() const { return (1ull << size_) - 1; }

   public:
    cnex auto value() const { return value_ & mask(); }

    /** Returns the size in bits. */
    cnex auto size() const { return size_; }

    /** Change the number of bits stored. */
    void resize(int new_size) { size_ = new_size; }

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
            int digit = bits[i] - '0';
            if (digit < 0 || digit >= base) throw logic_error(msg);
            value_ <<= digit_bits;
            value_ |= digit;
            size_ += digit_bits;
        }
    }

    // assignments
    cnex bits_t operator=(uint64_t other) {
        value_ = other;
        return *this;
    }

    // relations
    cnex bool operator==(uint64_t other) const { return value_ == other; }
    //cnex bool operator==(int64_t other) const { return value_ == other; }
    cnex bool operator>(uint64_t other) const { return value_ > other; }
    cnex bool operator<(uint64_t other) const { return value_ < other; }
    cnex bool operator>=(uint64_t other) const { return value_ >= other; }
    cnex bool operator<=(uint64_t other) const { return value_ <= other; }

    // arithmetic
    cnex bits_t operator+(uint64_t x) const { return { value_ + x, size_ }; }
    cnex bits_t operator-(uint64_t x) const { return { value_ - x, size_ }; }
    cnex bits_t operator*(uint64_t x) const { return { value_ * x, size_ }; }
    cnex bits_t operator/(uint64_t x) const { return { value_ / x, size_ }; }

    cnex bits_t operator&(uint64_t x) const { return { value_ & x, size_ }; }
    cnex bits_t operator|(uint64_t x) const { return { value_ | x, size_ }; }
    cnex bits_t operator<<(uint64_t x) const { return { value_ << x, size_ }; }
    cnex bits_t operator>>(uint64_t x) const { return { value_ >> x, size_ }; }

    cnex void operator+=(uint64_t x) { value_ += x; }
    cnex void operator-=(uint64_t x) { value_ -= x; }
    cnex void operator*=(uint64_t x) { value_ *= x; }
    cnex void operator/=(uint64_t x) { value_ /= x; }

    cnex bits_t operator++() {
        value_ = (value_ + 1);
        return *this;
    }
    cnex bits_t operator++(int) {
        long now = value_;
        value_ = (value_ + 1);
        return bits_t(now, size_);
    }

    /** Concatenate another bits_t to the right of this one. */
    void push_back(const bits_t& other) {
        value_ = value_ << other.size() | other.value();
        size_ += other.size();
    }

    class ctor {
       private:
        uint64_t bits_;

       public:
        cnex ctor(uint64_t bits) : bits_(bits) {}
        cnex bits_t operator()(long value) { return bits_t(value, bits_); }
    };
};

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