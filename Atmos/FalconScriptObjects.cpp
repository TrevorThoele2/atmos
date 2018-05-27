
#include "FalconScriptObjects.h"
#include "GameEnvironment.h"
#include "ModulatorDescribers.h"
#include "CurrentField.h"
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
    std::vector<PropertyBase*>{__VA_ARGS__

#define NO_PARAMETERS           \
    std::vector<PropertyBase*>{

#define BODY(classT)                                                \
    }, static_cast<classT::Method::FalconFuncT>([](Falcon::VMachine *vm)

#define BODY_END \
    ))

#define CONSTRUCTOR(classT)                 \
    classT::Constructor classT::constructor(

#define SETUP_FUNCTION(className, selfObj)  \
auto &selfFunc = selfObj;                   \
if (!selfFunc.Setup(*vm))                   \
    return;

namespace Atmos
{
    namespace Fal
    {
        namespace Classes
        {
            namespace General
            {
                CLASS_NAME(LogType, "Atmos_LogType");
                PROPERTY(LogType, Logger::Type, errorSevere, Logger::Type::ERROR_SEVERE);
                PROPERTY(LogType, Logger::Type, errorModerate, Logger::Type::ERROR_MODERATE);
                PROPERTY(LogType, Logger::Type, errorLow, Logger::Type::ERROR_LOW);
                PROPERTY(LogType, Logger::Type, warning, Logger::Type::WARNING);
                PROPERTY(LogType, Logger::Type, information, Logger::Type::INFORMATION);

                CLASS_NAME(GridPosition, "Atmos_GridPosition");
                PROPERTY(GridPosition, ::Atmos::GridPosition::ValueT, x);
                PROPERTY(GridPosition, ::Atmos::GridPosition::ValueT, y);
                PROPERTY(GridPosition, ::Atmos::GridPosition::ValueT, z);
                // Distance method
                METHOD(GridPosition, distance, "Distance", false)
                    PARAMETERS(new Property<GridPosition>("other"))
                BODY(GridPosition)
                {
                    SETUP_FUNCTION(GridPosition, distance);

                    auto &self = *distance.self;
                    auto &other = static_cast<Property<GridPosition>*>(distance.parameters.Get("other")->get())->value;
                    vm->retval(
                        std::abs(other.GetProperty(x)->value - self.GetProperty(x)->value) +
                        std::abs(other.GetProperty(y)->value - self.GetProperty(y)->value) +
                        std::abs(other.GetProperty(z)->value - self.GetProperty(z)->value));
                } BODY_END;

                // GridPosition constructor
                CONSTRUCTOR(GridPosition)
                    PARAMETERS(new Property<::Atmos::GridPosition::ValueT>("x"), new Property<::Atmos::GridPosition::ValueT>("y"), new Property<::Atmos::GridPosition::ValueT>("z"))
                BODY(GridPosition)
                {
                    GetProperty(x)->value = constructor.GetParameter(x)->value;
                    GetProperty(y)->value = constructor.GetParameter(y)->value;
                    GetProperty(z)->value = constructor.GetParameter(z)->value;
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
                PROPERTY_SPLIT(Type, Name, sprite, ::Atmos::Modulator::Description::Sprite.name, ::Atmos::Modulator::Description::Sprite.name);
                PROPERTY_SPLIT(Type, Name, spriteOffset, ::Atmos::Modulator::Description::SpriteOffset.name, ::Atmos::Modulator::Description::SpriteOffset.name);
                PROPERTY_SPLIT(Type, Name, sound, ::Atmos::Modulator::Description::Sound.name, ::Atmos::Modulator::Description::Sound.name);
                PROPERTY_SPLIT(Type, Name, soundOffset, ::Atmos::Modulator::Description::SoundOffset.name, ::Atmos::Modulator::Description::SoundOffset.name);
                PROPERTY_SPLIT(Type, Name, avEffect, ::Atmos::Modulator::Description::AVEffect.name, ::Atmos::Modulator::Description::AVEffect.name);
                PROPERTY_SPLIT(Type, Name, avEffectOffset, ::Atmos::Modulator::Description::AVEffectOffset.name, ::Atmos::Modulator::Description::AVEffectOffset.name);

                // ModulatorTrackType
                CLASS_NAME(TrackType, "AtmosModulator_TrackType");
                PROPERTY_SPLIT(TrackType, Name, positionX, ::Atmos::Modulator::Description::Track::PositionX.name, ::Atmos::Modulator::Description::Track::PositionX.name);
                PROPERTY_SPLIT(TrackType, Name, positionY, ::Atmos::Modulator::Description::Track::PositionY.name, ::Atmos::Modulator::Description::Track::PositionY.name);
                PROPERTY_SPLIT(TrackType, Name, positionZ, ::Atmos::Modulator::Description::Track::PositionZ.name, ::Atmos::Modulator::Description::Track::PositionZ.name);
                PROPERTY_SPLIT(TrackType, Name, scalingX, ::Atmos::Modulator::Description::Track::ScalingX.name, ::Atmos::Modulator::Description::Track::ScalingX.name);
                PROPERTY_SPLIT(TrackType, Name, scalingY, ::Atmos::Modulator::Description::Track::ScalingY.name, ::Atmos::Modulator::Description::Track::ScalingY.name);
                PROPERTY_SPLIT(TrackType, Name, scalingZ, ::Atmos::Modulator::Description::Track::ScalingZ.name, ::Atmos::Modulator::Description::Track::ScalingZ.name);
                PROPERTY_SPLIT(TrackType, Name, rotationX, ::Atmos::Modulator::Description::Track::RotationX.name, ::Atmos::Modulator::Description::Track::RotationX.name);
                PROPERTY_SPLIT(TrackType, Name, rotationY, ::Atmos::Modulator::Description::Track::RotationY.name, ::Atmos::Modulator::Description::Track::RotationY.name);
                PROPERTY_SPLIT(TrackType, Name, rotationZ, ::Atmos::Modulator::Description::Track::RotationZ.name, ::Atmos::Modulator::Description::Track::RotationZ.name);

                // Modulator
                CLASS_NAME(Modulator, "AtmosModulator");
                PROPERTY(Modulator, ::Atmos::Modulator::Controller::ID, modID);
                CONSTRUCTOR(Modulator)
                    PARAMETERS(new Property<::Atmos::Modulator::Controller::ID>("modID"))
                BODY(Modulator)
                {
                    GetProperty(modID)->value = constructor.GetParameter(modID)->value;
                } BODY_END;
                METHOD(Modulator, createTrack, "CreateTrack", false)
                    PARAMETERS(new Property<Name>("name"))
                BODY(Modulator)
                {
                    SETUP_FUNCTION(Modulator, createTrack);
                    auto &name = selfFunc.GetParameter<Name>("name")->value;
                    auto &modID = Modulator::modID.value;

                    ::Atmos::Modulator::Observer mod(GameEnvironment::FindModulator(modID));
                    if (!mod)
                    {
                        // Log error if not found
                        Logger::Log("A modulator was attempted to be found with an invalid ID.",
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("ID", modID) });
                        return;
                    }

                    auto &madeTrack = GameEnvironment::GenerateModulatorTrack(mod->GetGeneratorName(), name);
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
                    Track::modID.value = modID;
                    Track::trackID.value = madeTrackID;
                    vm->retval(Track::CreateItem(*vm));
                } BODY_END;

                // ModulatorTrack
                CLASS_NAME(Track, "AtmosModulator_Track");
                PROPERTY(Track, ::Atmos::Modulator::Controller::ID, modID);
                PROPERTY(Track, ::Atmos::Modulator::ModulatorBase::TrackID, trackID);
                CONSTRUCTOR(Track)
                    PARAMETERS(new Property<::Atmos::Modulator::Controller::ID>("modID"), new Property<::Atmos::Modulator::ModulatorBase::TrackID>("trackID"))
                BODY(Track)
                {
                    GetProperty(modID)->value = constructor.GetParameter(modID)->value;
                    GetProperty(trackID)->value = constructor.GetParameter(trackID)->value;
                } BODY_END;
                METHOD(Track, createNode, "CreateNode", false)
                    NO_PARAMETERS
                BODY(Track)
                {
                    SETUP_FUNCTION(Track, createNode);
                    auto &modID = Track::modID.value;
                    auto &trackID = Track::trackID.value;

                    ::Atmos::Modulator::Observer foundMod = GameEnvironment::FindModulator(modID);
                    if (!foundMod)
                    {
                        // Log error if not found
                        Logger::Log("A modulator was attempted to be found with an invalid ID.",
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Mod ID", modID) });
                        return;
                    }

                    auto foundTrack = foundMod->FindTrack(trackID);
                    if (!foundTrack)
                    {
                        // Log error if not made
                        Logger::Log("A modulator track was attempted to be found with an invalid ID.",
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Track ID", trackID) });
                        return;
                    }

                    // Make node
                    auto nodeID = foundTrack->AddNode();
                    // Setup the TrackNode class, then create it as an item and return
                    TrackNode::modID.value = modID;
                    TrackNode::trackID.value = trackID;
                    TrackNode::nodeID.value = nodeID;
                    vm->retval(TrackNode::CreateItem(*vm));
                } BODY_END;

                // ModulatorTrackNode
                CLASS_NAME(TrackNode, "AtmosModulator_TrackNode");
                PROPERTY(TrackNode, ::Atmos::Modulator::Controller::ID, modID);
                PROPERTY(TrackNode, ::Atmos::Modulator::ModulatorBase::TrackID, trackID);
                PROPERTY(TrackNode, ::Atmos::Modulator::TrackBase::NodeID, nodeID);
                CONSTRUCTOR(TrackNode)
                    PARAMETERS(new Property<::Atmos::Modulator::Controller::ID>("modID"), new Property<::Atmos::Modulator::ModulatorBase::TrackID>("trackID"), new Property<::Atmos::Modulator::TrackBase::NodeID>("nodeID"))
                BODY(TrackNode)
                {
                    GetProperty(modID)->value = constructor.GetParameter(modID)->value;
                    GetProperty(trackID)->value = constructor.GetParameter(trackID)->value;
                    GetProperty(nodeID)->value = constructor.GetParameter(nodeID)->value;
                } BODY_END;
            }

            namespace Ent
            {
                // SenseComponent
                CLASS_NAME(SenseComponent, "AtmosEntity_SenseComponent");
                PROPERTY(SenseComponent, ::Atmos::Entity, entity);
                CONSTRUCTOR(SenseComponent)
                    PARAMETERS(new Property<::Atmos::Entity>("entity"))
                BODY(SenseComponent)
                {
                    GetProperty(entity)->value = constructor.GetParameter(entity)->value;
                } BODY_END;
                // Attach modulator to sprite
                METHOD(SenseComponent, startModulatorWithSprite, "StartModulatorWithSprite", false)
                    PARAMETERS(new Property<Modulator::Modulator>("attach"), new Property<::Atmos::Ent::SenseComponent::SpriteList::ID>("spriteID"))
                BODY(SenseComponent)
                {
                    SETUP_FUNCTION(SenseComponent, startModulatorWithSprite);

                    auto &self = *selfFunc.self;
                    auto &attach = selfFunc.GetParameter<Modulator::Modulator>("attach")->value;
                    auto &spriteID = selfFunc.GetParameter<::Atmos::Ent::SenseComponent::SpriteList::ID>("spriteID")->value;

                    // Get modulator
                    auto modulator = ::Atmos::GameEnvironment::FindModulator(attach.modID.value);
                    if (!modulator)
                    {
                        Logger::Log("A modulator was required but wasn't found.",
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Modulator ID", attach.modID.value) });
                        return;
                    }
                    
                    // Make sure modulator is the right type
                    if (modulator->GetGeneratorName() != ::Atmos::Modulator::Description::SpriteOffset.name)
                    {
                        Logger::Log("The modulator is the incorrect type.",
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Modulator ID", attach.modID.value),
                                                     NameValuePair("Required Type", ::Atmos::Modulator::Description::SpriteOffset.name),
                                                     NameValuePair("Actual Type", modulator->GetGeneratorName()) });
                        return;
                    }

                    // Get entity
                    auto &entity = self.GetProperty(SenseComponent::entity)->value;
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
                    auto foundSprite = senseComponent->FindSprite(spriteID);
                    if (!foundSprite)
                    {
                        Logger::Log("A sense component did not have a required sprite.",
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)),
                                                     NameValuePair("Sprite ID", spriteID) });
                    }

                    // Finally, attach the modulator
                    auto castedModulator = static_cast<decltype(::Atmos::Modulator::Description::SpriteOffset)::ModulatorT*>(modulator.Get());
                    castedModulator->Start(*foundSprite);
                } BODY_END;
            }
        }
    }
}