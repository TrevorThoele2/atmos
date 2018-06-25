#pragma once

#include "FalconScriptObjectPrototype.h"

#include "Modulator.h"
#include "ModulatorController.h"
#include "ComponentTypeName.h"
#include "ScriptCaller.h"
#include "InputDefines.h"

#define DECLARE_CONSTRUCTORS(T)         \
T();                                    \
T(const T &arg);                        \
T(T &&arg);                             \
T& operator=(const T &arg) = default;   \
T& operator=(T &&arg);

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
                    static PropertyPrototype<Logger::Type> errorSevere;
                    static PropertyPrototype<Logger::Type> errorModerate;
                    static PropertyPrototype<Logger::Type> errorLow;
                    static PropertyPrototype<Logger::Type> warning;
                    static PropertyPrototype<Logger::Type> information;
                };

                class FixedPoint : public Definition<FixedPoint>
                {
                public:
                    static const Name className;
                    PropertyPrototype<std::int64_t> value;

                    static Constructor constructor;

                    static Method &fromDecimal;

                    DECLARE_CONSTRUCTORS(FixedPoint);
                };

                class Pair : public Definition<Pair>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Falcon::Item> first;
                    PropertyPrototype<::Falcon::Item> second;

                    static Constructor constructor;

                    DECLARE_CONSTRUCTORS(Pair);
                };
            }

            namespace Position
            {
                class GridPosition : public Definition<GridPosition>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::GridPosition::ValueT> x;
                    PropertyPrototype<::Atmos::GridPosition::ValueT> y;
                    PropertyPrototype<::Atmos::GridPosition::ValueT> z;

                    static Constructor constructor;

                    static Method &distance;
                    static Method &convertToPosition3D;

                    DECLARE_CONSTRUCTORS(GridPosition);
                };

                class Position3D : public Definition<Position3D>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Position3D::ValueT> x;
                    PropertyPrototype<::Atmos::Position3D::ValueT> y;
                    PropertyPrototype<::Atmos::Position3D::ValueT> z;

                    static Constructor constructor;

                    static Method &distance;
                    static Method &convertToGridPosition;

                    DECLARE_CONSTRUCTORS(Position3D);
                };

                class Direction : public Definition<Direction>
                {
                public:
                    static const Name className;
                    static PropertyPrototype<::Atmos::Direction::ValueT> left;
                    static PropertyPrototype<::Atmos::Direction::ValueT> up;
                    static PropertyPrototype<::Atmos::Direction::ValueT> right;
                    static PropertyPrototype<::Atmos::Direction::ValueT> down;
                };
            }

            namespace Input
            {
                class ActionID : public Definition<ActionID>
                {
                public:
                    static const Name className;
                    static PropertyPrototype<::Atmos::Input::ActionID> moveUp;
                    static PropertyPrototype<::Atmos::Input::ActionID> moveDown;
                    static PropertyPrototype<::Atmos::Input::ActionID> moveLeft;
                    static PropertyPrototype<::Atmos::Input::ActionID> moveRight;
                    static PropertyPrototype<::Atmos::Input::ActionID> use;
                    static PropertyPrototype<::Atmos::Input::ActionID> navigateMenuUp;
                    static PropertyPrototype<::Atmos::Input::ActionID> navigateMenuDown;
                    static PropertyPrototype<::Atmos::Input::ActionID> navigateMenuLeft;
                    static PropertyPrototype<::Atmos::Input::ActionID> navigateMenuRight;
                    static PropertyPrototype<::Atmos::Input::ActionID> changeSelectedCharacterLeft;
                    static PropertyPrototype<::Atmos::Input::ActionID> changeSelectedCharacterRight;
                    static PropertyPrototype<::Atmos::Input::ActionID> inventory;
                    static PropertyPrototype<::Atmos::Input::ActionID> endTurn;
                    static PropertyPrototype<::Atmos::Input::ActionID> attack;
                    static PropertyPrototype<::Atmos::Input::ActionID> openSpells;
                    static PropertyPrototype<::Atmos::Input::ActionID> cancel;
                    static PropertyPrototype<::Atmos::Input::ActionID> stats;
                };
            }

            namespace Time
            {
                class Epoch : public Definition<Epoch>
                {
                public:
                    static const Name className;
                    static PropertyPrototype<::Atmos::TimeValueEpoch> minutes;
                    static PropertyPrototype<::Atmos::TimeValueEpoch> seconds;
                    static PropertyPrototype<::Atmos::TimeValueEpoch> milliseconds;
                    static PropertyPrototype<::Atmos::TimeValueEpoch> microseconds;
                    static PropertyPrototype<::Atmos::TimeValueEpoch> nanoseconds;
                };

                class TimeValue : public Definition<TimeValue>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::TimeValueEpoch> epoch;
                    PropertyPrototype<General::FixedPoint> value;

                    static Constructor constructor;

                    DECLARE_CONSTRUCTORS(TimeValue);
                };

                class FrameTimer : public Definition<FrameTimer>
                {
                public:
                    static const Name className;
                    PropertyPrototype<bool> isRunning;
                    PropertyPrototype<TimeValue> startValue;
                    PropertyPrototype<TimeValue> goal;
                    PropertyPrototype<::Atmos::TimeValueEpoch> epoch;

                    static Constructor constructor;

                    static Method &start;
                    static Method &stop;
                    static Method &isActive;
                    static Method &getElapsed;
                    static Method &hasReachedGoal;

                    DECLARE_CONSTRUCTORS(FrameTimer);
                };
            }

            namespace Modulator
            {
                class Type : public Definition<Type>
                {
                public:
                    static const Name className;
                    static PropertyPrototype<Name> sprite;
                    static PropertyPrototype<Name> spriteOffset;
                    static PropertyPrototype<Name> sound;
                    static PropertyPrototype<Name> soundOffset;
                    static PropertyPrototype<Name> avEffect;
                    static PropertyPrototype<Name> avEffectOffset;
                    static PropertyPrototype<Name> senseComponent;
                };

                class TrackType : public Definition<TrackType>
                {
                public:
                    static const Name className;
                    static PropertyPrototype<Name> positionX;
                    static PropertyPrototype<Name> positionY;
                    static PropertyPrototype<Name> positionZ;
                    static PropertyPrototype<Name> scalingX;
                    static PropertyPrototype<Name> scalingY;
                    static PropertyPrototype<Name> scalingZ;
                    static PropertyPrototype<Name> rotationX;
                    static PropertyPrototype<Name> rotationY;
                    static PropertyPrototype<Name> rotationZ;
                    static PropertyPrototype<Name> colorA;
                    static PropertyPrototype<Name> colorR;
                    static PropertyPrototype<Name> colorG;
                    static PropertyPrototype<Name> colorB;
                    static PropertyPrototype<Name> index;
                    static PropertyPrototype<Name> volume;
                };

                class Modulator : public Definition<Modulator>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Modulator::Controller::ID> modID;

                    static Constructor constructor;

                    static Method &createTrack;
                    static Method &forceStop;

                    DECLARE_CONSTRUCTORS(Modulator);
                };

                class Track : public Definition<Track>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Modulator::Controller::ID> modID;
                    PropertyPrototype<::Atmos::Modulator::ModulatorBase::TrackID> trackID;

                    static Constructor constructor;

                    static Method &createNode;
                    static Method &setDefaultStartValue;
                    static Method &resetDefaultStartValue;

                    DECLARE_CONSTRUCTORS(Track);
                };

                class TrackNode : public Definition<TrackNode>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Modulator::Controller::ID> modID;
                    PropertyPrototype<::Atmos::Modulator::ModulatorBase::TrackID> trackID;
                    PropertyPrototype<::Atmos::Modulator::TrackBase::NodeID> nodeID;

                    static Constructor constructor;

                    static Method &setInterpolationLinear;
                    static Method &setInterpolationOscillation;
                    static Method &setEndState;
                    static Method &setTimeTaken;

                    DECLARE_CONSTRUCTORS(TrackNode);
                };
            }

            namespace Ent
            {
                class ComponentType : public Definition<ComponentType>
                {
                public:
                    static const Name className;
                    static PropertyPrototype<::Atmos::Ent::ComponentTypeName> action;
                    static PropertyPrototype<::Atmos::Ent::ComponentTypeName> battle;
                    static PropertyPrototype<::Atmos::Ent::ComponentTypeName> ai;
                    static PropertyPrototype<::Atmos::Ent::ComponentTypeName> avatar;
                    static PropertyPrototype<::Atmos::Ent::ComponentTypeName> character;
                    static PropertyPrototype<::Atmos::Ent::ComponentTypeName> combat;
                    static PropertyPrototype<::Atmos::Ent::ComponentTypeName> dialogue;
                    static PropertyPrototype<::Atmos::Ent::ComponentTypeName> general;
                    static PropertyPrototype<::Atmos::Ent::ComponentTypeName> inventory;
                    static PropertyPrototype<::Atmos::Ent::ComponentTypeName> movement;
                    static PropertyPrototype<::Atmos::Ent::ComponentTypeName> sense;
                };

                class ActionComponent : public Definition<ActionComponent>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Entity::ValueT> entity;

                    static Constructor constructor;

                    DECLARE_CONSTRUCTORS(ActionComponent);
                };

                class BattleComponent : public Definition<BattleComponent>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Entity::ValueT> entity;

                    static Constructor constructor;

                    DECLARE_CONSTRUCTORS(BattleComponent);
                };

                class AIComponent : public Definition<AIComponent>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Entity::ValueT> entity;

                    static Constructor constructor;

                    DECLARE_CONSTRUCTORS(AIComponent);
                };

                class AvatarComponent : public Definition<AvatarComponent>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Entity::ValueT> entity;

                    static Constructor constructor;

                    DECLARE_CONSTRUCTORS(AvatarComponent);
                };

                class CharacterComponent : public Definition<CharacterComponent>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Entity::ValueT> entity;

                    static Constructor constructor;

                    DECLARE_CONSTRUCTORS(CharacterComponent);
                };

                class CombatComponent : public Definition<CombatComponent>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Entity::ValueT> entity;

                    static Constructor constructor;

                    DECLARE_CONSTRUCTORS(CombatComponent);
                };

                class DialogueComponent : public Definition<DialogueComponent>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Entity::ValueT> entity;

                    static Constructor constructor;

                    DECLARE_CONSTRUCTORS(DialogueComponent);
                };

                class GeneralComponent : public Definition<GeneralComponent>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Entity::ValueT> entity;

                    static Constructor constructor;

                    DECLARE_CONSTRUCTORS(GeneralComponent);
                };

                class InventoryComponent : public Definition<InventoryComponent>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Entity::ValueT> entity;

                    static Constructor constructor;

                    DECLARE_CONSTRUCTORS(InventoryComponent);
                };

                class MovementComponent : public Definition<MovementComponent>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Entity::ValueT> entity;

                    static Constructor constructor;

                    static Method &moveDirection;
                    static Method &moveInstant;
                    static Method &canMove;
                    static Method &completeMovementSetup;

                    DECLARE_CONSTRUCTORS(MovementComponent);
                };

                class SenseComponent : public Definition<SenseComponent>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Entity::ValueT> entity;

                    static Constructor constructor;

                    static Method &startModulatorWithSprite;
                    static Method &startModulatorWithSound;
                    static Method &getPosition;
                    static Method &getStagedPosition;

                    DECLARE_CONSTRUCTORS(SenseComponent);
                };

                class Entity : public Definition<Entity>
                {
                public:
                    static const Name className;
                    PropertyPrototype<::Atmos::Entity::ValueT> value;

                    static Constructor constructor;

                    static Method &hasComponent;
                    static Method &getComponent;
                    static Method &getPosition;

                    DECLARE_CONSTRUCTORS(Entity);
                };
            }
        }
    }
}

#undef DECLARE_CONSTRUCTORS