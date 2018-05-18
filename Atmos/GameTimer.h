#pragma once

#include "LargeInteger.h"
#include "FixedPoint.h"
#include "Serialization.h"

namespace Atmos
{
    enum class GameTimerEpoch
    {
        DAYS,
        HOURS,
        MINUTES,
        SECONDS,
        MILLISECONDS
    };

    class GameTimeValue
    {
    public:
        typedef GameTimerEpoch EpochT;
        typedef FixedPoint64 ValueT;
        typedef ValueT::Radix Radix;

        typedef unsigned int CountT;
        static constexpr CountT hoursPerDay = 24;
        static constexpr CountT minutesPerHour = 60;
        static constexpr CountT secondsPerMinute = 60;
        static constexpr CountT millisecondsPerSecond = 1000;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT value;
        EpochT epoch;

        static ValueT ConvertValueStatic(ValueT value, EpochT oldEpoch, EpochT newEpoch);
    public:
        GameTimeValue(ValueT value = ValueT());
        bool operator==(const GameTimeValue &arg) const;
        bool operator!=(const GameTimeValue &arg) const;
        bool operator>(const GameTimeValue &arg) const;
        bool operator>=(const GameTimeValue &arg) const;
        bool operator<(const GameTimeValue &arg) const;
        bool operator<=(const GameTimeValue &arg) const;
        GameTimeValue operator+(const GameTimeValue &other) const;
        GameTimeValue& operator+=(const GameTimeValue &other);
        GameTimeValue operator-(const GameTimeValue &other) const;
        GameTimeValue& operator-=(const GameTimeValue &other);
        GameTimeValue operator*(const GameTimeValue &other) const;
        GameTimeValue& operator*=(const GameTimeValue &other);
        GameTimeValue operator/(const GameTimeValue &other) const;
        GameTimeValue& operator/=(const GameTimeValue &other);

        explicit operator ValueT() const;
        ValueT Get() const;
        Radix GetRadixPoint() const;

        void Convert(EpochT epoch);
        ValueT ConvertValue(EpochT epoch) const;
        EpochT GetEpoch() const;
    };

    // A timer which times the in-game time
    class GameTimer
    {
    public:
        typedef GameTimerEpoch EpochT;

        typedef unsigned int CountT;
        static constexpr CountT hoursPerDay = 24;
        static constexpr CountT minutesPerHour = 60;
        static constexpr CountT secondsPerMinute = 60;
        static constexpr CountT millisecondsPerSecond = 1000;

        typedef GameTimeValue Goal;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        GameTimeValue start;
        GameTimeValue goal;
    public:
        GameTimer(Goal goal = Goal());
        bool operator==(const GameTimer &arg) const;
        bool operator!=(const GameTimer &arg) const;

        GameTimeValue Start();
        void SetStart(const GameTimeValue &set);
        GameTimeValue GetStart() const;

        void SetGoal(Goal set);
        Goal GetGoal() const;
        bool HasReachedGoal() const;

        GameTimeValue Elapsed() const;
        // Instead of 0% - 100%, it's 0 - 1 (but you probably already knew that didn't you?)
        Goal PercentageElapsed() const;

        GameTimeValue CurrentTime() const;
    };
}