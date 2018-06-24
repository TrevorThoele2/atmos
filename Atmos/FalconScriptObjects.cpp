
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
#include "EntityPositionSystem.h"

#include <falcon/engine.h>

template<class T>
T* AddPropertyReturner(Atmos::Fal::Classes::Prototype &prototype, T *add)
{
    prototype.AddProperty(add);
    return add;
}

Atmos::Fal::Classes::Prototype::Method* AddMethodReturner(Atmos::Fal::Classes::Prototype &prototype, Atmos::Fal::Classes::Prototype::Method *add)
{
    prototype.AddMethod(add);
    return add;
}

#define CLASS_NAME(classT, name)    \
    const Name classT::className(name);

#define PROPERTY(classT, T, name, ...)   \
    classT::Property<T>& classT::name(*AddPropertyReturner(classT::Instance(), new classT::Property<T>(#name, __VA_ARGS__)));
#define PROPERTY_SPLIT(classT, T, varName, falconName, ...)   \
    classT::Property<T>& classT::varName(*AddPropertyReturner(classT::Instance(), new classT::Property<T>(falconName, __VA_ARGS__)));

#define METHOD(classT, methodName, falconMethodName, isStatic)      \
    classT::Method& classT::methodName(*AddMethodReturner(classT::Instance(), new classT::Method(classT::Instance(), falconMethodName, isStatic,

#define PARAMETERS(...)         \
    std::vector<ParameterBase*>{__VA_ARGS__

#define NO_PARAMETERS           \
    std::vector<ParameterBase*>{

#define BODY(classT)                                                \
    }, static_cast<classT::Method::FalconFuncT>([](Falcon::VMachine *vm)

#define BODY_END \
    ))

#define METHOD_END \
    ))))

#define CONSTRUCTOR(classT)                                     \
    classT::Constructor classT::constructor(classT::Instance(),

#define SETUP_FUNCTION(className, func)     \
auto &selfFunc = func;                      \
if (!selfFunc.Setup(*vm))                   \
    return;

#define SETUP_CONSTRUCTOR(className)        \
auto &selfFunc = className::constructor;    \
if (!selfFunc.SetupParameters(*vm))         \
    return;

namespace Atmos
{
    namespace Fal
    {
        namespace Classes
        {
            namespace General
            {
                CLASS_NAME(Caller, "Atmos_Caller")
                METHOD(Caller, isEmpty, "IsEmpty", true)
                    NO_PARAMETERS
                BODY(Caller)
                {
                    auto script = ScriptController::Current();
                    if (!script)
                        vm->retval(false);

                    vm->retval(script->GetCaller().IsEmpty());
                } METHOD_END;
                METHOD(Caller, isEntity, "IsEntity", true)
                    NO_PARAMETERS
                BODY(Caller)
                {
                    auto script = ScriptController::Current();
                    if (!script)
                        vm->retval(false);

                    vm->retval(script->GetCaller().IsEntity());
                } METHOD_END;
                METHOD(Caller, getEntity, "GetEntity", true)
                    NO_PARAMETERS
                BODY(Caller)
                {
                    auto script = ScriptController::Current();
                    if (!script || !script->GetCaller().IsEntity())
                        vm->retnil();

                    Ent::Entity::value.obj = script->GetCaller().GetEntity();
                    vm->retval(Ent::Entity::Instance().CreateItem(*vm));
                } METHOD_END;

                CLASS_NAME(LogType, "Atmos_LogType");
                    PROPERTY_SPLIT(LogType, Logger::Type, errorSevere, "ErrorSevere", Logger::Type::ERROR_SEVERE, true);
                    PROPERTY_SPLIT(LogType, Logger::Type, errorModerate, "ErrorModerate", Logger::Type::ERROR_MODERATE, true);
                    PROPERTY_SPLIT(LogType, Logger::Type, errorLow, "ErrorLow", Logger::Type::ERROR_LOW, true);
                    PROPERTY_SPLIT(LogType, Logger::Type, warning, "Warning", Logger::Type::WARNING, true);
                    PROPERTY_SPLIT(LogType, Logger::Type, information, "Information", Logger::Type::INFORMATION, true);

                CLASS_NAME(FixedPoint, "Atmos_FixedPoint");
                    PROPERTY_SPLIT(FixedPoint, std::int64_t, value, "value");
                CONSTRUCTOR(FixedPoint)
                    PARAMETERS(new Parameter<std::int64_t>("value"))
                BODY(FixedPoint)
                {
                    SETUP_CONSTRUCTOR(FixedPoint);
                    value.SetPropertyTo(*vm, vm->self(), constructor.GetParameter<std::int64_t>("value")->obj);
                } BODY_END;
                METHOD(FixedPoint, fromDecimal, "FromDecimal", true)
                    PARAMETERS(new Parameter<double>("from"))
                BODY(FixedPoint)
                {
                    SETUP_FUNCTION(FixedPoint, fromDecimal);

                    auto from = selfFunc.GetParameter<double>("from");

                    ::Atmos::FixedPoint64 fixedPoint(from->obj);
                    General::FixedPoint::value.obj = fixedPoint.GetRawValue();
                    vm->retval(General::FixedPoint::Instance().CreateItem(*vm));
                } METHOD_END;

                CLASS_NAME(Pair, "Atmos_Pair");
                    PROPERTY(Pair, ::Falcon::Item, first);
                    PROPERTY(Pair, ::Falcon::Item, second);
                CONSTRUCTOR(Pair)
                    PARAMETERS(new Parameter<::Falcon::Item>("first"), new Parameter<::Falcon::Item>("second"))
                BODY(Pair)
                {
                    SETUP_CONSTRUCTOR(Pair);
                    first.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(first)->obj);
                    second.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(second)->obj);
                } BODY_END;
            }

            namespace Position
            {
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
                    x.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(x)->obj);
                    y.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(y)->obj);
                    z.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(z)->obj);
                } BODY_END;
                // Distance method
                METHOD(GridPosition, distance, "Distance", false)
                    PARAMETERS(new Parameter<GridPosition>("other"))
                    BODY(GridPosition)
                {
                    SETUP_FUNCTION(GridPosition, distance);

                    auto &other = static_cast<Parameter<GridPosition>*>(distance.parameters.Get("other")->get())->obj;
                    vm->retval(
                        std::abs(other.GetProperty(x)->obj - *GridPosition::x) +
                        std::abs(other.GetProperty(y)->obj - *GridPosition::y) +
                        std::abs(other.GetProperty(z)->obj - *GridPosition::z));
                } METHOD_END;
                METHOD(GridPosition, convertToPosition3D, "ConvertToPosition3D", false)
                    NO_PARAMETERS
                BODY(GridPosition)
                {
                    SETUP_FUNCTION(GridPosition, convertToPosition3D);

                    Position3D::x.obj = ::Atmos::GridPosition::DimensionToPosition(GridPosition::x.obj);
                    Position3D::y.obj = ::Atmos::GridPosition::DimensionToPosition(GridPosition::y.obj);
                    Position3D::z.obj = ::Atmos::GridPosition::DimensionToPosition(GridPosition::z.obj);
                    vm->retval(Position3D::Instance().CreateItem(*vm));
                } METHOD_END;

                CLASS_NAME(Position3D, "Atmos_Position3D");
                PROPERTY(Position3D, ::Atmos::Position3D::ValueT, x);
                PROPERTY(Position3D, ::Atmos::Position3D::ValueT, y);
                PROPERTY(Position3D, ::Atmos::Position3D::ValueT, z);
                // GridPosition constructor
                CONSTRUCTOR(Position3D)
                    PARAMETERS(new Parameter<::Atmos::Position3D::ValueT>("x"), new Parameter<::Atmos::Position3D::ValueT>("y"), new Parameter<::Atmos::Position3D::ValueT>("z"))
                    BODY(Position3D)
                {
                    SETUP_CONSTRUCTOR(Position3D);
                    x.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(x)->obj);
                    y.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(y)->obj);
                    z.SetPropertyTo(*vm, vm->self(), constructor.GetParameter(z)->obj);
                } BODY_END;
                // Distance method
                METHOD(Position3D, distance, "Distance", false)
                    PARAMETERS(new Parameter<Position3D>("other"))
                    BODY(Position3D)
                {
                    SETUP_FUNCTION(Position3D, distance);

                    auto &other = static_cast<Parameter<Position3D>*>(distance.parameters.Get("other")->get())->obj;
                    vm->retval(
                        std::abs(other.GetProperty(x)->obj - *Position3D::x) +
                        std::abs(other.GetProperty(y)->obj - *Position3D::y) +
                        std::abs(other.GetProperty(z)->obj - *Position3D::z));
                } METHOD_END;
                METHOD(Position3D, convertToGridPosition, "ConvertToGridPosition", false)
                    NO_PARAMETERS
                BODY(Position3D)
                {
                    SETUP_FUNCTION(Position3D, convertToGridPosition);

                    GridPosition::x.obj = ::Atmos::GridPosition::DimensionFromPosition(Position3D::x.obj);
                    GridPosition::y.obj = ::Atmos::GridPosition::DimensionFromPosition(Position3D::y.obj);
                    GridPosition::z.obj = ::Atmos::GridPosition::DimensionFromPosition(Position3D::z.obj);
                    vm->retval(GridPosition::Instance().CreateItem(*vm));
                } METHOD_END;

                CLASS_NAME(Direction, "Atmos_Direction");
                    PROPERTY_SPLIT(Direction, ::Atmos::Direction::ValueT, left, "Left", ::Atmos::Direction::ValueT::LEFT, Readonly(true));
                    PROPERTY_SPLIT(Direction, ::Atmos::Direction::ValueT, up, "Up", ::Atmos::Direction::ValueT::UP, Readonly(true));
                    PROPERTY_SPLIT(Direction, ::Atmos::Direction::ValueT, right, "Right", ::Atmos::Direction::ValueT::RIGHT, Readonly(true));
                    PROPERTY_SPLIT(Direction, ::Atmos::Direction::ValueT, down, "Down", ::Atmos::Direction::ValueT::DOWN, Readonly(true));
            }

            namespace Input
            {
#define ACTION_ID_CREATOR(varName, falconName, enumName)                                                            \
PROPERTY_SPLIT(ActionID, ::Atmos::Input::ActionID, varName, falconName, ::Atmos::Input::ActionID::enumName, true);

                CLASS_NAME(ActionID, "AtmosInput_Action");
                    ACTION_ID_CREATOR(moveUp, "MoveUp", MOVE_UP);
                    ACTION_ID_CREATOR(moveDown, "MoveDown", MOVE_DOWN);
                    ACTION_ID_CREATOR(moveLeft, "MoveLeft", MOVE_LEFT);
                    ACTION_ID_CREATOR(moveRight, "MoveRight", MOVE_RIGHT);
                    ACTION_ID_CREATOR(use, "Use", USE);
                    ACTION_ID_CREATOR(navigateMenuUp, "NavigateMenuUp", NAVIGATE_MENU_UP);
                    ACTION_ID_CREATOR(navigateMenuDown, "NavigateMenuDown", NAVIGATE_MENU_DOWN);
                    ACTION_ID_CREATOR(navigateMenuLeft, "NavigateMenuLeft", NAVIGATE_MENU_LEFT);
                    ACTION_ID_CREATOR(navigateMenuRight, "NavigateMenuRight", NAVIGATE_MENU_RIGHT);
                    ACTION_ID_CREATOR(changeSelectedCharacterLeft, "ChangeSelectedCharacterLeft", CHANGE_SELECTED_CHARACTER_LEFT);
                    ACTION_ID_CREATOR(changeSelectedCharacterRight, "ChangeSelectedCharacterRight", CHANGE_SELECTED_CHARACTER_RIGHT);
                    ACTION_ID_CREATOR(inventory, "Inventory", INVENTORY);
                    ACTION_ID_CREATOR(endTurn, "EndTurn", END_TURN);
                    ACTION_ID_CREATOR(attack, "Attack", ATTACK);
                    ACTION_ID_CREATOR(openSpells, "OpenSpells", OPEN_SPELLS);
                    ACTION_ID_CREATOR(cancel, "Cancel", CANCEL);
                    ACTION_ID_CREATOR(stats, "Stats", STATS);

#undef ACTION_ID_CREATOR
            }

            namespace Time
            {
                CLASS_NAME(Epoch, "AtmosTime_Epoch");
                    PROPERTY_SPLIT(Epoch, ::Atmos::TimeValueEpoch, minutes, "Minutes", ::Atmos::TimeValueEpoch::MINUTES, true);
                    PROPERTY_SPLIT(Epoch, ::Atmos::TimeValueEpoch, seconds, "Seconds", ::Atmos::TimeValueEpoch::SECONDS, true);
                    PROPERTY_SPLIT(Epoch, ::Atmos::TimeValueEpoch, milliseconds, "Milliseconds", ::Atmos::TimeValueEpoch::MILLISECONDS, true);
                    PROPERTY_SPLIT(Epoch, ::Atmos::TimeValueEpoch, microseconds, "Microseconds", ::Atmos::TimeValueEpoch::MICROSECONDS, true);
                    PROPERTY_SPLIT(Epoch, ::Atmos::TimeValueEpoch, nanoseconds, "Nanoseconds", ::Atmos::TimeValueEpoch::NANOSECONDS, true);

                CLASS_NAME(TimeValue, "AtmosTime_Value");
                    PROPERTY_SPLIT(TimeValue, General::FixedPoint, value, "value");
                    PROPERTY_SPLIT(TimeValue, ::Atmos::TimeValueEpoch, epoch, "epoch");
                CONSTRUCTOR(TimeValue)
                    PARAMETERS(new Parameter<General::FixedPoint>("value"), new Parameter<::Atmos::TimeValueEpoch>("epoch"))
                BODY(TimeValue)
                {
                    SETUP_CONSTRUCTOR(TimeValue);
                    value.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(value));
                    epoch.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(epoch));
                } BODY_END;

                // FrameTimer constructor
                CLASS_NAME(FrameTimer, "AtmosTime_FrameTimer")
                    PROPERTY_SPLIT(FrameTimer, bool, isRunning, "_isRunning");
                    PROPERTY_SPLIT(FrameTimer, TimeValue, startProp, "_start", 0, false);
                    PROPERTY_SPLIT(FrameTimer, TimeValue, goal, "_goal", 0);
                    PROPERTY_SPLIT(FrameTimer, ::Atmos::TimeValueEpoch, epoch, "_epoch", ::Atmos::TimeValueEpoch::SECONDS);
                CONSTRUCTOR(FrameTimer)
                    PARAMETERS(new Parameter<TimeValue>("goal", false), new Parameter<TimeValue>("epoch", false))
                BODY(FrameTimer)
                {
                    SETUP_CONSTRUCTOR(FrameTimer);

                    auto goalPass = selfFunc.GetParameter<TimeValue>("goal");
                    auto epochPass = selfFunc.GetParameter<::Atmos::TimeValueEpoch>("epoch");

                    // Set is running
                    isRunning.obj = false;

                    ::Atmos::TimeValueEpoch useEpoch = epochPass->WasSet() ? **constructor.GetParameter<::Atmos::TimeValueEpoch>("epoch") : ::Atmos::TimeValueEpoch::SECONDS;
                    TimeValue::epoch.obj = useEpoch;

                    // Set goal
                    if (!goalPass->WasSet())
                        TimeValue::value->value.obj = 0;
                    goal.SetPropertyTo(*vm, vm->self(), TimeValue::Instance());

                    // Set epoch
                    epoch.SetPropertyTo(*vm, vm->self(), useEpoch);
                } BODY_END;
                METHOD(FrameTimer, start, "Start", false)
                    NO_PARAMETERS
                BODY(FrameTimer)
                {
                    SETUP_FUNCTION(FrameTimer, start);

                    // Set the is running
                    selfFunc.prototype->GetProperty(isRunning)->obj = true;
                    // Set the starting point
                    auto epochUse = selfFunc.prototype->GetProperty(epoch)->obj;
                    auto timeValue = ::Atmos::Act::Time::GetFrameTime();
                    timeValue.Convert(epochUse);

                    TimeValue::epoch.obj = epochUse;
                    TimeValue::value->value.obj = timeValue.Get().GetRawValue();
                    selfFunc.prototype->GetProperty(startProp)->obj = TimeValue::Instance();
                } METHOD_END;
                METHOD(FrameTimer, stop, "Stop", false)
                    NO_PARAMETERS
                BODY(FrameTimer)
                {
                    SETUP_FUNCTION(FrameTimer, stop);

                    // Set the is running
                    selfFunc.prototype->GetProperty(isRunning)->obj = false;
                    // Set the starting point
                    TimeValue::epoch.obj = selfFunc.prototype->GetProperty(epoch)->obj;
                    TimeValue::value->value.obj = 0;
                    selfFunc.prototype->GetProperty(startProp)->obj = TimeValue::Instance();
                } METHOD_END;
                METHOD(FrameTimer, isActive, "IsActive", false)
                    NO_PARAMETERS
                BODY(FrameTimer)
                {
                    SETUP_FUNCTION(FrameTimer, isActive);

                    vm->retval(selfFunc.prototype->GetProperty(isRunning)->obj);
                } METHOD_END;
                METHOD(FrameTimer, getElapsed, "GetElapsed", false)
                    NO_PARAMETERS
                BODY(FrameTimer)
                {
                    SETUP_FUNCTION(FrameTimer, getElapsed);

                    // Get the ending time value
                    auto epochUse = selfFunc.prototype->GetProperty(epoch)->obj;
                    auto timeValue = ::Atmos::Act::Time::GetFrameTime();
                    timeValue.Convert(epochUse);

                    // Get the starting value
                    auto &starting = selfFunc.prototype->GetProperty(startProp)->obj;

                    // Push the elapsed
                    TimeValue::epoch.obj = epochUse;
                    TimeValue::value->value.obj = timeValue.Get().GetRawValue() - starting.value->value.obj;
                    selfFunc.prototype->GetProperty(startProp)->obj = TimeValue::Instance();
                } METHOD_END;
                METHOD(FrameTimer, hasReachedGoal, "HasReachedGoal", false)
                    NO_PARAMETERS
                BODY(FrameTimer)
                {
                    SETUP_FUNCTION(FrameTimer, hasReachedGoal);

                    // Get the ending time value
                    auto epochUse = selfFunc.prototype->GetProperty(epoch)->obj;
                    auto timeValue = ::Atmos::Act::Time::GetFrameTime();
                    timeValue.Convert(epochUse);

                    // Get the starting value
                    auto &starting = selfFunc.prototype->GetProperty(startProp)->obj;

                    // Get the goal
                    auto &goalUse = selfFunc.prototype->GetProperty(goal)->obj;

                    // Push if we've reached the goal yet
                    auto elapsed = timeValue.Get().GetRawValue() - starting.value->value.obj;
                    vm->retval(elapsed >= goalUse.value->value.obj);
                } METHOD_END;
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
                    PROPERTY_SPLIT(Type, Name, senseComponent, ::Atmos::Modulator::Description::SenseComponent.name, ::Atmos::Modulator::Description::SenseComponent.name, true);

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

                    modID.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(modID));
                } BODY_END;
                METHOD(Modulator, createTrack, "CreateTrack", false)
                    PARAMETERS(new Parameter<Name>("name"))
                BODY(Modulator)
                {
                    SETUP_FUNCTION(Modulator, createTrack);
                    auto name = selfFunc.GetParameter<Name>("name");
                    auto &modID = Modulator::modID;

                    ::Atmos::Modulator::Observer mod(GameEnvironment::GetModulatorController().Find(*modID));
                    if (!mod)
                    {
                        // Log error if not found
                        Logger::Log(AddTracebackToString(*vm, "A modulator was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("ID", *modID) });
                        return;
                    }

                    auto &madeTrack = GameEnvironment::GenerateModulatorTrack(mod->GetGeneratorName(), **name);
                    if (!madeTrack)
                    {
                        // Log error if not made
                        Logger::Log(AddTracebackToString(*vm, "A modulator track was attempted to be created with an invalid name."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Track Name", name) });
                        return;
                    }

                    // Make track
                    auto madeTrackID = mod->AddTrack(std::move(madeTrack));
                    // Setup the Track class, then create it as an item and return
                    Track::modID.obj = *modID;
                    Track::trackID.obj = madeTrackID;
                    vm->retval(Track::Instance().CreateItem(*vm));
                } METHOD_END;
                METHOD(Modulator, forceStop, "ForceStop", false)
                    NO_PARAMETERS
                BODY(Modulator)
                {
                    SETUP_FUNCTION(Modulator, forceStop);

                    auto &modID = Modulator::modID;

                    ::Atmos::Modulator::Observer mod(GameEnvironment::GetModulatorController().Find(*modID));
                    if (!mod)
                    {
                        // Log error if not found
                        Logger::Log(AddTracebackToString(*vm, "A modulator was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("ID", *modID) });
                        return;
                    }

                    GameEnvironment::GetModulatorController().ForceStop(mod);
                } METHOD_END;

                // ModulatorTrack
                CLASS_NAME(Track, "AtmosModulator_Track");
                    PROPERTY(Track, ::Atmos::Modulator::Controller::ID, modID);
                    PROPERTY(Track, ::Atmos::Modulator::ModulatorBase::TrackID, trackID);
                CONSTRUCTOR(Track)
                    PARAMETERS(new Parameter<::Atmos::Modulator::Controller::ID>("modID"), new Parameter<::Atmos::Modulator::ModulatorBase::TrackID>("trackID"))
                BODY(Track)
                {
                    SETUP_CONSTRUCTOR(Track);

                    modID.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(modID));
                    trackID.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(trackID));
                } BODY_END;
                METHOD(Track, createNode, "CreateNode", false)
                    NO_PARAMETERS
                BODY(Track)
                {
                    SETUP_FUNCTION(Track, createNode);
                    auto &modID = Track::modID;
                    auto &trackID = Track::trackID;

                    ::Atmos::Modulator::Observer foundMod = GameEnvironment::GetModulatorController().Find(*modID);
                    if (!foundMod)
                    {
                        // Log error if not found
                        Logger::Log(AddTracebackToString(*vm, "A modulator was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Mod ID", *modID) });
                        return;
                    }

                    auto foundTrack = foundMod->FindTrack(trackID.obj);
                    if (!foundTrack)
                    {
                        // Log error if not made
                        Logger::Log(AddTracebackToString(*vm, "A modulator track was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Track ID", *trackID) });
                        return;
                    }

                    // Make node
                    auto nodeID = foundTrack->AddNode();
                    // Setup the TrackNode class, then create it as an item and return
                    TrackNode::modID.obj = *modID;
                    TrackNode::trackID.obj = *trackID;
                    TrackNode::nodeID.obj = nodeID;
                    vm->retval(TrackNode::Instance().CreateItem(*vm));
                } METHOD_END;

                // Returns true if the method should continue
                bool RetrieveNode(Falcon::VMachine &vm, ::Atmos::Modulator::TrackNode *&out, ::Atmos::Modulator::Controller::ID modID, ::Atmos::Modulator::ModulatorBase::TrackID trackID, ::Atmos::Modulator::TrackBase::NodeID nodeID)
                {
                    auto foundMod = GameEnvironment::GetModulatorController().Find(modID);
                    if (!foundMod)
                    {
                        // Log error if not found
                        Logger::Log(AddTracebackToString(vm, "A modulator was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("ID", modID) });
                        return false;
                    }

                    auto foundTrack = foundMod->FindTrack(trackID);
                    if (!foundTrack)
                    {
                        // Log error if not made
                        Logger::Log(AddTracebackToString(vm, "A modulator track was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Track ID", trackID) });
                        return false;
                    }

                    auto foundNode = foundTrack->FindNode(nodeID);
                    if (!foundTrack)
                    {
                        // Log error if not made
                        Logger::Log(AddTracebackToString(vm, "A modulator track node was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Node ID", nodeID) });
                        return false;
                    }

                    out = foundNode;
                    return true;
                }

                bool ExtractModulatorValueFromItem(Falcon::VMachine &vm, ::Falcon::Item &item, ::Atmos::Modulator::Value &focusedChange)
                {
                    if (TraitsT<std::int64_t>::Is(item))
                    {
                        focusedChange.Set(TraitsT<std::int64_t>::FromItem(vm, item));
                        return true;
                    }
                    else if (TraitsT<float>::Is(item))
                    {
                        focusedChange.Set(TraitsT<float>::FromItem(vm, item));
                        return true;
                    }

                    return false;
                };

                CLASS_NAME(TrackNode, "AtmosModulator_TrackNode");
                    PROPERTY(TrackNode, ::Atmos::Modulator::Controller::ID, modID);
                    PROPERTY(TrackNode, ::Atmos::Modulator::ModulatorBase::TrackID, trackID);
                    PROPERTY(TrackNode, ::Atmos::Modulator::TrackBase::NodeID, nodeID);
                CONSTRUCTOR(TrackNode)
                    PARAMETERS(new Parameter<::Atmos::Modulator::Controller::ID>("modID"), new Parameter<::Atmos::Modulator::ModulatorBase::TrackID>("trackID"), new Parameter<::Atmos::Modulator::TrackBase::NodeID>("nodeID"))
                BODY(TrackNode)
                {
                    SETUP_CONSTRUCTOR(TrackNode);
                    modID.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(modID));
                    trackID.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(trackID));
                    nodeID.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(nodeID));
                } BODY_END;
                METHOD(TrackNode, setInterpolationLinear, "SetInterpolationLinear", false)
                    NO_PARAMETERS
                BODY(TrackNode)
                {
                    SETUP_FUNCTION(TrackNode, setInterpolationLinear);

                    ::Atmos::Modulator::TrackNode *node = nullptr;
                    if (!RetrieveNode(*vm, node, *TrackNode::modID, *TrackNode::trackID, *TrackNode::nodeID))
                    {
                        vm->retnil();
                        return;
                    }

                    node->SetInterpolation(::Atmos::Modulator::TrackNodeInterpolation());
                } METHOD_END;
                METHOD(TrackNode, setInterpolationOscillation, "SetInterpolationOscillation", false)
                    PARAMETERS(new Parameter<::Atmos::Modulator::TrackNodeInterpolation::Acceleration>("acceleration"), new Parameter<::Atmos::Modulator::TrackNodeInterpolation::Rate>("rate"))
                BODY(TrackNode)
                {
                    SETUP_FUNCTION(TrackNode, setInterpolationOscillation);

                    ::Atmos::Modulator::TrackNode *node = nullptr;
                    if (!RetrieveNode(*vm, node, *TrackNode::modID, *TrackNode::trackID, *TrackNode::nodeID))
                    {
                        vm->retnil();
                        return;
                    }

                    auto acceleration = selfFunc.GetParameter<::Atmos::Modulator::TrackNodeInterpolation::Acceleration>("acceleration");
                    auto rate = selfFunc.GetParameter<::Atmos::Modulator::TrackNodeInterpolation::Rate>("rate");

                    node->SetInterpolation(::Atmos::Modulator::TrackNodeInterpolation(**acceleration, **rate));
                } METHOD_END;
                METHOD(TrackNode, setEndStateNormal, "SetEndStateNormal", false)
                    PARAMETERS(new Parameter<::function::Variant<std::int64_t, float>>("value"))
                BODY(TrackNode)
                {
                    SETUP_FUNCTION(TrackNode, setEndStateNormal);

                    ::Atmos::Modulator::TrackNode *node = nullptr;
                    if (!RetrieveNode(*vm, node, *TrackNode::modID, *TrackNode::trackID, *TrackNode::nodeID))
                    {
                        vm->retnil();
                        return;
                    }

                    // Setup the end state
                    typedef ::function::Variant<std::int64_t, float> Value;
                    auto &value = selfFunc.GetParameter<Value>("value")->obj;
                    auto &endState = node->PrototypeEndState();
                    if (value.Is<std::int64_t>())
                        endState.SetNormal(::Atmos::Modulator::Value(value.Get<std::int64_t>()));
                    else
                        endState.SetNormal(::Atmos::Modulator::Value(value.Get<float>()));
                    node->SetEndState(endState);
                } METHOD_END;
                METHOD(TrackNode, setEndStateRandom, "SetEndStateRandom", false)
                    PARAMETERS(new Parameter<General::Pair>("pair"))
                BODY(TrackNode)
                {
                    SETUP_FUNCTION(TrackNode, setEndStateRandom);

                    ::Atmos::Modulator::TrackNode *node = nullptr;
                    if (!RetrieveNode(*vm, node, *TrackNode::modID, *TrackNode::trackID, *TrackNode::nodeID))
                    {
                        vm->retnil();
                        return;
                    }

                    // Setup the end state
                    auto pair = setEndStateRandom.GetParameter<General::Pair>("pair");

                    typedef ::function::Variant<std::int64_t, float> VariantT;
                    ::Atmos::Modulator::Value first, second;
                    if (!ExtractModulatorValueFromItem(*vm, (*pair)->first.obj, first) || !ExtractModulatorValueFromItem(*vm, (*pair)->second.obj, second))
                    {
                        Logger::Log(AddTracebackToString(*vm, "One of the pieces of the pair is not the correct type."),
                            Logger::Type::ERROR_LOW);
                        return;
                    }

                    auto &endState = node->PrototypeEndState();
                    endState.SetRandom(::Atmos::Modulator::TrackNodeEndState::RangeT(first, second));
                    node->SetEndState(endState);
                } METHOD_END;
                METHOD(TrackNode, setTimeTaken, "SetTimeTaken", false)
                    PARAMETERS(new Parameter<Time::TimeValue>("value"))
                BODY(TrackNode)
                {
                    SETUP_FUNCTION(TrackNode, setTimeTaken);

                    ::Atmos::Modulator::TrackNode *node = nullptr;
                    if (!RetrieveNode(*vm, node, *TrackNode::modID, *TrackNode::trackID, *TrackNode::nodeID))
                    {
                        vm->retnil();
                        return;
                    }

                    // Setup the time taken
                    auto value = selfFunc.GetParameter<Time::TimeValue>("value");
                    FixedPoint64 fixedPoint(RadixPoint(9));
                    fixedPoint.SetRawValue((*value)->value->value.obj);
                    ::Atmos::TimeValue timeValue(fixedPoint, (*value)->epoch.obj);

                    node->SetTimeTaken(timeValue);
                } METHOD_END;
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
                    PROPERTY(ActionComponent, ::Atmos::Entity::ValueT, entity);
                CONSTRUCTOR(ActionComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(ActionComponent)
                {
                    SETUP_CONSTRUCTOR(ActionComponent);
                    entity.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(entity));
                } BODY_END;

                // BattleComponent
                CLASS_NAME(BattleComponent, "AtmosEntity_BattleComponent");
                    PROPERTY(BattleComponent, ::Atmos::Entity::ValueT, entity);
                CONSTRUCTOR(BattleComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(BattleComponent)
                {
                    SETUP_CONSTRUCTOR(BattleComponent);
                    entity.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(entity));
                } BODY_END;

                // AIComponent
                CLASS_NAME(AIComponent, "AtmosEntity_AIComponent");
                    PROPERTY(AIComponent, ::Atmos::Entity::ValueT, entity);
                CONSTRUCTOR(AIComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(AIComponent)
                {
                    SETUP_CONSTRUCTOR(AIComponent);
                    entity.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(entity));
                } BODY_END;

                // AvatarComponent
                CLASS_NAME(AvatarComponent, "AtmosEntity_AvatarComponent");
                    PROPERTY(AvatarComponent, ::Atmos::Entity::ValueT, entity);
                CONSTRUCTOR(AvatarComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(AvatarComponent)
                {
                    SETUP_CONSTRUCTOR(AvatarComponent);
                    entity.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(entity));
                } BODY_END;

                // CharacterComponent
                CLASS_NAME(CharacterComponent, "AtmosEntity_CharacterComponent");
                    PROPERTY(CharacterComponent, ::Atmos::Entity::ValueT, entity);
                CONSTRUCTOR(CharacterComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(CharacterComponent)
                {
                    SETUP_CONSTRUCTOR(CharacterComponent);
                    entity.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(entity));
                } BODY_END;

                // CombatComponent
                CLASS_NAME(CombatComponent, "AtmosEntity_CombatComponent");
                    PROPERTY(CombatComponent, ::Atmos::Entity::ValueT, entity);
                CONSTRUCTOR(CombatComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(CombatComponent)
                {
                    SETUP_CONSTRUCTOR(CombatComponent);
                    entity.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(entity));
                } BODY_END;

                // DialogueComponent
                CLASS_NAME(DialogueComponent, "AtmosEntity_DialogueComponent");
                    PROPERTY(DialogueComponent, ::Atmos::Entity::ValueT, entity);
                CONSTRUCTOR(DialogueComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(DialogueComponent)
                {
                    SETUP_CONSTRUCTOR(DialogueComponent);
                    entity.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(entity));
                } BODY_END;

                // GeneralComponent
                CLASS_NAME(GeneralComponent, "AtmosEntity_GeneralComponent");
                    PROPERTY(GeneralComponent, ::Atmos::Entity::ValueT, entity);
                CONSTRUCTOR(GeneralComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(GeneralComponent)
                {
                    SETUP_CONSTRUCTOR(GeneralComponent);
                    entity.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(entity));
                } BODY_END;

                // InventoryComponent
                CLASS_NAME(InventoryComponent, "AtmosEntity_InventoryComponent");
                    PROPERTY(InventoryComponent, ::Atmos::Entity::ValueT, entity);
                CONSTRUCTOR(InventoryComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(InventoryComponent)
                {
                    SETUP_CONSTRUCTOR(InventoryComponent);
                    entity.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(entity));
                } BODY_END;

                // MovementComponent
                CLASS_NAME(MovementComponent, "AtmosEntity_MovementComponent");
                    PROPERTY(MovementComponent, ::Atmos::Entity::ValueT, entity);
                CONSTRUCTOR(MovementComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(MovementComponent)
                {
                    SETUP_CONSTRUCTOR(MovementComponent);
                    entity.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(entity));
                } BODY_END;
                METHOD(MovementComponent, moveDirection, "MoveDirection", false)
                    PARAMETERS(new Parameter<::Atmos::Direction::ValueT>("direction"))
                BODY(MovementComponent)
                {
                    SETUP_FUNCTION(MovementComponent, moveDirection);

                    auto direction = selfFunc.GetParameter<::Atmos::Direction::ValueT>("direction");

                    // Get entity
                    auto &entity = MovementComponent::entity.obj;

                    // Get movement component
                    auto movementComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::MovementComponent>(entity);
                    if (!movementComponent)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A movement component was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)) });
                        return;
                    }

                    ::Atmos::Ent::PositionSystem::MoveEntity(entity, direction->obj);
                } METHOD_END;
                METHOD(MovementComponent, moveInstant, "MoveInstant", false)
                    PARAMETERS(new Parameter<Position::GridPosition>("position"))
                BODY(MovementComponent)
                {
                    SETUP_FUNCTION(MovementComponent, moveInstant);

                    auto position = selfFunc.GetParameter<Position::GridPosition>("position");

                    // Get entity
                    auto &entity = MovementComponent::entity.obj;

                    // Get movement component
                    auto movementComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::MovementComponent>(entity);
                    if (!movementComponent)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A movement component was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)) });
                        return;
                    }

                    ::Atmos::GridPosition giveToPositionSystem(*position->obj.x, *position->obj.y, *position->obj.z);
                    ::Atmos::Ent::PositionSystem::MoveEntityInstant(entity, giveToPositionSystem);
                } METHOD_END;
                METHOD(MovementComponent, canMove, "CanMove", false)
                    PARAMETERS(new Parameter<Position::GridPosition>("position", false))
                BODY(MovementComponent)
                {
                    SETUP_FUNCTION(MovementComponent, canMove);

                    auto position = selfFunc.GetParameter<Position::GridPosition>("position");

                    // Get entity
                    auto &entity = MovementComponent::entity.obj;

                    // Get movement component
                    auto movementComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::MovementComponent>(entity);
                    if (!movementComponent)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A movement component was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)) });
                        return;
                    }

                    if (!position->WasSet())
                        vm->retval(::Atmos::Ent::PositionSystem::CanMove(*movementComponent));
                    else
                    {
                        ::Atmos::GridPosition positionToPass(*position->obj.x, *position->obj.y, *position->obj.z);
                        vm->retval(::Atmos::Ent::PositionSystem::CanMove(*movementComponent, positionToPass));
                    }
                } METHOD_END;
                METHOD(MovementComponent, completeMovementSetup, "CompleteMovementSetup", false)
                    PARAMETERS(new Parameter<Modulator::Modulator>("modulator"), new Parameter<bool>("force", false, false))
                BODY(MovementComponent)
                {
                    SETUP_FUNCTION(MovementComponent, completeMovementSetup);

                    auto modulatorPass = selfFunc.GetParameter<Modulator::Modulator>("modulator");
                    auto force = selfFunc.GetParameter<bool>("force");

                    // Get entity
                    auto &entity = MovementComponent::entity.obj;

                    // Get movement component
                    auto movementComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::MovementComponent>(entity);
                    if (!movementComponent)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A movement component was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)) });
                        return;
                    }

                    // Get modulator
                    auto modulator = ::Atmos::GameEnvironment::GetModulatorController().Find((*modulatorPass)->modID.obj);
                    if (!modulator)
                    {
                        // If no modulator, then move instantly
                        ::Atmos::Ent::PositionSystem::MoveEntityInstant(entity, ::Atmos::Ent::PositionSystem::GetStagedPosition(entity)->position);
                        return;
                    }

                    // Make sure modulator is the right type
                    if (modulator->GetGeneratorName() != ::Atmos::Modulator::Description::SenseComponent.name)
                    {
                        Logger::Log(AddTracebackToString(*vm, "The modulator is the incorrect type."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Modulator ID", (*modulatorPass)->modID.obj),
                            NameValuePair("Required Type", ::Atmos::Modulator::Description::SpriteOffset.name),
                            NameValuePair("Actual Type", modulator->GetGeneratorName()) });
                        return;
                    }

                    // Get sense component
                    auto senseComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::SenseComponent>(entity);
                    if (!senseComponent)
                    {
                        // If no sense component, move instantly
                        ::Atmos::Ent::PositionSystem::MoveEntityInstant(entity, ::Atmos::Ent::PositionSystem::GetStagedPosition(entity)->position);
                    }

                    // If forcing wasn't set...
                    if (!force->WasSet() || !force->obj)
                    {
                        // And there's a modulator working the object
                        if (GameEnvironment::GetModulatorController().IsModulatorWorkingObject(senseComponent))
                            // Return silently
                            return;
                    }

                    // Finally, attach the modulator
                    auto castedModulator = static_cast<decltype(::Atmos::Modulator::Description::SenseComponent)::ModulatorT*>(modulator.Get());
                    castedModulator->Start(*senseComponent);
                } METHOD_END;

                // SenseComponent
                CLASS_NAME(SenseComponent, "AtmosEntity_SenseComponent");
                    PROPERTY(SenseComponent, ::Atmos::Entity::ValueT, entity);
                CONSTRUCTOR(SenseComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(SenseComponent)
                {
                    SETUP_CONSTRUCTOR(SenseComponent);
                    entity.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(entity));
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
                    auto modulator = ::Atmos::GameEnvironment::GetModulatorController().Find((*attach)->modID.obj);
                    if (!modulator)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A modulator was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Modulator ID", (*attach)->modID.obj) });
                        return;
                    }
                    
                    // Make sure modulator is the right type
                    if (modulator->GetGeneratorName() != ::Atmos::Modulator::Description::SpriteOffset.name)
                    {
                        Logger::Log(AddTracebackToString(*vm, "The modulator is the incorrect type."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Modulator ID", (*attach)->modID.obj),
                                                     NameValuePair("Required Type", ::Atmos::Modulator::Description::SpriteOffset.name),
                                                     NameValuePair("Actual Type", modulator->GetGeneratorName()) });
                        return;
                    }

                    // Get entity
                    auto &entity = SenseComponent::entity.obj;
                    // Get sense component
                    auto senseComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::SenseComponent>(entity);
                    if (!senseComponent)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A sense component was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)) });
                        return;
                    }

                    // Get the sprite out of the sense component
                    auto foundSprite = senseComponent->FindSprite(**spriteID);
                    if (!foundSprite)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A sense component did not have a required sprite."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)),
                                                     NameValuePair("Sprite ID", **spriteID) });
                    }

                    // If forcing wasn't set...
                    if (!force->WasSet() || !force->obj)
                    {
                        // And there's a modulator working the object
                        if(GameEnvironment::GetModulatorController().IsModulatorWorkingObject(foundSprite))
                            // Return silently
                            return;
                    }

                    // Finally, attach the modulator
                    auto castedModulator = static_cast<decltype(::Atmos::Modulator::Description::SpriteOffset)::ModulatorT*>(modulator.Get());
                    castedModulator->Start(*foundSprite);
                } METHOD_END;
                METHOD(SenseComponent, getPosition, "GetPosition", false)
                    NO_PARAMETERS
                BODY(SenseComponent)
                {
                    SETUP_FUNCTION(SenseComponent, getPosition);

                    // Get entity
                    auto &entity = SenseComponent::entity.obj;
                    // Get sense component
                    auto senseComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::SenseComponent>(entity);
                    if (!senseComponent)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A sense component was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)) });
                        return;
                    }

                    Position::Position3D::x.obj = senseComponent->position.GetX();
                    Position::Position3D::y.obj = senseComponent->position.GetY();
                    Position::Position3D::z.obj = senseComponent->position.GetZ();
                    vm->retval(Position::Position3D::Instance().CreateItem(*vm));
                } METHOD_END;
                METHOD(SenseComponent, getStagedPosition, "GetStagedPosition", false)
                    NO_PARAMETERS
                BODY(SenseComponent)
                {
                    SETUP_FUNCTION(SenseComponent, getStagedPosition);

                    // Get entity
                    auto &entity = SenseComponent::entity.obj;
                    // Get sense component
                    auto senseComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::SenseComponent>(entity);
                    if (!senseComponent)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A sense component was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)) });
                        return;
                    }

                    Optional<::Atmos::Ent::PositionSystem::StagedPosition> stagedPosition = ::Atmos::Ent::PositionSystem::GetStagedPosition(entity);

                    Position::Position3D::x.obj = ::Atmos::GridPosition::DimensionToPosition(stagedPosition->position.GetX());
                    Position::Position3D::y.obj = ::Atmos::GridPosition::DimensionToPosition(stagedPosition->position.GetY());
                    Position::Position3D::z.obj = ::Atmos::GridPosition::DimensionToPosition(stagedPosition->position.GetZ());
                    vm->retval(Position::Position3D::Instance().CreateItem(*vm));
                } METHOD_END;

                // Entity
                CLASS_NAME(Entity, "AtmosEntity");
                    PROPERTY(Entity, ::Atmos::Entity::ValueT, value);
                CONSTRUCTOR(Entity)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("value"))
                BODY(Entity)
                {
                    SETUP_CONSTRUCTOR(Entity);
                    value.SetPropertyTo(*vm, vm->self(), **constructor.GetParameter(value));
                } BODY_END;
                // Has component
                METHOD(Entity, hasComponent, "HasComponent", false)
                    PARAMETERS(new Parameter<::Atmos::Ent::ComponentTypeName>("typeName"))
                BODY(Entity)
                {
                    SETUP_FUNCTION(Entity, hasComponent);

                    auto typeName = selfFunc.GetParameter<::Atmos::Ent::ComponentTypeName>("typeName");
                    // Get value
                    auto &entity = Entity::value.obj;
                    // Return if we could find the component
                    vm->retval(GetCurrentEntities()->FindComponent(entity, **typeName));
                } METHOD_END;

                template<class ComponentClassT>
                bool AttemptCreateComponent(::Atmos::Entity entity, const ::Atmos::Ent::ComponentTypeName &check, const ::Atmos::Ent::ComponentTypeName &checkAgainst, Falcon::VMachine &vm)
                {
                    if (check != checkAgainst)
                        return false;

                    ComponentClassT::entity.obj = entity;
                    vm.retval(ComponentClassT::Instance().CreateItem(vm));
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
                    auto &entity = Entity::value.obj;
                    // Find component
                    auto found = GetCurrentEntities()->FindComponent(entity, **typeName);
                    // If we didn't find the component, return nil
                    if (!found)
                    {
                        vm->retnil();
                        return;
                    }

#define ATTEMPT_CREATE_COMPONENT(ComponentClassT)                                                                       \
if (AttemptCreateComponent<ComponentClassT>(entity, **typeName, ::Atmos::Ent::ComponentClassT::TypeName(), *vm))   \
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

                } METHOD_END;

                METHOD(Entity, getPosition, "GetPosition", false)
                    NO_PARAMETERS
                BODY(Entity)
                {
                    SETUP_FUNCTION(Entity, getPosition);

                    // Get value
                    auto &entity = Entity::value.obj;

                    auto foundPosition = GetCurrentEntities()->GetPosition(entity);
                    if (!foundPosition)
                    {
                        vm->retnil();
                        return;
                    }

                    Position::GridPosition::x.obj = foundPosition->GetX();
                    Position::GridPosition::y.obj = foundPosition->GetY();
                    Position::GridPosition::z.obj = foundPosition->GetZ();
                    vm->retval(Position::GridPosition::Instance().CreateItem(*vm));
                } METHOD_END;
            }
        }
    }
}