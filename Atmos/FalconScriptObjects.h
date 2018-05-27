#pragma once

#include "FalconScriptObjectPrototype.h"

#include "Modulator.h"
#include "ModulatorController.h"

namespace Atmos
{
    namespace Fal
    {
        namespace Classes
        {
            namespace General
            {
                class LogType : public Prototype<LogType>
                {
                public:
                    static const Name className;
                    static Property<Logger::Type> errorSevere;
                    static Property<Logger::Type> errorModerate;
                    static Property<Logger::Type> errorLow;
                    static Property<Logger::Type> warning;
                    static Property<Logger::Type> information;
                };

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
            }

            namespace Time
            {
                class FrameTimer : public Prototype<FrameTimer>
                {
                public:
                    static const Name className;

                    static Constructor constructor;
                };
            }

            namespace Modulator
            {
                class Type : public Prototype<Type>
                {
                public:
                    static const Name className;
                    static Property<Name> sprite;
                    static Property<Name> spriteOffset;
                    static Property<Name> sound;
                    static Property<Name> soundOffset;
                    static Property<Name> avEffect;
                    static Property<Name> avEffectOffset;
                };

                class TrackType : public Prototype<TrackType>
                {
                public:
                    static const Name className;
                    static Property<Name> positionX;
                    static Property<Name> positionY;
                    static Property<Name> positionZ;
                    static Property<Name> scalingX;
                    static Property<Name> scalingY;
                    static Property<Name> scalingZ;
                    static Property<Name> rotationX;
                    static Property<Name> rotationY;
                    static Property<Name> rotationZ;
                };

                class Modulator : public Prototype<Modulator>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Modulator::Controller::ID> modID;

                    static Constructor constructor;

                    static Method createTrack;
                };

                class Track : public Prototype<Track>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Modulator::Controller::ID> modID;
                    static Property<::Atmos::Modulator::ModulatorBase::TrackID> trackID;

                    static Constructor constructor;

                    static Method createNode;
                };

                class TrackNode : public Prototype<TrackNode>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Modulator::Controller::ID> modID;
                    static Property<::Atmos::Modulator::ModulatorBase::TrackID> trackID;
                    static Property<::Atmos::Modulator::TrackBase::NodeID> nodeID;

                    static Constructor constructor;
                };
            }

            namespace Ent
            {
                class SenseComponent : public Prototype<SenseComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity> entity;

                    static Constructor constructor;

                    static Method startModulatorWithSprite;
                    static Method startModulatorWithSound;
                };
            }
        }
    }
}