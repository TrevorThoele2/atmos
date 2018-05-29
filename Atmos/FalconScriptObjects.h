#pragma once

#include "FalconScriptObjectPrototype.h"

#include "Modulator.h"
#include "ModulatorController.h"
#include "ComponentTypeName.h"
#include "ScriptCaller.h"

namespace Atmos
{
    namespace Fal
    {
        namespace Classes
        {
            namespace General
            {
                class CallerType : public Prototype<CallerType>
                {
                public:
                    static const Name className;
                    static Property<ScriptCaller::Type> empty;
                    static Property<ScriptCaller::Type> entity;
                };

                class Caller : public Prototype<Caller>
                {
                public:
                    static const Name className;

                    static Method isEmpty;
                    static Method isEntity;
                    static Method getEntity;
                };

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
                class ComponentType : public Prototype<ComponentType>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Ent::ComponentTypeName> action;
                    static Property<::Atmos::Ent::ComponentTypeName> battle;
                    static Property<::Atmos::Ent::ComponentTypeName> ai;
                    static Property<::Atmos::Ent::ComponentTypeName> avatar;
                    static Property<::Atmos::Ent::ComponentTypeName> character;
                    static Property<::Atmos::Ent::ComponentTypeName> combat;
                    static Property<::Atmos::Ent::ComponentTypeName> dialogue;
                    static Property<::Atmos::Ent::ComponentTypeName> general;
                    static Property<::Atmos::Ent::ComponentTypeName> inventory;
                    static Property<::Atmos::Ent::ComponentTypeName> movement;
                    static Property<::Atmos::Ent::ComponentTypeName> sense;
                };

                class ActionComponent : public Prototype<ActionComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity> entity;

                    static Constructor constructor;
                };

                class BattleComponent : public Prototype<BattleComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity> entity;

                    static Constructor constructor;
                };

                class AIComponent : public Prototype<AIComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity> entity;

                    static Constructor constructor;
                };

                class AvatarComponent : public Prototype<AvatarComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity> entity;

                    static Constructor constructor;
                };

                class CharacterComponent : public Prototype<CharacterComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity> entity;

                    static Constructor constructor;
                };

                class CombatComponent : public Prototype<CombatComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity> entity;

                    static Constructor constructor;
                };

                class DialogueComponent : public Prototype<DialogueComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity> entity;

                    static Constructor constructor;
                };

                class GeneralComponent : public Prototype<GeneralComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity> entity;

                    static Constructor constructor;
                };

                class InventoryComponent : public Prototype<InventoryComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity> entity;

                    static Constructor constructor;
                };

                class MovementComponent : public Prototype<MovementComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity> entity;

                    static Constructor constructor;
                };

                class SenseComponent : public Prototype<SenseComponent>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity> entity;

                    static Constructor constructor;

                    static Method startModulatorWithSprite;
                    static Method startModulatorWithSound;
                };

                class Entity : public Prototype<Entity>
                {
                public:
                    static const Name className;
                    static Property<::Atmos::Entity> value;

                    static Constructor constructor;

                    static Method hasComponent;
                    static Method getComponent;
                };
            }
        }
    }
}