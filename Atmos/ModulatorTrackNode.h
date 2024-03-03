#pragma once

#include "Name.h"
#include "ModulatorDelta.h"
#include "ModulatorValue.h"
#include "Random.h"
#include "FrameTimer.h"
#include <Function/Function.h>
#include <Function/Variant.h>

namespace Atmos
{
    namespace Modulator
    {
        class TrackNodeInterpolation
        {
        public:
            typedef TimeValue::ValueT Delta;

            typedef float Acceleration;
            typedef float Rate;

            enum Type : unsigned char
            {
                LINEAR,
                OSCILLATION,
                NONE
            };
        private:
            class Linear
            {
            private:
                INSCRIPTION_SERIALIZE_FUNCTION{}
                INSCRIPTION_ACCESS;
            public:
                Linear();
                bool operator==(const Linear &arg) const;
                bool operator!=(const Linear &arg) const;
            };

            class Oscillation
            {
            private:
                INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
                INSCRIPTION_ACCESS;
            public:
                Acceleration acceleration;
                Rate rate;
                Oscillation();
                Oscillation(Acceleration acceleration, Rate rate);
                bool operator==(const Oscillation &arg) const;
                bool operator!=(const Oscillation &arg) const;
            };
        private:
            // The sequence of the types here needs to be the same as the Type enum
            typedef ::function::Variant<Linear, Oscillation> VariantT;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            VariantT variant;
        public:
            TrackNodeInterpolation();
            TrackNodeInterpolation(Acceleration acceleration, Rate rate);
            TrackNodeInterpolation(const TrackNodeInterpolation &arg);
            TrackNodeInterpolation(TrackNodeInterpolation &&arg);
            TrackNodeInterpolation& operator=(const TrackNodeInterpolation &arg);
            TrackNodeInterpolation& operator=(TrackNodeInterpolation &&arg);
            bool operator==(const TrackNodeInterpolation &arg) const;
            bool operator!=(const TrackNodeInterpolation &arg) const;

            void SetLinear();
            void SetOscillation(Acceleration acceleration, Rate rate);
            Delta TransformDelta(Delta delta) const;
            Type GetType() const;
        };

        class TrackNodeEndState
        {
        public:
            typedef Range<Value> RangeT;

            enum Type : unsigned char
            {
                NORMAL,
                RANDOM,
                NONE
            };
        private:
            class Normal
            {
            private:
                INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
                INSCRIPTION_ACCESS;
            public:
                Value end;
                Normal();
                Normal(Value end);
                Normal(const Normal &arg) = default;
                Normal& operator=(const Normal &arg) = default;
                bool operator==(const Normal &arg) const;
                bool operator!=(const Normal &arg) const;
            };

            class Random
            {
            private:
                INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
                INSCRIPTION_ACCESS;
            public:
                mutable Value pickedValue;
                RangeT range;
                Random();
                Random(const RangeT &range);
                Random(const Random &arg) = default;
                Random& operator=(const Random &arg) = default;
                bool operator==(const Random &arg) const;
                bool operator!=(const Random &arg) const;
            };
        private:
            // The sequence of the types here needs to be the same as the Type enum
            typedef ::function::Variant<Normal, Random> VariantT;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            VariantT variant;
            Value::Type variantType;
        public:
            TrackNodeEndState(Value::Type variantType);
            TrackNodeEndState(const TrackNodeEndState &arg);
            TrackNodeEndState(TrackNodeEndState &&arg);
            TrackNodeEndState& operator=(const TrackNodeEndState &arg);
            TrackNodeEndState& operator=(TrackNodeEndState &&arg);
            bool operator==(const TrackNodeEndState &arg) const;
            bool operator!=(const TrackNodeEndState &arg) const;

            void SetNormal(Value end);
            void SetRandom(const RangeT &range);
            Value GetEnding() const;
            Type GetType() const;
            Value::Type GetVariantType() const;
        };

        class TrackNode
        {
        public:
            typedef TrackNodeEndState EndStateT;
            typedef TrackNodeInterpolation InterpolationT;
        private:
            EndStateT endState;
            InterpolationT interpolation;
            TimeValue timeTaken;
            Value::Type variantType;
        public:
            TrackNode(Value::Type variantType);
            TrackNode(const TrackNode &arg) = default;
            TrackNode(TrackNode &&arg);
            TrackNode& operator=(const TrackNode &arg) = default;
            TrackNode& operator=(TrackNode &&arg);
            bool operator==(const TrackNode &arg) const;
            bool operator!=(const TrackNode &arg) const;

            void SetEndState(const EndStateT &set);
            void SetInterpolation(const InterpolationT &set);
            Delta GetDelta(const FrameTimer &timer) const;
            Value GetEndValue() const;

            void SetTimeTaken(const TimeValue &set);
            const TimeValue& GetTimeTaken() const;
            
            // Use this to get an end state with the correct variant type
            EndStateT PrototypeEndState() const;
        };
    }
}