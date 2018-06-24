#pragma once

#include "FalconScriptObjectPrototype.h"

#include "Modulator.h"
#include "ModulatorController.h"
#include "ComponentTypeName.h"
#include "ScriptCaller.h"
#include "InputDefines.h"

namespace Atmos
{
    namespace Fal
    {
        namespace Classes
        {
            namespace General
            {
                class Caller : public Definition<Caller>
                {
                public:
                    static const Name className;

                    static Method &isEmpty;
                    static Method &isEntity;
                    static Method &getEntity;
                };

                class LogType : public Definition<LogType>
                {
                public:
                    static const Name className;
                    static Property<Logger::Type> &errorSevere;
                    static Property<Logger::Type> &errorModerate;
                    static Property<Logger::Type> &errorLow;
                    static Property<Logger::Type> &warning;
                    static Property<Logger::Type> &information;
                };

                class FixedPoint : public Definition<FixedPoint>
                {
                public:
                    static const Name className;
                    static Property<std::int64_t> &value;

                    static Constructor constructor;

                    static Method &fromDecimal;
                };

                class Pair : public Definition<Pair>
                {
                public:
                    static const Name className;
                    static Property<::Falcon::Item> &first;
                    static Property<::Falcon::Item> &second;

                    static Constructor constructor;
                };
            }

            namespace Position
            {
                class GridPosition : public Definition<GridPosition>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::GridPosition::ValueT> &x;
                    static Property<::Atmos::GridPosition::ValueT> &y;
                    static Property<::Atmos::GridPosition::ValueT> &z;

                    static Constructor constructor;

                    static Method &distance;
                    static Method &convertToPosition3D;
                };

                class Position3D : public Definition<Position3D>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Position3D::ValueT> &x;
                    static Property<::Atmos::Position3D::ValueT> &y;
                    static Property<::Atmos::Position3D::ValueT> &z;

                    static Constructor constructor;

                    static Method &distance;
                    static Method &convertToGridPosition;
                };

