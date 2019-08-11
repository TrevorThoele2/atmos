#pragma once

#include "FixedPoint.h"

#include "Serialization.h"

namespace Atmos
{
    class Percentage
    {
    public:
        using Wrapped = FixedPoint64;
        using Split = Wrapped::Split;

        static const Wrapped max;
        static const Wrapped min;
    public:
        Percentage() = default;
        Percentage(const Wrapped::Split& split);
        Percentage(const Percentage& arg) = default;
        Percentage& operator=(const Wrapped::Split& split);
        Percentage& operator=(const Percentage& arg) = default;

        bool operator==(const Percentage& arg) const;
        bool operator==(const Wrapped& arg) const;
        bool operator==(const Split& arg) const;
        bool operator!=(const Percentage& arg) const;
        bool operator!=(const Wrapped& arg) const;
        bool operator!=(const Split& arg) const;
        bool operator<(const Percentage& arg) const;
        bool operator<(const Wrapped& arg) const;
        bool operator<(const Split& arg) const;
        bool operator<=(const Percentage& arg) const;
        bool operator<=(const Wrapped& arg) const;
        bool operator<=(const Split& arg) const;
        bool operator>(const Percentage& arg) const;
        bool operator>(const Wrapped& arg) const;
        bool operator>(const Split& arg) const;
        bool operator>=(const Percentage& arg) const;
        bool operator>=(const Wrapped& arg) const;
        bool operator>=(const Split& arg) const;

        Percentage operator+(const Percentage& arg) const;
        Percentage operator+(const Wrapped& arg) const;
        Percentage operator+(const Split& arg) const;
        Percentage operator-(const Percentage& arg) const;
        Percentage operator-(const Wrapped& arg) const;
        Percentage operator-(const Split& arg) const;
        Percentage operator*(const Percentage& arg) const;
        Percentage operator*(const Wrapped& arg) const;
        Percentage operator*(const Split& arg) const;
        Percentage operator/(const Percentage& arg) const;
        Percentage operator/(const Wrapped& arg) const;
        Percentage operator/(const Split& arg) const;
        Percentage& operator+=(const Percentage& arg);
        Percentage& operator+=(const Wrapped& arg);
        Percentage& operator+=(const Split& arg);
        Percentage& operator-=(const Percentage& arg);
        Percentage& operator-=(const Wrapped& arg);
        Percentage& operator-=(const Split& arg);
        Percentage& operator*=(const Percentage& arg);
        Percentage& operator*=(const Wrapped& arg);
        Percentage& operator*=(const Split& arg);
        Percentage& operator/=(const Percentage& arg);
        Percentage& operator/=(const Wrapped& arg);
        Percentage& operator/=(const Split& arg);

        Percentage& operator++();
        Percentage& operator--();
        Percentage operator++(int);
        Percentage operator--(int);

        explicit operator char() const;
        explicit operator unsigned char() const;
        explicit operator short() const;
        explicit operator unsigned short() const;
        explicit operator int() const;
        explicit operator unsigned int() const;
        explicit operator long long() const;
        explicit operator unsigned long long() const;
        explicit operator float() const;
        explicit operator double() const;
        explicit operator Split() const;
        explicit operator Wrapped() const;

        void Floor();
        void Ceiling();

        // 100 = 1
        // 95 = 0.95
        Percentage ConvertToDecimal() const;
    private:
        Wrapped wrapped;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Percentage, BinaryArchive> :
        public CompositeScribe<::Atmos::Percentage, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}