
#include "ModulatorTrackNode.h"

#include "FunctionVariant.h"

namespace Atmos
{
    namespace Modulator
    {
        TrackNodeInterpolation::Linear::Linear()
        {}

        bool TrackNodeInterpolation::Linear::operator==(const Linear &arg) const
        {
            return true;
        }

        bool TrackNodeInterpolation::Linear::operator!=(const Linear &arg) const
        {
            return !(*this == arg);
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(TrackNodeInterpolation::Oscillation)
        {
            scribe(acceleration);
            scribe(rate);
        }

        TrackNodeInterpolation::Oscillation::Oscillation() : acceleration(0), rate(0)
        {}

        TrackNodeInterpolation::Oscillation::Oscillation(Acceleration acceleration, Rate rate) : acceleration(acceleration), rate(rate)
        {}

        bool TrackNodeInterpolation::Oscillation::operator==(const Oscillation &arg) const
        {
            return acceleration == arg.acceleration && rate == arg.rate;
        }

        bool TrackNodeInterpolation::Oscillation::operator!=(const Oscillation &arg) const
        {
            return !(*this == arg);
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(TrackNodeInterpolation)
        {
            scribe(variant);
        }

        TrackNodeInterpolation::TrackNodeInterpolation() : variant(Linear())
        {}

        TrackNodeInterpolation::TrackNodeInterpolation(Acceleration acceleration, Rate rate) : variant(Oscillation(acceleration, rate))
        {}

        TrackNodeInterpolation::TrackNodeInterpolation(const TrackNodeInterpolation &arg) : variant(arg.variant)
        {}

        TrackNodeInterpolation::TrackNodeInterpolation(TrackNodeInterpolation &&arg) : variant(std::move(arg.variant))
        {}

        TrackNodeInterpolation& TrackNodeInterpolation::operator=(const TrackNodeInterpolation &arg)
        {
            variant = arg.variant;
            return *this;
        }

        TrackNodeInterpolation& TrackNodeInterpolation::operator=(TrackNodeInterpolation &&arg)
        {
            variant = std::move(arg.variant);
            return *this;
        }

        bool TrackNodeInterpolation::operator==(const TrackNodeInterpolation &arg) const
        {
            return variant == arg.variant;
        }

        bool TrackNodeInterpolation::operator!=(const TrackNodeInterpolation &arg) const
        {
            return !(*this == arg);
        }

        void TrackNodeInterpolation::SetLinear()
        {
            variant.Set(Linear());
        }

        void TrackNodeInterpolation::SetOscillation(Acceleration acceleration, Rate rate)
        {
            variant.Set(Oscillation(acceleration, rate));
        }

        TrackNodeInterpolation::Delta TrackNodeInterpolation::TransformDelta(Delta delta) const
        {
            return delta;
        }

        TrackNodeInterpolation::Type TrackNodeInterpolation::GetType() const
        {
            return static_cast<Type>(variant.GetTypeAsID());
        }

        TrackNode::TrackNode(Value::Type variantType) : variantType(variantType)
        {}

        TrackNode::TrackNode(TrackNode &&arg) : variantType(std::move(arg.variantType)), endState(std::move(arg.endState)), interpolation(std::move(arg.interpolation)), timeTaken(std::move(arg.timeTaken))
        {}

        TrackNode& TrackNode::operator=(TrackNode &&arg)
        {
            variantType = std::move(arg.variantType);
            endState = std::move(arg.endState);
            interpolation = std::move(arg.interpolation);
            timeTaken = std::move(arg.timeTaken);
            return *this;
        }

        bool TrackNode::operator==(const TrackNode &arg) const
        {
            return variantType == arg.variantType && endState == arg.endState && interpolation == arg.interpolation && timeTaken == arg.timeTaken;
        }

        bool TrackNode::operator!=(const TrackNode &arg) const
        {
            return !(*this == arg);
        }

        void TrackNode::SetEndState(const Value &set)
        {
            if (set.GetType() != variantType)
                return;

            endState = set;
        }

        void TrackNode::SetInterpolation(const InterpolationT &set)
        {
            interpolation = set;
        }

        Delta TrackNode::GetDelta(const FrameTimer &timer) const
        {
            return interpolation.TransformDelta(timer.PercentageElapsed().Get());
        }

        Value TrackNode::GetEndValue() const
        {
            return endState;
        }

        void TrackNode::SetTimeTaken(const TimeValue &set)
        {
            timeTaken = set;
        }

        const TimeValue& TrackNode::GetTimeTaken() const
        {
            return timeTaken;
        }
    }
}