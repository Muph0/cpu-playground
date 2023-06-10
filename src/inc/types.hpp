#pragma once

#include <cstring>
#include <format>
#include <stdexcept>
#include <string>
#include <vector>

#define num_t unsigned long long

class bits_t {
   private:
    int size_{ 0 };
    long value_{ 0 };

    constexpr long mask() const { return (1 << size_) - 1; }

   public:
    constexpr auto value() const { return value_; }

    /** Returns the size in bits. */
    constexpr auto size() const { return size_; }

    /** Change the number of bits stored. */
    void resize(int new_size) {
        size_ = new_size;
        value_ &= mask();
    }

    constexpr bits_t() : size_(0), value_(0) {}
    constexpr bits_t(num_t x, int size) : size_(size), value_(0) { *this = x; }
    constexpr bits_t(const char* bits) {
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
    constexpr bits_t operator=(num_t other) {
        value_ = other & mask();
        return *this;
    }

    // relations
    constexpr bool operator==(num_t other) { return value_ == other; }
    constexpr bool operator>(num_t other) { return value_ > other; }
    constexpr bool operator<(num_t other) { return value_ < other; }
    constexpr bool operator>=(num_t other) { return value_ >= other; }
    constexpr bool operator<=(num_t other) { return value_ <= other; }

    // arithmetic
    constexpr num_t operator+(num_t other) { return value_ + other; }
    constexpr num_t operator-(num_t other) { return value_ - other; }
    constexpr num_t operator*(num_t other) { return value_ * other; }
    constexpr num_t operator/(num_t other) { return value_ / other; }

    constexpr num_t operator&(num_t other) { return value_ & other; }
    constexpr num_t operator|(num_t other) { return value_ | other; }
    constexpr num_t operator<<(num_t other) { return value_ << other; }
    constexpr num_t operator>>(num_t other) { return value_ >> other; }

    constexpr void operator+=(num_t other) { value_ += other; }
    constexpr void operator-=(num_t other) { value_ -= other; }
    constexpr void operator*=(num_t other) { value_ *= other; }
    constexpr void operator/=(num_t other) { value_ /= other; }

    constexpr bits_t operator++() {
        value_ = (value_ + 1) & mask();
        return *this;
    }
    constexpr bits_t operator++(int) {
        long now = value_;
        value_ = (value_ + 1) & mask();
        return bits_t(now, size_);
    }

    /** Concatenate another bits_t to the right of this one. */
    void push_back(const bits_t& other) {
        value_ = value_ << other.size() | other.value();
        size_ += other.size();
    }

    class ctor {
       private:
        num_t bits_;

       public:
        constexpr ctor(num_t bits) : bits_(bits) {}
        constexpr bits_t operator()(long value) { return bits_t(value, bits_); }
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
constexpr bits_t operator"" _bits() {
    const char str2[] = { str..., '\0' };
    return bits_t(str2);
}

constexpr bits_t::ctor operator""_bit(num_t bits) {
    return bits_t::ctor{ bits };
}