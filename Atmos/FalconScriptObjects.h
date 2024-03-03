#pragma once

#include "FalconScriptObjectPrototype.h"

namespace Atmos
{
    namespace Fal
    {
        class GridPosition : public Prototype<GridPosition>
        {
        public:
            static const Name className;
            static Property<::Atmos::GridPosition::ValueT> x;
            static Property<::Atmos::GridPosition::ValueT> y;
            static Property<::Atmos::GridPosition::ValueT> z;

            static Constructor constructor;
            static Method distance;
        };

        class FrameTimer : public Prototype<FrameTimer>
        {
        public:
            static const Name className;

            static Constructor constructor;
        };

        class Modulator : public Prototype<Modulator>
        {
        public:
            static const Name className;

            static Constructor constructor;
        };

        class ModulatorTrack : public Prototype<ModulatorTrack>
        {
        public:
            static const Name className;

            static Constructor constructor;
        };

        class ModulatorTrackNode : public Prototype<ModulatorTrackNode>
        {
        public:
            static const Name className;

            static Constructor constructor;
        };
    }
}