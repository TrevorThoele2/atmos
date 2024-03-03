
#include "TimeValue.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(TimeValue)
    {
        scribe(value, epoch);
    }

    TimeValue::ValueT TimeValue::ConvertValueStatic(ValueT value, EpochT newEpoch, bool manipulateValue)
    {
        RadixPoint newRadixPoint;
        switch (newEpoch)
        {
        case EpochT::MINUTES:
            newRadixPoint = ValueT::GetDefaultRadixPoint().Get() + 1;
            break;
        case EpochT::SECONDS:
            newRadixPoint = ValueT::GetDefaultRadixPoint().Get();
            break;
        case EpochT::MILLISECONDS:
            newRadixPoint = ValueT::GetDefaultRadixPoint().Get() - 3;
            break;
        case EpochT::MICROSECONDS:
            newRadixPoint = ValueT::GetDefaultRadixPoint().Get() - 6;
            break;
        case EpochT::NANOSECONDS:
            newRadixPoint = ValueT::GetDefaultRadixPoint().Get() - 9;
            break;
        }

        value.SetRadixPoint(newRadixPoint, manipulateValue);
        return value;
    }

    TimeValue::TimeValue(ValueT value, EpochT epoch) : value(value), epoch(epoch)
    {
        value = ConvertValueStatic(value, epoch, true);
    }

    TimeValue::TimeValue(ValueT::ValueT value, EpochT epoch) : value(value, GetRadixPoint(epoch)), epoch(epoch)
    {}

    bool TimeValue::operator==(const TimeValue &arg) const
    {
        return value == arg.value && epoch == arg.epoch;
    }

    bool TimeValue::operator!=(const TimeValue &arg) const
    {
        return !(*this == arg);
    }

    bool TimeValue::operator>(const TimeValue &arg) const
    {
        return value > ConvertValueStatic(arg.value, epoch);
    }

    bool TimeValue::operator>=(const TimeValue &arg) const
    {
        return value >= ConvertValueStatic(arg.value, epoch);
    }

    bool TimeValue::operator<(const TimeValue &arg) const
    {
        return value < ConvertValueStatic(arg.value, epoch);
    }

    bool TimeValue::operator<=(const TimeValue &arg) const
    {
        return value <= ConvertValueStatic(arg.value, epoch);
    }

    TimeValue TimeValue::operator+(const TimeValue &other) const
    {
        return TimeValue(value + ConvertValueStatic(other.value, epoch));
    }

    TimeValue& TimeValue::operator+=(const TimeValue &other)
    {
        value += ConvertValueStatic(other.value, epoch);
        return *this;
    }

    TimeValue TimeValue::operator-(const TimeValue &other) const
    {
        return TimeValue(value - ConvertValueStatic(other.value, epoch));
    }

    TimeValue& TimeValue::operator-=(const TimeValue &other)
    {
        value -= ConvertValueStatic(other.value, epoch);
        return *this;
    }

    TimeValue TimeValue::operator*(const TimeValue &other) const
    {
        return TimeValue(value * ConvertValueStatic(other.value, epoch));
    }

    TimeValue& TimeValue::operator*=(const TimeValue &other)
    {
        value *= ConvertValueStatic(other.value, epoch);
        return *this;
    }

    TimeValue TimeValue::operator/(const TimeValue &other) const
    {
        return TimeValue(value / ConvertValueStatic(other.value, epoch));
    }

    TimeValue& TimeValue::operator/=(const TimeValue &other)
    {
        value /= ConvertValueStatic(other.value, epoch);
        return *this;
    }

    TimeValue::operator double() const
    {
        return static_cast<double>(value);
    }

    TimeValue::operator ValueT() const
    {
        return value;
    }

    typename TimeValue::ValueT TimeValue::Get() const
    {
        return value;
    }

    TimeValue::Radix TimeValue::GetRadixPoint() const
    {
        return value.GetRadixPoint();
    }

    TimeValue::Radix TimeValue::GetRadixPoint(EpochT epoch)
    {
        switch (epoch)
        {
        case EpochT::MINUTES:
            return TimeValue::Radix(ValueT::GetDefaultRadixPoint().Get() + 1);
        case EpochT::SECONDS:
            return TimeValue::Radix(ValueT::GetDefaultRadixPoint());
        case EpochT::MILLISECONDS:
            return TimeValue::Radix(ValueT::GetDefaultRadixPoint().Get() - 3);
        case EpochT::MICROSECONDS:
            return TimeValue::Radix(ValueT::GetDefaultRadixPoint().Get() - 6);
        case EpochT::NANOSECONDS:
            return TimeValue::Radix(ValueT::GetDefaultRadixPoint().Get() - 9);
        }

        return TimeValue::Radix(ValueT::GetDefaultRadixPoint());
    }

    void TimeValue::Convert(EpochT epoch)
    {
        value = ConvertValueStatic(value, epoch);
    }

    TimeValue::ValueT TimeValue::ConvertValue(EpochT epoch) const
    {
        return ConvertValueStatic(value, epoch);
    }

    TimeValue::EpochT TimeValue::GetEpoch() const
    {
        return epoch;
    }
}