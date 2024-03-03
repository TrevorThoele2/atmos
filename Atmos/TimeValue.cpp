
#include "TimeValue.h"

namespace Atmos
{
    TimeValue::TimeValue(Value value, EpochT epoch) : value(value), epoch(epoch)
    {
        value = ConvertValueStatic(value, EpochT::SECONDS, epoch, true);
    }

    TimeValue::TimeValue(Value::Value value, EpochT epoch) : value(value, GetRadixPoint(epoch)), epoch(epoch)
    {}

    bool TimeValue::operator==(const TimeValue& arg) const
    {
        return value == ConvertValueStatic(arg.value, arg.epoch, epoch);
    }

    bool TimeValue::operator!=(const TimeValue& arg) const
    {
        return !(*this == arg);
    }

    bool TimeValue::operator<(const TimeValue& arg) const
    {
        return value < ConvertValueStatic(arg.value, arg.epoch, epoch);
    }

    bool TimeValue::operator<=(const TimeValue& arg) const
    {
        return value <= ConvertValueStatic(arg.value, arg.epoch, epoch);
    }

    bool TimeValue::operator>(const TimeValue& arg) const
    {
        return value > ConvertValueStatic(arg.value, arg.epoch, epoch);
    }

    bool TimeValue::operator>=(const TimeValue& arg) const
    {
        return value >= ConvertValueStatic(arg.value, arg.epoch, epoch);
    }

    TimeValue TimeValue::operator+(const TimeValue& arg) const
    {
        return TimeValue(value + ConvertValueStatic(arg.value, arg.epoch, epoch));
    }

    TimeValue& TimeValue::operator+=(const TimeValue& arg)
    {
        value += ConvertValueStatic(arg.value, arg.epoch, epoch);
        return *this;
    }

    TimeValue TimeValue::operator-(const TimeValue& arg) const
    {
        return TimeValue(value - ConvertValueStatic(arg.value, arg.epoch, epoch));
    }

    TimeValue& TimeValue::operator-=(const TimeValue& arg)
    {
        value -= ConvertValueStatic(arg.value, arg.epoch, epoch);
        return *this;
    }

    TimeValue TimeValue::operator*(const TimeValue& arg) const
    {
        return TimeValue(value * ConvertValueStatic(arg.value, arg.epoch, epoch));
    }

    TimeValue& TimeValue::operator*=(const TimeValue& arg)
    {
        value *= ConvertValueStatic(arg.value, arg.epoch, epoch);
        return *this;
    }

    TimeValue TimeValue::operator/(const TimeValue& arg) const
    {
        return TimeValue(value / ConvertValueStatic(arg.value, arg.epoch, epoch));
    }

    TimeValue& TimeValue::operator/=(const TimeValue& arg)
    {
        value /= ConvertValueStatic(arg.value, arg.epoch, epoch);
        return *this;
    }

    TimeValue::operator double() const
    {
        return static_cast<double>(value);
    }

    TimeValue::operator Value() const
    {
        return value;
    }

    typename TimeValue::Value TimeValue::Get() const
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
            return TimeValue::Radix(Value::GetDefaultRadixPoint().Get() + 1);
        case EpochT::SECONDS:
            return TimeValue::Radix(Value::GetDefaultRadixPoint());
        case EpochT::MILLISECONDS:
            return TimeValue::Radix(Value::GetDefaultRadixPoint().Get() - 3);
        case EpochT::MICROSECONDS:
            return TimeValue::Radix(Value::GetDefaultRadixPoint().Get() - 6);
        case EpochT::NANOSECONDS:
            return TimeValue::Radix(Value::GetDefaultRadixPoint().Get() - 9);
        }

        return TimeValue::Radix(Value::GetDefaultRadixPoint());
    }

    void TimeValue::Convert(EpochT epoch)
    {
        value = ConvertValueStatic(value, this->epoch, epoch);
    }

    TimeValue::Value TimeValue::ConvertValue(EpochT epoch) const
    {
        return ConvertValueStatic(value, this->epoch, epoch);
    }

    TimeValue::EpochT TimeValue::GetEpoch() const
    {
        return epoch;
    }

    TimeValue::Value TimeValue::ConvertValueStatic(Value value, EpochT oldEpoch, EpochT newEpoch, bool manipulateValue)
    {
        unsigned int epochDifference = 0;
        switch (oldEpoch)
        {
        case EpochT::MINUTES:
            epochDifference -= 1;
            break;
        case EpochT::SECONDS:
            break;
        case EpochT::MILLISECONDS:
            epochDifference += 3;
            break;
        case EpochT::MICROSECONDS:
            epochDifference += 6;
            break;
        case EpochT::NANOSECONDS:
            epochDifference += 9;
            break;
        }

        switch (newEpoch)
        {
        case EpochT::MINUTES:
            epochDifference += 1;
            break;
        case EpochT::SECONDS:
            break;
        case EpochT::MILLISECONDS:
            epochDifference -= 3;
            break;
        case EpochT::MICROSECONDS:
            epochDifference -= 6;
            break;
        case EpochT::NANOSECONDS:
            epochDifference -= 9;
            break;
        }

        value.SetRadixPoint(RadixPoint(Value::GetDefaultRadixPoint().Get() + epochDifference), manipulateValue);
        return value;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::TimeValue, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.value);
        archive(object.epoch);
    }
}