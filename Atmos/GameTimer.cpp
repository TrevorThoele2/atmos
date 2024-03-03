
#include "GameTimer.h"
#include "DayCycle.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(GameTimeValue)
    {
        scribe(value);
        scribe(epoch);
    }

    GameTimeValue::ValueT GameTimeValue::ConvertValueStatic(ValueT value, EpochT oldEpoch, EpochT newEpoch)
    {
        ValueT numerator = ValueT(1);
        ValueT denominator = ValueT(1);
        auto pieceMaker = [](ValueT &val, EpochT e)
        {
            switch (e)
            {
            case EpochT::DAYS:
                val = ValueT::Split(1, 0) / (ValueT::Split(secondsPerMinute, 0) * ValueT::Split(minutesPerHour, 0) * ValueT::Split(hoursPerDay, 0));
                break;
            case EpochT::HOURS:
                val = ValueT::Split(1, 0) / (ValueT::Split(secondsPerMinute, 0) * ValueT::Split(minutesPerHour, 0));
                break;
            case EpochT::MINUTES:
                val = ValueT::Split(1, 0) / ValueT::Split(secondsPerMinute, 0);
                break;
            case EpochT::SECONDS:
                val = ValueT(1);
                break;
            case EpochT::MILLISECONDS:
                val = ValueT(millisecondsPerSecond);
                break;
            }
        };

        pieceMaker(numerator, oldEpoch);
        pieceMaker(denominator, newEpoch);
        return (value * numerator) / denominator;
    }

    GameTimeValue::GameTimeValue(ValueT value) : value(value)
    {}

    bool GameTimeValue::operator==(const GameTimeValue &arg) const
    {
        return value == arg.value;
    }

    bool GameTimeValue::operator!=(const GameTimeValue &arg) const
    {
        return !(*this == arg);
    }

    bool GameTimeValue::operator>(const GameTimeValue &arg) const
    {
        return value > arg.value;
    }

    bool GameTimeValue::operator>=(const GameTimeValue &arg) const
    {
        return value >= arg.value;
    }

    bool GameTimeValue::operator<(const GameTimeValue &arg) const
    {
        return value < arg.value;
    }

    bool GameTimeValue::operator<=(const GameTimeValue &arg) const
    {
        return value <= arg.value;
    }

    GameTimeValue GameTimeValue::operator+(const GameTimeValue &other) const
    {
        return value + other.value;
    }

    GameTimeValue& GameTimeValue::operator+=(const GameTimeValue &other)
    {
        value += other.value;
        return *this;
    }

    GameTimeValue GameTimeValue::operator-(const GameTimeValue &other) const
    {
        return value - other.value;
    }

    GameTimeValue& GameTimeValue::operator-=(const GameTimeValue &other)
    {
        value -= other.value;
        return *this;
    }

    GameTimeValue GameTimeValue::operator*(const GameTimeValue &other) const
    {
        return value * other.value;
    }

    GameTimeValue& GameTimeValue::operator*=(const GameTimeValue &other)
    {
        value *= other.value;
        return *this;
    }

    GameTimeValue GameTimeValue::operator/(const GameTimeValue &other) const
    {
        return value / other.value;
    }

    GameTimeValue& GameTimeValue::operator/=(const GameTimeValue &other)
    {
        value /= other.value;
        return *this;
    }

    GameTimeValue::operator ValueT() const
    {
        return value;
    }

    typename GameTimeValue::ValueT GameTimeValue::Get() const
    {
        return value;
    }

    GameTimeValue::Radix GameTimeValue::GetRadixPoint() const
    {
        return value.GetRadixPoint();
    }

    void GameTimeValue::Convert(EpochT epoch)
    {
        value = ConvertValueStatic(value, this->epoch, epoch);
    }

    GameTimeValue::ValueT GameTimeValue::ConvertValue(EpochT epoch) const
    {
        return ConvertValueStatic(value, this->epoch, epoch);
    }

    GameTimeValue::EpochT GameTimeValue::GetEpoch() const
    {
        return epoch;
    }

    GameTimer::GameTimer(Goal goal) : goal(goal), start(GameTimeValue::ValueT(0))
    {}

    bool GameTimer::operator==(const GameTimer &arg) const
    {
        return goal == arg.goal && start == arg.start;
    }

    bool GameTimer::operator!=(const GameTimer &arg) const
    {
        return !(*this == arg);
    }

    GameTimeValue GameTimer::Start()
    {
        start = CurrentTime();
        return start;
    }

    void GameTimer::SetStart(const GameTimeValue &set)
    {
        start = set;
    }

    GameTimeValue GameTimer::GetStart() const
    {
        return start;
    }

    void GameTimer::SetGoal(Goal set)
    {
        goal = set;
    }

    GameTimer::Goal GameTimer::GetGoal() const
    {
        return goal;
    }

    bool GameTimer::HasReachedGoal() const
    {
        return Elapsed() >= goal;
    }

    GameTimeValue GameTimer::Elapsed() const
    {
        return CurrentTime() - start;
    }

    GameTimer::Goal GameTimer::PercentageElapsed() const
    {
        return Elapsed() / goal;
    }

    GameTimeValue GameTimer::CurrentTime() const
    {
        return DayCycleHandler::GetCurrentTime().ConvertValue(EpochT::MILLISECONDS);
    }
}