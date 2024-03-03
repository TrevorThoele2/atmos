#pragma once

#include <chrono>

#include "Epoch.h"

#include "FixedPoint.h"

#include "Serialization.h"

namespace Atmos::Time
{
    using Value = std::chrono::time_point<std::chrono::steady_clock>;

    class Value
    {
    public:
        using Number = FixedPoint64;
        using Radix = Number::Radix;
    public:
        explicit Value(Number number = Number(), Epoch epoch = Epoch::Seconds);
        Value(Number::Value number, Epoch epoch);
        Value(const Value& arg) = default;

        Value& operator=(const Value& arg) = default;

        bool operator==(const Value& arg) const;
        bool operator!=(const Value& arg) const;
        bool operator<(const Value& arg) const;
        bool operator<=(const Value& arg) const;
        bool operator>(const Value& arg) const;
        bool operator>=(const Value& arg) const;

        Value operator+(const Value& arg) const;
        Value& operator+=(const Value& arg);
        Value operator-(const Value& arg) const;
        Value& operator-=(const Value& arg);
        Value operator*(const Value& arg) const;
        Value& operator*=(const Value& arg);
        Value operator/(const Value& arg) const;
        Value& operator/=(const Value& arg);

        explicit operator double() const;

        explicit operator Number() const;
        [[nodiscard]] Number Get() const;
        [[nodiscard]] Number GetAs(Epoch epoch) const;
        [[nodiscard]] Radix GetRadixPoint() const;
        static Radix GetRadixPoint(Epoch epoch);

        void ConvertTo(Epoch epoch);
        [[nodiscard]] Epoch GetEpoch() const;
    private:
        Number number;
        Epoch epoch;
    private:
        static Number ConvertNumberStatic(
            Number number,
            Epoch oldEpoch,
            Epoch newEpoch,
            bool manipulateValue = false);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Time::Value, BinaryArchive> final :
        public CompositeScribe<::Atmos::Time::Value, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };

    template<>
    class Scribe<::Atmos::Time::Epoch, BinaryArchive> final :
        public EnumScribe<::Atmos::Time::Epoch, BinaryArchive>
    {};
}