                class Direction : public Definition<Direction>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Direction::ValueT> &left;
                    static Property<::Atmos::Direction::ValueT> &up;
                    static Property<::Atmos::Direction::ValueT> &right;
                    static Property<::Atmos::Direction::ValueT> &down;
                };
            }

            namespace Input
            {
                class ActionID : public Definition<ActionID>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Input::ActionID> &moveUp;
                    static Property<::Atmos::Input::ActionID> &moveDown;
                    static Property<::Atmos::Input::ActionID> &moveLeft;
                    static Property<::Atmos::Input::ActionID> &moveRight;
                    static Property<::Atmos::Input::ActionID> &use;
                    static Property<::Atmos::Input::ActionID> &navigateMenuUp;
                    static Property<::Atmos::Input::ActionID> &navigateMenuDown;
                    static Property<::Atmos::Input::ActionID> &navigateMenuLeft;
                    static Property<::Atmos::Input::ActionID> &navigateMenuRight;
                    static Property<::Atmos::Input::ActionID> &changeSelectedCharacterLeft;
                    static Property<::Atmos::Input::ActionID> &changeSelectedCharacterRight;
                    static Property<::Atmos::Input::ActionID> &inventory;
                    static Property<::Atmos::Input::ActionID> &endTurn;
                    static Property<::Atmos::Input::ActionID> &attack;
                    static Property<::Atmos::Input::ActionID> &openSpells;
                    static Property<::Atmos::Input::ActionID> &cancel;
                    static Property<::Atmos::Input::ActionID> &stats;
                };
            }

            namespace Time
            {
                class Epoch : public Definition<Epoch>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::TimeValueEpoch> &minutes;
                    static Property<::Atmos::TimeValueEpoch> &seconds;
                    static Property<::Atmos::TimeValueEpoch> &milliseconds;
                    static Property<::Atmos::TimeValueEpoch> &microseconds;
                    static Property<::Atmos::TimeValueEpoch> &nanoseconds;
                };

                class TimeValue : public Definition<TimeValue>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::TimeValueEpoch> &epoch;
                    static Property<General::FixedPoint> &value;

                    static Constructor constructor;
                };

                class FrameTimer : public Definition<FrameTimer>
                {
                public:
                    static const Name className;
                    static Property<bool> &isRunning;
                    static Property<TimeValue> &startProp;
                    static Property<TimeValue> &goal;
                    static Property<::Atmos::TimeValueEpoch> &epoch;

                    static Constructor constructor;

                    static Method &start;
                    static Method &stop;
                    static Method &isActive;
                    static Method &getElapsed;
                    static Method &hasReachedGoal;
                };
            }

            namespace Modulator
            {
                class Type : public Definition<Type>
                {
                public:
                    static const Name className;
                    static Property<Name> &sprite;
                    static Property<Name> &spriteOffset;
                    static Property<Name> &sound;
                    static Property<Name> &soundOffset;
                    static Property<Name> &avEffect;
                    static Property<Name> &avEffectOffset;
                    static Property<Name> &senseComponent;
                };

                class TrackType : public Definition<TrackType>
                {
                public:
                    static const Name className;
                    static Property<Name> &positionX;
                    static Property<Name> &positionY;
                    static Property<Name> &positionZ;
                    static Property<Name> &scalingX;
                    static Property<Name> &scalingY;
                    static Property<Name> &scalingZ;
                    static Property<Name> &rotationX;
                    static Property<Name> &rotationY;
                    static Property<Name> &rotationZ;
                };

                class Modulator : public Definition<Modulator>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Modulator::Controller::ID> &modID;

                    static Constructor constructor;

                    static Method &createTrack;
                    static Method &forceStop;
                };

                class Track : public Definition<Track>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Modulator::Controller::ID> &modID;
                    static Property<::Atmos::Modulator::ModulatorBase::TrackID> &trackID;

                    static Constructor constructor;

                    static Method &createNode;
                };

                class TrackNode : public Definition<TrackNode>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Modulator::Controller::ID> &modID;
                    static Property<::Atmos::Modulator::ModulatorBase::TrackID> &trackID;
                    static Property<::Atmos::Modulator::TrackBase::NodeID> &nodeID;

                    static Constructor constructor;

                    static Method &setInterpolationLinear;
                    static Method &setInterpolationOscillation;
                    static Method &setEndStateNormal;
                    static Method &setEndStateRandom;
                    static Method &setTimeTaken;
                };
            }

            namespace Ent
            {
                class ComponentType : public Definition<ComponentType>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Ent::ComponentTypeName> &action;
                    static Property<::Atmos::Ent::ComponentTypeName> &battle;
                    static Property<::Atmos::Ent::ComponentTypeName> &ai;
                    static Property<::Atmos::Ent::ComponentTypeName> &avatar;
                    static Property<::Atmos::Ent::ComponentTypeName> &character;
                    static Property<::Atmos::Ent::ComponentTypeName> &combat;
                    static Property<::Atmos::Ent::ComponentTypeName> &dialogue;
                    static Property<::Atmos::Ent::ComponentTypeName> &general;
                    static Property<::Atmos::Ent::ComponentTypeName> &inventory;
                    static Property<::Atmos::Ent::ComponentTypeName> &movement;
                    static Property<::Atmos::Ent::ComponentTypeName> &sense;
                };

                class ActionComponent : public Definition<ActionComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity::ValueT> &entity;

                    static Constructor constructor;
                };

                class BattleComponent : public Definition<BattleComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity::ValueT> &entity;

                    static Constructor constructor;
                };

                class AIComponent : public Definition<AIComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity::ValueT> &entity;

                    static Constructor constructor;
                };

                class AvatarComponent : public Definition<AvatarComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity::ValueT> &entity;

                    static Constructor constructor;
                };

                class CharacterComponent : public Definition<CharacterComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity::ValueT> &entity;

                    static Constructor constructor;
                };

                class CombatComponent : public Definition<CombatComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity::ValueT> &entity;

                    static Constructor constructor;
                };

                class DialogueComponent : public Definition<DialogueComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity::ValueT> &entity;

                    static Constructor constructor;
                };

                class GeneralComponent : public Definition<GeneralComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity::ValueT> &entity;

                    static Constructor constructor;
                };

                class InventoryComponent : public Definition<InventoryComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity::ValueT> &entity;

                    static Constructor constructor;
                };

                class MovementComponent : public Definition<MovementComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity::ValueT> &entity;

                    static Constructor constructor;

                    static Method &moveDirection;
                    static Method &moveInstant;
                    static Method &canMove;
                    static Method &completeMovementSetup;
                };

                class SenseComponent : public Definition<SenseComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity::ValueT> &entity;

                    static Constructor constructor;

                    static Method &startModulatorWithSprite;
                    static Method &startModulatorWithSound;
                    static Method &getPosition;
                    static Method &getStagedPosition;
                };

                class Entity : public Definition<Entity>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity::ValueT> &value;

                    static Constructor constructor;

                    static Method &hasComponent;
                    static Method &getComponent;
                    static Method &getPosition;
                };
            }
        }
    }
}