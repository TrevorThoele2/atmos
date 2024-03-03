#pragma once

#include "NumericLimits.h"
#include "FixedPoint.h"
#include "String.h"

#include "Serialization.h"

namespace Atmos
{
    class LargeInteger
    {
    public:
        typedef unsigned long long WrappedT;
    public:
        constexpr LargeInteger();
        constexpr LargeInteger(WrappedT low, WrappedT high);
        constexpr LargeInteger(char c);
        constexpr LargeInteger(unsigned char c);
        constexpr LargeInteger(short s);
        constexpr LargeInteger(unsigned short s);
        constexpr LargeInteger(int i);
        constexpr LargeInteger(unsigned int i);
        constexpr LargeInteger(long long ll);
        constexpr LargeInteger(unsigned long long ll);
        LargeInteger(const LargeInteger& arg) = default;
        LargeInteger& operator=(const LargeInteger& arg) = default;

        bool operator==(const LargeInteger& arg) const;
        bool operator!=(const LargeInteger& arg) const;
        bool operator<(const LargeInteger& arg) const;
        bool operator<=(const LargeInteger& arg) const;
        bool operator>(const LargeInteger& arg) const;
        bool operator>=(const LargeInteger& arg) const;
        explicit operator bool() const;

        LargeInteger operator+(const LargeInteger& arg) const;
        LargeInteger& operator+=(const LargeInteger& arg);
        LargeInteger operator-(const LargeInteger& arg) const;
        LargeInteger& operator-=(const LargeInteger& arg);
        LargeInteger operator*(const LargeInteger& arg) const;
        LargeInteger& operator*=(const LargeInteger& arg);
        LargeInteger operator/(const LargeInteger& arg) const;
        LargeInteger& operator/=(const LargeInteger& arg);

        LargeInteger operator<<(const LargeInteger& arg) const;
        LargeInteger operator<<(size_t arg) const;
        LargeInteger& operator<<=(const LargeInteger& arg);
        LargeInteger& operator<<=(size_t arg);
        LargeInteger operator >> (const LargeInteger& arg) const;
        LargeInteger operator >> (size_t arg) const;
        LargeInteger& operator>>=(const LargeInteger& arg);
        LargeInteger& operator>>=(size_t arg);

        LargeInteger& operator++();
        LargeInteger& operator--();
        LargeInteger operator++(int);
        LargeInteger operator--(int);

        LargeInteger operator%(const LargeInteger& arg) const;
        LargeInteger& operator%=(const LargeInteger& arg);
        bool operator!() const;
        LargeInteger operator~() const;
        LargeInteger operator|(const LargeInteger& arg) const;
        LargeInteger& operator|=(const LargeInteger& arg);
        LargeInteger operator&(const LargeInteger& arg) const;
        LargeInteger& operator&=(const LargeInteger& arg);
        LargeInteger operator^(const LargeInteger& arg) const;
        LargeInteger& operator^=(const LargeInteger& arg);

        WrappedT GetLow() const;
        WrappedT GetHigh() const;

        String ToString() const;
        void FromString(const String& arg);
    private:
        static constexpr WrappedT maxWrappedValue = std::numeric_limits<unsigned long long>::max();
        static constexpr size_t digitCount = sizeof(WrappedT) * 2 * CHAR_BIT;
    private:
        WrappedT low;
        WrappedT high;
    private:
        static void AddImpl(const LargeInteger& left, const LargeInteger& right, LargeInteger& result);
        static void SubtractImpl(const LargeInteger& left, const LargeInteger& right, LargeInteger& result);
        static void MultiplyImpl(const LargeInteger& multiplicand, const LargeInteger& multiplier, LargeInteger& result);
        static void DivideImpl(const LargeInteger& dividend, const LargeInteger& divisor, LargeInteger& quotient, LargeInteger& remainder);
    private:
        INSCRIPTION_ACCESS;
    };

    constexpr LargeInteger::LargeInteger() : low(0), high(0)
    {}

    constexpr LargeInteger::LargeInteger(WrappedT low, WrappedT high) : low(low), high(high)
    {}

    constexpr LargeInteger::LargeInteger(char c) : low(c), high(0)
    {}

    constexpr LargeInteger::LargeInteger(unsigned char c) : low(c), high(0)
    {}

    constexpr LargeInteger::LargeInteger(short s) : low(s), high(0)
    {}

    constexpr LargeInteger::LargeInteger(unsigned short s) : low(s), high(0)
    {}

    constexpr LargeInteger::LargeInteger(int i) : low(i), high(0)
    {}

    constexpr LargeInteger::LargeInteger(unsigned int i) : low(i), high(0)
    {}

    constexpr LargeInteger::LargeInteger(long long ll) : low(ll), high(0)
    {}

    constexpr LargeInteger::LargeInteger(unsigned long long ll) : low(ll), high(0)
    {}

    template<>
    struct NumericLimits<LargeInteger>
    {
        typedef LargeInteger T;
        static constexpr T min()
        {
            return LargeInteger(0U);
        }

        static constexpr T lowest()
        {
            return LargeInteger(0);
        }

        static constexpr T max()
        {
            return LargeInteger(std::numeric_limits<unsigned long long>::max());
        }

        static constexpr int digits10()
        {
            return std::numeric_limits<unsigned long long>::digits10 * 2ULL;
        }

        static constexpr bool is_signed = false;
    };

    typedef FixedPoint<LargeInteger> FixedPoint128;
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::LargeInteger, BinaryArchive> : public CompositeScribe<::Atmos::LargeInteger, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}