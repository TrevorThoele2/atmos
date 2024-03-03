
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

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(TrackNodeEndState::Normal)
        {
            scribe(end);
        }

        TrackNodeEndState::Normal::Normal() : end(std::int64_t(0))
        {}

        TrackNodeEndState::Normal::Normal(Value end) : end(end)
        {}

        bool TrackNodeEndState::Normal::operator==(const Normal &arg) const
        {
            return end == arg.end;
        }

        bool TrackNodeEndState::Normal::operator!=(const Normal &arg) const
        {
            return !(*this == arg);
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(TrackNodeEndState::Random)
        {
            scribe(range);
        }

        TrackNodeEndState::Random::Random() : pickedValue(Value(std::int64_t(0))), range(Value(std::int64_t(0)), Value(std::int64_t(0)))
        {}

        TrackNodeEndState::Random::Random(const RangeT &range) : pickedValue(Value(std::int64_t(0))), range(range)
        {}

        bool TrackNodeEndState::Random::operator==(const Random &arg) const
        {
            return range == arg.range;
        }

        bool TrackNodeEndState::Random::operator!=(const Random &arg) const
        {
            return !(*this == arg);
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(TrackNodeEndState)
        {
            scribe(variant);
        }

        TrackNodeEndState::TrackNodeEndState(Value::Type variantType) : variantType(variantType)
        {}

        TrackNodeEndState::TrackNodeEndState(const TrackNodeEndState &arg) : variantType(arg.variantType), variant(arg.variant)
        {}

        TrackNodeEndState::TrackNodeEndState(TrackNodeEndState &&arg) : variantType(std::move(arg.variantType)), variant(std::move(arg.variant))
        {}

        TrackNodeEndState& TrackNodeEndState::operator=(const TrackNodeEndState &arg)
        {
            variantType = arg.variantType;
            variant = arg.variant;
            return *this;
        }

        TrackNodeEndState& TrackNodeEndState::operator=(TrackNodeEndState &&arg)
        {
            variantType = std::move(arg.variantType);
            variant = std::move(arg.variant);
            return *this;
        }

        bool TrackNodeEndState::operator==(const TrackNodeEndState &arg) const
        {
            return variantType == arg.variantType && variant == arg.variant;
        }

        bool TrackNodeEndState::operator!=(const TrackNodeEndState &arg) const
        {
            return !(*this == arg);
        }

        void TrackNodeEndState::SetNormal(Value end)
        {
            end.Convert(variantType);
            variant.Set(Normal(end));
        }

        void TrackNodeEndState::SetRandom(const RangeT &range)
        {
            RangeT use(range);
            use.begin.Convert(variantType);
            use.end.Convert(variantType);
            variant.Set(Random(use));
        }

        Value TrackNodeEndState::GetEnding() const
        {
            if (!variant.IsInhabited())
                return Value();

            struct Getter
            {
                static Value DoReturn(const Normal &normal, Value::Type variantType)
                {
                    return normal.end;
                }

                static Value DoReturn(const Random &random, Value::Type variantType)
                {
                    switch (variantType)
                    {
                    case Value::Type::INT:
                        random.pickedValue = Value(::Atmos::Random::Generate<ValueTraits<Value::Type::INT>::T>(random.range.begin.AsInt(), random.range.end.AsInt()));
                        return random.pickedValue;
                    case Value::Type::FLOAT:
                        random.pickedValue = Value(::Atmos::Random::Generate<ValueTraits<Value::Type::FLOAT>::T>(random.range.begin.AsFloat(), random.range.end.AsFloat()));
                        return random.pickedValue;
                    }

                    return Value();
                }
            };

            return ::function::VisitReturn<Getter, Value>(variant, variantType);
        }

        TrackNodeEndState::Type TrackNodeEndState::GetType() const
        {
            return static_cast<Type>(variant.GetTypeAsID());
        }

        Value::Type TrackNodeEndState::GetVariantType() const
        {
            return variantType;
        }

        TrackNode::TrackNode(Value::Type variantType) : variantType(variantType), endState(variantType)
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

        void TrackNode::SetEndState(const EndStateT &set)
        {
            if (set.GetVariantType() != variantType)
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
            return endState.GetEnding();
        }

        void TrackNode::SetTimeTaken(const TimeValue &set)
        {
            timeTaken = set;
        }

        const TimeValue& TrackNode::GetTimeTaken() const
        {
            return timeTaken;
        }

        TrackNode::EndStateT TrackNode::PrototypeEndState() const
        {
            return EndStateT(variantType);
        }
    }
}