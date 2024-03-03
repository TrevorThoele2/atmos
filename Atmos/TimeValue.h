#pragma once

#include "Epoch.h"

#include "FixedPoint.h"

#include "Serialization.h"

namespace Atmos::Time
{
    class Value
    {
    public:
        typedef FixedPoint64 Number;
        typedef Number::Radix Radix;
    public:
        Value(Number number = Number(), Epoch epoch = Epoch::SECONDS);
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
        Number Get() const;
        Number GetAs(Epoch epoch) const;
        Radix GetRadixPoint() const;
        static Radix GetRadixPoint(Epoch epoch);

        void ConvertTo(Epoch epoch);
        Epoch GetEpoch() const;
    private:
        Number number;
        Epoch epoch;
    private:
        static Number ConvertNumberStatic(Number number, Epoch oldEpoch, Epoch newEpoch, bool manipulateValue = false);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Time::Value, BinaryArchive> :
        public CompositeScribe<::Atmos::Time::Value, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };

    template<>
    class Scribe<::Atmos::Time::Epoch, BinaryArchive> :
        public EnumScribe<::Atmos::Time::Epoch, BinaryArchive>
    {};
}