#include "TimeValue.h"

namespace Atmos::Time
{
    Value::Value(Number number, Epoch epoch) : number(number), epoch(epoch)
    {
        this->number = ConvertNumberStatic(number, Epoch::Seconds, epoch, true);
    }

    Value::Value(Number::Value number, Epoch epoch) : number(number, GetRadixPoint(epoch)), epoch(epoch)
    {}

    bool Value::operator==(const Value& arg) const
    {
        return number == ConvertNumberStatic(arg.number, arg.epoch, epoch);
    }

    bool Value::operator!=(const Value& arg) const
    {
        return !(*this == arg);
    }

    bool Value::operator<(const Value& arg) const
    {
        return number < ConvertNumberStatic(arg.number, arg.epoch, epoch);
    }

    bool Value::operator<=(const Value& arg) const
    {
        return number <= ConvertNumberStatic(arg.number, arg.epoch, epoch);
    }

    bool Value::operator>(const Value& arg) const
    {
        return number > ConvertNumberStatic(arg.number, arg.epoch, epoch);
    }

    bool Value::operator>=(const Value& arg) const
    {
        return number >= ConvertNumberStatic(arg.number, arg.epoch, epoch);
    }

    Value Value::operator+(const Value& arg) const
    {
        return Value(number + ConvertNumberStatic(arg.number, arg.epoch, epoch));
    }

    Value& Value::operator+=(const Value& arg)
    {
        number += ConvertNumberStatic(arg.number, arg.epoch, epoch);
        return *this;
    }

    Value Value::operator-(const Value& arg) const
    {
        return Value(number - ConvertNumberStatic(arg.number, arg.epoch, epoch));
    }

    Value& Value::operator-=(const Value& arg)
    {
        number -= ConvertNumberStatic(arg.number, arg.epoch, epoch);
        return *this;
    }

    Value Value::operator*(const Value& arg) const
    {
        return Value(number * ConvertNumberStatic(arg.number, arg.epoch, epoch));
    }

    Value& Value::operator*=(const Value& arg)
    {
        number *= ConvertNumberStatic(arg.number, arg.epoch, epoch);
        return *this;
    }

    Value Value::operator/(const Value& arg) const
    {
        return Value(number / ConvertNumberStatic(arg.number, arg.epoch, epoch));
    }

    Value& Value::operator/=(const Value& arg)
    {
        number /= ConvertNumberStatic(arg.number, arg.epoch, epoch);
        return *this;
    }

    Value::operator double() const
    {
        return static_cast<double>(number);
    }

    Value::operator Number() const
    {
        return number;
    }

    Value::Number Value::Get() const
    {
        return number;
    }

    Value::Number Value::GetAs(Epoch epoch) const
    {
        return ConvertNumberStatic(number, this->epoch, epoch);
    }

    Value::Radix Value::GetRadixPoint() const
    {
        return number.GetRadixPoint();
    }

    Value::Radix Value::GetRadixPoint(Epoch epoch)
    {
        switch (epoch)
        {
        case Epoch::Minutes:
            return Number::Radix(Number::GetDefaultRadixPoint().Get() + 1);
        case Epoch::Seconds:
            return Number::Radix(Number::GetDefaultRadixPoint());
        case Epoch::Milliseconds:
            return Number::Radix(Number::GetDefaultRadixPoint().Get() - 3);
        case Epoch::Microseconds:
            return Number::Radix(Number::GetDefaultRadixPoint().Get() - 6);
        case Epoch::Nanoseconds:
            return Number::Radix(Number::GetDefaultRadixPoint().Get() - 9);
        }

        return Number::Radix(Number::GetDefaultRadixPoint());
    }

    void Value::ConvertTo(Epoch epoch)
    {
        number = ConvertNumberStatic(number, this->epoch, epoch);
    }

    Epoch Value::GetEpoch() const
    {
        return epoch;
    }

    Value::Number Value::ConvertNumberStatic(Number number, Epoch oldEpoch, Epoch newEpoch, bool manipulateValue)
    {
        unsigned int epochDifference = 0;
        switch (oldEpoch)
        {
        case Epoch::Minutes:
            epochDifference -= 1;
            break;
        case Epoch::Seconds:
            break;
        case Epoch::Milliseconds:
            epochDifference += 3;
            break;
        case Epoch::Microseconds:
            epochDifference += 6;
            break;
        case Epoch::Nanoseconds:
            epochDifference += 9;
            break;
        }

        switch (newEpoch)
        {
        case Epoch::Minutes:
            epochDifference += 1;
            break;
        case Epoch::Seconds:
            break;
        case Epoch::Milliseconds:
            epochDifference -= 3;
            break;
        case Epoch::Microseconds:
            epochDifference -= 6;
            break;
        case Epoch::Nanoseconds:
            epochDifference -= 9;
            break;
        }

        number.SetRadixPoint(RadixPoint(Number::GetDefaultRadixPoint().Get() + epochDifference), manipulateValue);
        return number;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Time::Value, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.number);
        archive(object.epoch);
    }
}