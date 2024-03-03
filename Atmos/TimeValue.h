#pragma once

#include "FixedPoint.h"

#include "Serialization.h"

namespace Atmos
{
    enum class TimeValueEpoch
    {
        MINUTES,
        SECONDS,
        MILLISECONDS,
        MICROSECONDS,
        NANOSECONDS
    };

    class TimeValue
    {
    public:
        typedef TimeValueEpoch EpochT;
        typedef FixedPoint64 Value;
        typedef Value::Radix Radix;
    public:
        TimeValue(Value value = Value(), EpochT epoch = EpochT::SECONDS);
        TimeValue(Value::Value value, EpochT epoch);
        TimeValue(const TimeValue& arg) = default;

        TimeValue& operator=(const TimeValue& arg) = default;

        bool operator==(const TimeValue& arg) const;
        bool operator!=(const TimeValue& arg) const;
        bool operator<(const TimeValue& arg) const;
        bool operator<=(const TimeValue& arg) const;
        bool operator>(const TimeValue& arg) const;
        bool operator>=(const TimeValue& arg) const;

        TimeValue operator+(const TimeValue& arg) const;
        TimeValue& operator+=(const TimeValue& arg);
        TimeValue operator-(const TimeValue& arg) const;
        TimeValue& operator-=(const TimeValue& arg);
        TimeValue operator*(const TimeValue& arg) const;
        TimeValue& operator*=(const TimeValue& arg);
        TimeValue operator/(const TimeValue& arg) const;
        TimeValue& operator/=(const TimeValue& arg);

        explicit operator double() const;

        explicit operator Value() const;
        Value Get() const;
        Radix GetRadixPoint() const;
        static Radix GetRadixPoint(EpochT epoch);

        void Convert(EpochT epoch);
        Value ConvertValue(EpochT epoch) const;
        EpochT GetEpoch() const;
    private:
        Value value;
        EpochT epoch;
    private:
        static Value ConvertValueStatic(Value value, EpochT oldEpoch, EpochT newEpoch, bool manipulateValue = false);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::TimeValue, BinaryArchive> :
        public CompositeScribe<::Atmos::TimeValue, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };

    template<>
    class Scribe<::Atmos::TimeValueEpoch, BinaryArchive> :
        public EnumScribe<::Atmos::TimeValueEpoch, BinaryArchive>
    {};
}