
#include "FalconScriptObjects.h"

#include "GameEnvironment.h"
#include "CurrentField.h"

#include "ModulatorDescribers.h"
#include "ActionComponent.h"
#include "BattleComponent.h"
#include "AIComponent.h"
#include "AvatarComponent.h"
#include "CharacterComponent.h"
#include "CombatComponent.h"
#include "DialogueComponent.h"
#include "GeneralComponent.h"
#include "InventoryComponent.h"
#include "MovementComponent.h"
#include "SenseComponent.h"

#include <falcon/engine.h>

#define CLASS_NAME(classT, name)    \
    const Name classT::className(name);

#define PROPERTY(classT, T, name, ...)   \
    classT::Property<T> classT::name(#name, __VA_ARGS__);
#define PROPERTY_SPLIT(classT, T, varName, falconName, ...)   \
    classT::Property<T> classT::varName(falconName, __VA_ARGS__);

#define METHOD(classT, methodName, falconMethodName, isStatic)      \
    classT::Method classT::methodName(falconMethodName, isStatic,

#define PARAMETERS(...)         \
    std::vector<ParameterBase*>{__VA_ARGS__

#define NO_PARAMETERS           \
    std::vector<ParameterBase*>{

#define BODY(classT)                                                \
    }, static_cast<classT::Method::FalconFuncT>([](Falcon::VMachine *vm)

#define BODY_END \
    ))

#define CONSTRUCTOR(classT)                 \
    classT::Constructor classT::constructor(

#define SETUP_FUNCTION(className, func)     \
auto &selfFunc = func;                      \
if (!selfFunc.Setup(*vm))                   \
    return;

#define SETUP_CONSTRUCTOR(className) SETUP_FUNCTION(className, constructor)

namespace Atmos
{
    namespace Fal
    {
        namespace Classes
        {
            namespace General
            {
                CLASS_NAME(CallerType, "Atmos_CallerType");
                    PROPERTY_SPLIT(CallerType, ScriptCaller::Type, empty, "Empty", ScriptCaller::Type::EMPTY, true);
                    PROPERTY_SPLIT(CallerType, ScriptCaller::Type, entity, "Entity", ScriptCaller::Type::ENTITY, true);

                CLASS_NAME(Caller, "Atmos_Caller")
                METHOD(Caller, isEmpty, "IsEmpty", true)
                    NO_PARAMETERS
                BODY(Caller)
                {
                    auto script = ScriptController::Current();
                    if (!script)
                        vm->retval(false);

                    vm->retval(script->GetCaller().IsEmpty());
                } BODY_END;
                METHOD(Caller, isEntity, "IsEntity", true)
                    NO_PARAMETERS
                BODY(Caller)
                {
                    auto script = ScriptController::Current();
                    if (!script)
                        vm->retval(false);

                    vm->retval(script->GetCaller().IsEntity());
                } BODY_END;
                METHOD(Caller, getEntity, "GetEntity", true)
                    NO_PARAMETERS
                BODY(Caller)
                {
                    auto script = ScriptController::Current();
                    if (!script || !script->GetCaller().IsEntity())
                        vm->retnil();

                    Ent::Entity::value.value = script->GetCaller().GetEntity();
                    vm->retval(Ent::Entity::CreateItem(*vm));
                } BODY_END;

                CLASS_NAME(LogType, "Atmos_LogType");
                    PROPERTY_SPLIT(LogType, Logger::Type, errorSevere, "ErrorSevere", Logger::Type::ERROR_SEVERE, true);
                    PROPERTY_SPLIT(LogType, Logger::Type, errorModerate, "ErrorModerate", Logger::Type::ERROR_MODERATE, true);
                    PROPERTY_SPLIT(LogType, Logger::Type, errorLow, "ErrorLow", Logger::Type::ERROR_LOW, true);
                    PROPERTY_SPLIT(LogType, Logger::Type, warning, "Warning", Logger::Type::WARNING, true);
                    PROPERTY_SPLIT(LogType, Logger::Type, information, "Information", Logger::Type::INFORMATION, true);

                CLASS_NAME(GridPosition, "Atmos_GridPosition");
                    PROPERTY(GridPosition, ::Atmos::GridPosition::ValueT, x);
                    PROPERTY(GridPosition, ::Atmos::GridPosition::ValueT, y);
                    PROPERTY(GridPosition, ::Atmos::GridPosition::ValueT, z);
                // GridPosition constructor
                CONSTRUCTOR(GridPosition)
                    PARAMETERS(new Parameter<::Atmos::GridPosition::ValueT>("x"), new Parameter<::Atmos::GridPosition::ValueT>("y"), new Parameter<::Atmos::GridPosition::ValueT>("z"))
                    BODY(GridPosition)
                {
                    SETUP_CONSTRUCTOR(GridPosition);
                    x.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(x)->value);
                    y.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(y)->value);
                    z.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(z)->value);
                } BODY_END;
                // Distance method
                METHOD(GridPosition, distance, "Distance", false)
                    PARAMETERS(new Parameter<GridPosition>("other"))
                BODY(GridPosition)
                {
                    SETUP_FUNCTION(GridPosition, distance);

                    auto &other = static_cast<Parameter<GridPosition>*>(distance.parameters.Get("other")->get())->value;
                    vm->retval(
                        std::abs(other.GetProperty(x)->value - GridPosition::x.value) +
                        std::abs(other.GetProperty(y)->value - GridPosition::y.value) +
                        std::abs(other.GetProperty(z)->value - GridPosition::z.value));
                } BODY_END;
            }

            namespace Time
            {
                // FrameTimer constructor
                CONSTRUCTOR(FrameTimer)
                    PARAMETERS()
                BODY(FrameTimer)
                {
                    //constructor.SetAllPropertiesToParameters(*vm);
                } BODY_END;
            }

            namespace Modulator
            {
                // ModulatorType
                CLASS_NAME(Type, "AtmosModulator_Type");
                    PROPERTY_SPLIT(Type, Name, sprite, ::Atmos::Modulator::Description::Sprite.name, ::Atmos::Modulator::Description::Sprite.name, true);
                    PROPERTY_SPLIT(Type, Name, spriteOffset, ::Atmos::Modulator::Description::SpriteOffset.name, ::Atmos::Modulator::Description::SpriteOffset.name, true);
                    PROPERTY_SPLIT(Type, Name, sound, ::Atmos::Modulator::Description::Sound.name, ::Atmos::Modulator::Description::Sound.name, true);
                    PROPERTY_SPLIT(Type, Name, soundOffset, ::Atmos::Modulator::Description::SoundOffset.name, ::Atmos::Modulator::Description::SoundOffset.name, true);
                    PROPERTY_SPLIT(Type, Name, avEffect, ::Atmos::Modulator::Description::AVEffect.name, ::Atmos::Modulator::Description::AVEffect.name, true);
                    PROPERTY_SPLIT(Type, Name, avEffectOffset, ::Atmos::Modulator::Description::AVEffectOffset.name, ::Atmos::Modulator::Description::AVEffectOffset.name, true);

                // ModulatorTrackType
                CLASS_NAME(TrackType, "AtmosModulator_TrackType");
                    PROPERTY_SPLIT(TrackType, Name, positionX, ::Atmos::Modulator::Description::Track::PositionX.name, ::Atmos::Modulator::Description::Track::PositionX.name, true);
                    PROPERTY_SPLIT(TrackType, Name, positionY, ::Atmos::Modulator::Description::Track::PositionY.name, ::Atmos::Modulator::Description::Track::PositionY.name, true);
                    PROPERTY_SPLIT(TrackType, Name, positionZ, ::Atmos::Modulator::Description::Track::PositionZ.name, ::Atmos::Modulator::Description::Track::PositionZ.name, true);
                    PROPERTY_SPLIT(TrackType, Name, scalingX, ::Atmos::Modulator::Description::Track::ScalingX.name, ::Atmos::Modulator::Description::Track::ScalingX.name, true);
                    PROPERTY_SPLIT(TrackType, Name, scalingY, ::Atmos::Modulator::Description::Track::ScalingY.name, ::Atmos::Modulator::Description::Track::ScalingY.name, true);
                    PROPERTY_SPLIT(TrackType, Name, scalingZ, ::Atmos::Modulator::Description::Track::ScalingZ.name, ::Atmos::Modulator::Description::Track::ScalingZ.name, true);
                    PROPERTY_SPLIT(TrackType, Name, rotationX, ::Atmos::Modulator::Description::Track::RotationX.name, ::Atmos::Modulator::Description::Track::RotationX.name, true);
                    PROPERTY_SPLIT(TrackType, Name, rotationY, ::Atmos::Modulator::Description::Track::RotationY.name, ::Atmos::Modulator::Description::Track::RotationY.name, true);
                    PROPERTY_SPLIT(TrackType, Name, rotationZ, ::Atmos::Modulator::Description::Track::RotationZ.name, ::Atmos::Modulator::Description::Track::RotationZ.name, true);

                // Modulator
                CLASS_NAME(Modulator, "AtmosModulator");
                    PROPERTY(Modulator, ::Atmos::Modulator::Controller::ID, modID);
                CONSTRUCTOR(Modulator)
                    PARAMETERS(new Parameter<::Atmos::Modulator::Controller::ID>("modID"))
                BODY(Modulator)
                {
                    SETUP_CONSTRUCTOR(Modulator);

                    modID.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(modID)->value);
                } BODY_END;
                METHOD(Modulator, createTrack, "CreateTrack", false)
                    PARAMETERS(new Parameter<Name>("name"))
                BODY(Modulator)
                {
                    SETUP_FUNCTION(Modulator, createTrack);
                    auto name = selfFunc.GetParameter<Name>("name");
                    auto &modID = Modulator::modID;

                    ::Atmos::Modulator::Observer mod(GameEnvironment::FindModulator(modID.value));
                    if (!mod)
                    {
                        // Log error if not found
                        Logger::Log("A modulator was attempted to be found with an invalid ID.",
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("ID", modID.value) });
                        return;
                    }

                    auto &madeTrack = GameEnvironment::GenerateModulatorTrack(mod->GetGeneratorName(), name->value);
                    if (!madeTrack)
                    {
                        // Log error if not made
                        Logger::Log("A modulator track was attempted to be created with an invalid name.",
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Track Name", name) });
                        return;
                    }

                    // Make track
                    auto madeTrackID = mod->AddTrack(std::move(madeTrack));
                    // Setup the Track class, then create it as an item and return
                    Track::modID.value = modID.value;
                    Track::trackID.value = madeTrackID;
                    vm->retval(Track::CreateItem(*vm));
                } BODY_END;

                // ModulatorTrack
                CLASS_NAME(Track, "AtmosModulator_Track");
                    PROPERTY(Track, ::Atmos::Modulator::Controller::ID, modID);
                    PROPERTY(Track, ::Atmos::Modulator::ModulatorBase::TrackID, trackID);
                CONSTRUCTOR(Track)
                    PARAMETERS(new Parameter<::Atmos::Modulator::Controller::ID>("modID"), new Parameter<::Atmos::Modulator::ModulatorBase::TrackID>("trackID"))
                BODY(Track)
                {
                    SETUP_CONSTRUCTOR(Track);

                    modID.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(modID)->value);
                    trackID.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(trackID)->value);
                } BODY_END;
                METHOD(Track, createNode, "CreateNode", false)
                    NO_PARAMETERS
                BODY(Track)
                {
                    SETUP_FUNCTION(Track, createNode);
                    auto &modID = Track::modID;
                    auto &trackID = Track::trackID;

                    ::Atmos::Modulator::Observer foundMod = GameEnvironment::FindModulator(modID.value);
                    if (!foundMod)
                    {
                        // Log error if not found
                        Logger::Log("A modulator was attempted to be found with an invalid ID.",
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Mod ID", modID.value) });
                        return;
                    }

                    auto foundTrack = foundMod->FindTrack(trackID.value);
                    if (!foundTrack)
                    {
                        // Log error if not made
                        Logger::Log("A modulator track was attempted to be found with an invalid ID.",
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Track ID", trackID.value) });
                        return;
                    }

                    // Make node
                    auto nodeID = foundTrack->AddNode();
                    // Setup the TrackNode class, then create it as an item and return
                    TrackNode::modID.value = modID.value;
                    TrackNode::trackID.value = trackID.value;
                    TrackNode::nodeID.value = nodeID;
                    vm->retval(TrackNode::CreateItem(*vm));
                } BODY_END;

                // ModulatorTrackNode
                CLASS_NAME(TrackNode, "AtmosModulator_TrackNode");
                    PROPERTY(TrackNode, ::Atmos::Modulator::Controller::ID, modID);
                    PROPERTY(TrackNode, ::Atmos::Modulator::ModulatorBase::TrackID, trackID);
                    PROPERTY(TrackNode, ::Atmos::Modulator::TrackBase::NodeID, nodeID);
                CONSTRUCTOR(TrackNode)
                    PARAMETERS(new Parameter<::Atmos::Modulator::Controller::ID>("modID"), new Parameter<::Atmos::Modulator::ModulatorBase::TrackID>("trackID"), new Parameter<::Atmos::Modulator::TrackBase::NodeID>("nodeID"))
                BODY(TrackNode)
                {
                    SETUP_CONSTRUCTOR(TrackNode);
                    modID.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(modID)->value);
                    trackID.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(trackID)->value);
                    nodeID.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(nodeID)->value);
                } BODY_END;
            }

            namespace Ent
            {
                CLASS_NAME(ComponentType, "AtmosEntity_ComponentType");
                    PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, action, "Action", ::Atmos::Ent::ActionComponent::TypeName(), true);
                    PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, battle, "Battle", ::Atmos::Ent::BattleComponent::TypeName(), true);
                    PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, ai, "AI", ::Atmos::Ent::AIComponent::TypeName(), true);
                    PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, avatar, "Avatar", ::Atmos::Ent::AvatarComponent::TypeName(), true);
                    PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, character, "Character", ::Atmos::Ent::CharacterComponent::TypeName(), true);
                    PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, combat, "Combat", ::Atmos::Ent::CombatComponent::TypeName(), true);
                    PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, dialogue, "Dialogue", ::Atmos::Ent::DialogueComponent::TypeName(), true);
                    PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, general, "General", ::Atmos::Ent::GeneralComponent::TypeName(), true);
                    PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, inventory, "Inventory", ::Atmos::Ent::InventoryComponent::TypeName(), true);
                    PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, movement, "Movement", ::Atmos::Ent::MovementComponent::TypeName(), true);
                    PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, sense, "Sense", ::Atmos::Ent::SenseComponent::TypeName(), true);

                // ActionComponent
                CLASS_NAME(ActionComponent, "AtmosEntity_ActionComponent");
                    PROPERTY(ActionComponent, ::Atmos::Entity, entity);
                CONSTRUCTOR(ActionComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity>("entity"))
                BODY(ActionComponent)
                {
                    SETUP_CONSTRUCTOR(ActionComponent);
                    entity.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(entity)->value);
                } BODY_END;

                // BattleComponent
                CLASS_NAME(BattleComponent, "AtmosEntity_BattleComponent");
                    PROPERTY(BattleComponent, ::Atmos::Entity, entity);
                CONSTRUCTOR(BattleComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity>("entity"))
                BODY(BattleComponent)
                {
                    SETUP_CONSTRUCTOR(BattleComponent);
                    entity.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(entity)->value);
                } BODY_END;

                // AIComponent
                CLASS_NAME(AIComponent, "AtmosEntity_AIComponent");
                    PROPERTY(AIComponent, ::Atmos::Entity, entity);
                CONSTRUCTOR(AIComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity>("entity"))
                BODY(AIComponent)
                {
                    SETUP_CONSTRUCTOR(AIComponent);
                    entity.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(entity)->value);
                } BODY_END;

                // AvatarComponent
                CLASS_NAME(AvatarComponent, "AtmosEntity_AvatarComponent");
                    PROPERTY(AvatarComponent, ::Atmos::Entity, entity);
                CONSTRUCTOR(AvatarComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity>("entity"))
                BODY(AvatarComponent)
                {
                    SETUP_CONSTRUCTOR(AvatarComponent);
                    entity.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(entity)->value);
                } BODY_END;

                // CharacterComponent
                CLASS_NAME(CharacterComponent, "AtmosEntity_CharacterComponent");
                    PROPERTY(CharacterComponent, ::Atmos::Entity, entity);
                CONSTRUCTOR(CharacterComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity>("entity"))
                BODY(CharacterComponent)
                {
                    SETUP_CONSTRUCTOR(CharacterComponent);
                    entity.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(entity)->value);
                } BODY_END;

                // CombatComponent
                CLASS_NAME(CombatComponent, "AtmosEntity_CombatComponent");
                    PROPERTY(CombatComponent, ::Atmos::Entity, entity);
                CONSTRUCTOR(CombatComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity>("entity"))
                BODY(CombatComponent)
                {
                    SETUP_CONSTRUCTOR(CombatComponent);
                    entity.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(entity)->value);
                } BODY_END;

                // DialogueComponent
                CLASS_NAME(DialogueComponent, "AtmosEntity_DialogueComponent");
                    PROPERTY(DialogueComponent, ::Atmos::Entity, entity);
                CONSTRUCTOR(DialogueComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity>("entity"))
                BODY(DialogueComponent)
                {
                    SETUP_CONSTRUCTOR(DialogueComponent);
                    entity.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(entity)->value);
                } BODY_END;

                // GeneralComponent
                CLASS_NAME(GeneralComponent, "AtmosEntity_GeneralComponent");
                    PROPERTY(GeneralComponent, ::Atmos::Entity, entity);
                CONSTRUCTOR(GeneralComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity>("entity"))
                BODY(GeneralComponent)
                {
                    SETUP_CONSTRUCTOR(GeneralComponent);
                    entity.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(entity)->value);
                } BODY_END;

                // InventoryComponent
                CLASS_NAME(InventoryComponent, "AtmosEntity_InventoryComponent");
                    PROPERTY(InventoryComponent, ::Atmos::Entity, entity);
                CONSTRUCTOR(InventoryComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity>("entity"))
                BODY(InventoryComponent)
                {
                    SETUP_CONSTRUCTOR(InventoryComponent);
                    entity.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(entity)->value);
                } BODY_END;

                // MovementComponent
                CLASS_NAME(MovementComponent, "AtmosEntity_MovementComponent");
                    PROPERTY(MovementComponent, ::Atmos::Entity, entity);
                CONSTRUCTOR(MovementComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity>("entity"))
                BODY(MovementComponent)
                {
                    SETUP_CONSTRUCTOR(MovementComponent);
                    entity.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(entity)->value);
                } BODY_END;

                // SenseComponent
                CLASS_NAME(SenseComponent, "AtmosEntity_SenseComponent");
                    PROPERTY(SenseComponent, ::Atmos::Entity, entity);
                CONSTRUCTOR(SenseComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity>("entity"))
                BODY(SenseComponent)
                {
                    SETUP_CONSTRUCTOR(SenseComponent);
                    entity.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(entity)->value);
                } BODY_END;
                // Attach modulator to sprite
                METHOD(SenseComponent, startModulatorWithSprite, "StartModulatorWithSprite", false)
                    PARAMETERS(new Parameter<Modulator::Modulator>("attach"), new Parameter<::Atmos::Ent::SenseComponent::SpriteList::ID>("spriteID"), new Parameter<bool>("force", false, false))
                BODY(SenseComponent)
                {
                    SETUP_FUNCTION(SenseComponent, startModulatorWithSprite);

                    auto attach = selfFunc.GetParameter<Modulator::Modulator>("attach");
                    auto spriteID = selfFunc.GetParameter<::Atmos::Ent::SenseComponent::SpriteList::ID>("spriteID");
                    auto force = selfFunc.GetParameter<bool>("force");

                    // Get modulator
                    auto modulator = ::Atmos::GameEnvironment::FindModulator(attach->value.modID.value);
                    if (!modulator)
                    {
                        Logger::Log("A modulator was required but wasn't found.",
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Modulator ID", attach->value.modID.value) });
                        return;
                    }
                    
                    // Make sure modulator is the right type
                    if (modulator->GetGeneratorName() != ::Atmos::Modulator::Description::SpriteOffset.name)
                    {
                        Logger::Log("The modulator is the incorrect type.",
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Modulator ID", attach->value.modID.value),
                                                     NameValuePair("Required Type", ::Atmos::Modulator::Description::SpriteOffset.name),
                                                     NameValuePair("Actual Type", modulator->GetGeneratorName()) });
                        return;
                    }

                    // Get entity
                    auto &entity = SenseComponent::entity.value;
                    // Get sense component
                    auto senseComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::SenseComponent>(entity);
                    if (!senseComponent)
                    {
                        Logger::Log("A sense component was required but wasn't found.",
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)) });
                        return;
                    }

                    // Get the sprite out of the sense component
                    auto foundSprite = senseComponent->FindSprite(spriteID->value);
                    if (!foundSprite)
                    {
                        Logger::Log("A sense component did not have a required sprite.",
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)),
                                                     NameValuePair("Sprite ID", spriteID) });
                    }

                    // Make sure the sprite is not being affected by another modulator, EXCEPT if we're forcing
                    if (force->WasSet() && force->value && GameEnvironment::IsModulatorWorkingObject(foundSprite))
                        // Return silently
                        return;

                    // Finally, attach the modulator
                    auto castedModulator = static_cast<decltype(::Atmos::Modulator::Description::SpriteOffset)::ModulatorT*>(modulator.Get());
                    castedModulator->Start(*foundSprite);
                } BODY_END;

                // Entity
                CLASS_NAME(Entity, "AtmosEntity");
                    PROPERTY(Entity, ::Atmos::Entity, value);
                CONSTRUCTOR(Entity)
                    PARAMETERS(new Parameter<::Atmos::Entity>("value"))
                BODY(Entity)
                {
                    SETUP_CONSTRUCTOR(Entity);
                    value.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(value)->value);
                } BODY_END;
                // Has component
                METHOD(Entity, hasComponent, "HasComponent", false)
                    PARAMETERS(new Parameter<::Atmos::Ent::ComponentTypeName>("typeName"))
                BODY(Entity)
                {
                    SETUP_FUNCTION(Entity, hasComponent);

                    auto typeName = selfFunc.GetParameter<::Atmos::Ent::ComponentTypeName>("typeName");
                    // Get value
                    auto &entity = Entity::value.value;
                    // Return if we could find the component
                    vm->retval(GetCurrentEntities()->FindComponent(entity, typeName->value));
                } BODY_END;

                template<class ComponentClassT>
                bool AttemptCreateComponent(::Atmos::Entity entity, const ::Atmos::Ent::ComponentTypeName &check, const ::Atmos::Ent::ComponentTypeName &checkAgainst, Falcon::VMachine &vm)
                {
                    if (check != checkAgainst)
                        return false;

                    ComponentClassT::entity.value = entity;
                    vm.retval(ComponentClassT::CreateItem(vm));
                    return true;
                }

                // Get component
                METHOD(Entity, getComponent, "GetComponent", false)
                    PARAMETERS(new Parameter<::Atmos::Ent::ComponentTypeName>("typeName"))
                BODY(Entity)
                {
                    SETUP_FUNCTION(Entity, getComponent);

                    auto typeName = selfFunc.GetParameter<::Atmos::Ent::ComponentTypeName>("typeName");
                    // Get value
                    auto &entity = Entity::value.value;
                    // Find component
                    auto found = GetCurrentEntities()->FindComponent(entity, typeName->value);
                    // If we didn't find the component, return nil
                    if (!found)
                    {
                        vm->retnil();
                        return;
                    }

#define ATTEMPT_CREATE_COMPONENT(ComponentClassT)                                                                       \
if (AttemptCreateComponent<ComponentClassT>(entity, typeName->value, ::Atmos::Ent::ComponentClassT::TypeName(), *vm))   \
    return;

                    ATTEMPT_CREATE_COMPONENT(ActionComponent)
                    else ATTEMPT_CREATE_COMPONENT(BattleComponent)
                    else ATTEMPT_CREATE_COMPONENT(AIComponent)
                    else ATTEMPT_CREATE_COMPONENT(AvatarComponent)
                    else ATTEMPT_CREATE_COMPONENT(CharacterComponent)
                    else ATTEMPT_CREATE_COMPONENT(CombatComponent)
                    else ATTEMPT_CREATE_COMPONENT(DialogueComponent)
                    else ATTEMPT_CREATE_COMPONENT(GeneralComponent)
                    else ATTEMPT_CREATE_COMPONENT(InventoryComponent)
                    else ATTEMPT_CREATE_COMPONENT(MovementComponent)
                    else ATTEMPT_CREATE_COMPONENT(SenseComponent)

#undef ATTEMPT_CREATE_COMPONENT

                } BODY_END;
            }
        }
    }
}