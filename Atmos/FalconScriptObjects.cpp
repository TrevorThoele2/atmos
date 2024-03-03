
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

Atmos::Fal::Classes::Prototype::Method* AddMethodReturner(Atmos::Fal::Classes::Prototype &prototype, Atmos::Fal::Classes::Prototype::Method *add)
{
    prototype.AddMethod(add);
    return add;
}

#define CLASS_NAME(classT, name)    \
    const Name classT::className(name);

#define STATIC_PROPERTY(classT, T, name, ...)   \
    PropertyPrototype<T> classT::name(classT::Scaffolding(), #name, __VA_ARGS__);
#define STATIC_PROPERTY_SPLIT(classT, T, varName, falconName, ...)   \
    PropertyPrototype<T> classT::varName(classT::Scaffolding(), falconName, __VA_ARGS__);
#define PROPERTY(propName)      \
    propName(*this, #propName)
#define PROPERTY_SPLIT(propName, stringName)  \
    propName(*this, stringName)
#define COPY_PROPERTY(name) \
    name(*this, arg.name)
#define MOVE_PROPERTY(name)         \
    name(*this, std::move(arg.name))

#define METHOD(classT, methodName, falconMethodName, isStatic)      \
    classT::Method& classT::methodName(*AddMethodReturner(classT::Scaffolding(), new classT::Method(classT::Scaffolding(), falconMethodName, isStatic,

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

#define CONSTRUCTOR(classT)                                         \
    classT::Constructor classT::constructor(classT::Scaffolding(),

#define SETUP_METHOD(className, func)                       \
auto &selfMethod = func;                                    \
if (!selfMethod.SetupParameters(*vm))                       \
    return;                                                 \
auto self = static_cast<className*>(selfMethod.prototype);  \
self->FromItem(*vm, vm->self());

#define SETUP_STATIC_METHOD(className, func)                \
auto &selfMethod = func;                                    \
if (!selfMethod.SetupParameters(*vm))                       \
    return;

#define SETUP_CONSTRUCTOR(className)                        \
auto &selfMethod = className::constructor;                  \
if (!selfMethod.SetupParameters(*vm))                       \
    return;                                                 \
auto self = static_cast<className*>(selfMethod.prototype);

#define DEFINE_CONSTRUCTOR_1_PROPERTIES(className, one)         \
className::className() : PROPERTY(one)                          \
{}                                                              \
                                                                \
className::className(const className &arg) : COPY_PROPERTY(one) \
{}                                                              \
                                                                \
className::className(className &&arg) : MOVE_PROPERTY(one)      \
{}                                                              \
                                                                \
className& className::operator=(className &&arg)                \
{                                                               \
    Definition::operator=(std::move(arg));                      \
    one = std::move(arg.one);                                   \
    return *this;                                               \
}

#define DEFINE_CONSTRUCTOR_2_PROPERTIES(className, one, two)                        \
className::className() : PROPERTY(one), PROPERTY(two)                               \
{}                                                                                  \
                                                                                    \
className::className(const className &arg) : COPY_PROPERTY(one), COPY_PROPERTY(two) \
{}                                                                                  \
                                                                                    \
className::className(className &&arg) : MOVE_PROPERTY(one), MOVE_PROPERTY(two)      \
{}                                                                                  \
                                                                                    \
className& className::operator=(className &&arg)                                    \
{                                                                                   \
    Definition::operator=(std::move(arg));                                          \
    one = std::move(arg.one);                                                       \
    two = std::move(arg.two);                                                       \
    return *this;                                                                   \
}

#define DEFINE_CONSTRUCTOR_3_PROPERTIES(className, one, two, three)                                         \
className::className() : PROPERTY(one), PROPERTY(two), PROPERTY(three)                                      \
{}                                                                                                          \
                                                                                                            \
className::className(const className &arg) : COPY_PROPERTY(one), COPY_PROPERTY(two), COPY_PROPERTY(three)   \
{}                                                                                                          \
                                                                                                            \
className::className(className &&arg) : MOVE_PROPERTY(one), MOVE_PROPERTY(two), MOVE_PROPERTY(three)        \
{}                                                                                                          \
                                                                                                            \
className& className::operator=(className &&arg)                                                            \
{                                                                                                           \
    Definition::operator=(std::move(arg));                                                                  \
    one = std::move(arg.one);                                                                               \
    two = std::move(arg.two);                                                                               \
    three = std::move(arg.three);                                                                           \
    return *this;                                                                                           \
}

#define DEFINE_CONSTRUCTOR_4_PROPERTIES(className, one, two, three, four)                                                       \
className::className() : PROPERTY(one), PROPERTY(two), PROPERTY(three), PROPERTY(four)                                          \
{}                                                                                                                              \
                                                                                                                                \
className::className(const className &arg) : COPY_PROPERTY(one), COPY_PROPERTY(two), COPY_PROPERTY(three), COPY_PROPERTY(four)  \
{}                                                                                                                              \
                                                                                                                                \
className::className(className &&arg) : MOVE_PROPERTY(one), MOVE_PROPERTY(two), MOVE_PROPERTY(three), MOVE_PROPERTY(four)       \
{}                                                                                                                              \
                                                                                                                                \
className& className::operator=(className &&arg)                                                                                \
{                                                                                                                               \
    Definition::operator=(std::move(arg));                                                                                      \
    one = std::move(arg.one);                                                                                                   \
    two = std::move(arg.two);                                                                                                   \
    three = std::move(arg.three);                                                                                               \
    four = std::move(arg.four);                                                                                                 \
    return *this;                                                                                                               \
}

namespace Atmos
{
    class SenseComponentSpriteLocator : public ScriptLocator
    {
    public:
        typedef ::Atmos::Ent::SenseComponent::SpriteList::ID SpriteID;
        typedef Ent::SenseComponent::SpriteHandle TypeLocating;
    private:
        void* FindImpl() override final
        {
            auto senseComponent = GetCurrentEntities()->FindComponent<Ent::SenseComponent>(entity);
            if (!senseComponent)
                return nullptr;

            return senseComponent->FindSprite(spriteID);
        }

        std::type_index GetTypeImpl() override final
        {
            return typeid(TypeLocating);
        }
    public:
        Entity entity;
        SpriteID spriteID;

        SenseComponentSpriteLocator(Entity entity, SpriteID spriteID, Script::Instance &owner) : ScriptLocator(owner), entity(entity), spriteID(spriteID)
        {}
    };

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
                    SETUP_STATIC_METHOD(Caller, isEmpty);

                    auto script = ScriptController::Current();
                    if (!script)
                        vm->retval(false);

                    vm->retval(script->GetCaller().IsEmpty());
                } METHOD_END;
                METHOD(Caller, isEntity, "IsEntity", true)
                    NO_PARAMETERS
                BODY(Caller)
                {
                    SETUP_STATIC_METHOD(Caller, isEntity);

                    auto script = ScriptController::Current();
                    if (!script)
                        vm->retval(false);

                    vm->retval(script->GetCaller().IsEntity());
                } METHOD_END;
                METHOD(Caller, getEntity, "GetEntity", true)
                    NO_PARAMETERS
                BODY(Caller)
                {
                    SETUP_STATIC_METHOD(Caller, getEntity);

                    auto script = ScriptController::Current();
                    if (!script || !script->GetCaller().IsEntity())
                        vm->retnil();

                    Ent::Entity::Scaffolding().value.Set(script->GetCaller().GetEntity());
                    vm->retval(Ent::Entity::Scaffolding().CreateItem(*vm));
                } METHOD_END;

                CLASS_NAME(LogType, "Atmos_LogType");
                STATIC_PROPERTY_SPLIT(LogType, Logger::Type, errorSevere, "ErrorSevere", Logger::Type::ERROR_SEVERE, true);
                    STATIC_PROPERTY_SPLIT(LogType, Logger::Type, errorModerate, "ErrorModerate", Logger::Type::ERROR_MODERATE, true);
                    STATIC_PROPERTY_SPLIT(LogType, Logger::Type, errorLow, "ErrorLow", Logger::Type::ERROR_LOW, true);
                    STATIC_PROPERTY_SPLIT(LogType, Logger::Type, warning, "Warning", Logger::Type::WARNING, true);
                    STATIC_PROPERTY_SPLIT(LogType, Logger::Type, information, "Information", Logger::Type::INFORMATION, true);

                CLASS_NAME(FixedPoint, "Atmos_FixedPoint");
                CONSTRUCTOR(FixedPoint)
                    PARAMETERS(new Parameter<std::int64_t>("value"))
                BODY(FixedPoint)
                {
                    SETUP_CONSTRUCTOR(FixedPoint);
                    self->value.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<std::int64_t>("value")->obj);
                } BODY_END;
                METHOD(FixedPoint, add, "__add", false)
                    PARAMETERS(new Parameter<Falcon::Item>("other"))
                BODY(FixedPoint)
                {
                    SETUP_METHOD(FixedPoint, add);

                    auto &other = **selfMethod.GetParameter<Falcon::Item>("other");
                    if (!other.isOrdinal() && !FalconVariableTraitsClassPrototype<FixedPoint>::Is(other))
                    {
                        Falcon::String otherString;
                        other.toString(otherString);
                        Logger::Log(AddTracebackToString(*vm, "The fixed point can only have arithmetic against number-like objects."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Other", Convert(otherString)) });
                    }

                    FixedPoint64 newValue(self->value.Retrieve(), ::Atmos::FixedPoint64::Radix(0));
                    if (other.isOrdinal())
                        newValue += FixedPoint64(other.asInteger(), ::Atmos::FixedPoint64::Radix(0));
                    else
                        newValue += FixedPoint64(FalconVariableTraitsClassPrototype<FixedPoint>::FromItem(*vm, other).value.Retrieve(), ::Atmos::FixedPoint64::Radix(0));
                    self->value.SetPropertyTo(*vm, vm->self(), newValue.GetRawValue());
                } METHOD_END;
                METHOD(FixedPoint, subtract, "__subtract", false)
                    PARAMETERS(new Parameter<Falcon::Item>("other"))
                BODY(FixedPoint)
                {
                    SETUP_METHOD(FixedPoint, subtract);

                    auto &other = **selfMethod.GetParameter<Falcon::Item>("other");
                    if (!other.isOrdinal() && !FalconVariableTraitsClassPrototype<FixedPoint>::Is(other))
                    {
                        Falcon::String otherString;
                        other.toString(otherString);
                        Logger::Log(AddTracebackToString(*vm, "The fixed point can only have arithmetic against number-like objects."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Other", Convert(otherString)) });
                    }

                    FixedPoint64 newValue(self->value.Retrieve(), ::Atmos::FixedPoint64::Radix(0));
                    if (other.isOrdinal())
                        newValue -= FixedPoint64(other.asInteger(), ::Atmos::FixedPoint64::Radix(0));
                    else
                        newValue -= FixedPoint64(FalconVariableTraitsClassPrototype<FixedPoint>::FromItem(*vm, other).value.Retrieve(), ::Atmos::FixedPoint64::Radix(0));
                    self->value.SetPropertyTo(*vm, vm->self(), newValue.GetRawValue());
                } METHOD_END;
                METHOD(FixedPoint, multiply, "__mul", false)
                    PARAMETERS(new Parameter<Falcon::Item>("other"))
                BODY(FixedPoint)
                {
                    SETUP_METHOD(FixedPoint, multiply);

                    auto &other = **selfMethod.GetParameter<Falcon::Item>("other");
                    if (!other.isOrdinal() && !FalconVariableTraitsClassPrototype<FixedPoint>::Is(other))
                    {
                        Falcon::String otherString;
                        other.toString(otherString);
                        Logger::Log(AddTracebackToString(*vm, "The fixed point can only have arithmetic against number-like objects."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Other", Convert(otherString)) });
                    }

                    FixedPoint64 newValue(self->value.Retrieve(), ::Atmos::FixedPoint64::Radix(0));
                    if (other.isOrdinal())
                        newValue *= FixedPoint64(other.asInteger(), ::Atmos::FixedPoint64::Radix(0));
                    else
                        newValue *= FixedPoint64(FalconVariableTraitsClassPrototype<FixedPoint>::FromItem(*vm, other).value.Retrieve(), ::Atmos::FixedPoint64::Radix(0));
                    self->value.SetPropertyTo(*vm, vm->self(), newValue.GetRawValue());
                } METHOD_END;
                METHOD(FixedPoint, divide, "__div", false)
                    PARAMETERS(new Parameter<Falcon::Item>("other"))
                BODY(FixedPoint)
                {
                    SETUP_METHOD(FixedPoint, divide);

                    auto &other = **selfMethod.GetParameter<Falcon::Item>("other");
                    if (!other.isOrdinal() && !FalconVariableTraitsClassPrototype<FixedPoint>::Is(other))
                    {
                        Falcon::String otherString;
                        other.toString(otherString);
                        Logger::Log(AddTracebackToString(*vm, "The fixed point can only have arithmetic against number-like objects."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Other", Convert(otherString)) });
                    }

                    FixedPoint64 newValue(self->value.Retrieve(), ::Atmos::FixedPoint64::Radix(0));
                    if (other.isOrdinal())
                        newValue /= FixedPoint64(other.asInteger(), ::Atmos::FixedPoint64::Radix(0));
                    else
                        newValue /= FixedPoint64(FalconVariableTraitsClassPrototype<FixedPoint>::FromItem(*vm, other).value.Retrieve(), ::Atmos::FixedPoint64::Radix(0));
                    self->value.SetPropertyTo(*vm, vm->self(), newValue.GetRawValue());
                } METHOD_END;
                METHOD(FixedPoint, fromDecimal, "FromDecimal", true)
                    PARAMETERS(new Parameter<double>("from"))
                BODY(FixedPoint)
                {
                    SETUP_STATIC_METHOD(FixedPoint, fromDecimal);

                    auto from = selfMethod.GetParameter<double>("from");

                    ::Atmos::FixedPoint64 fixedPoint(from->obj);
                    General::FixedPoint::Scaffolding().value.Set(fixedPoint.GetRawValue());
                    vm->retval(General::FixedPoint::Scaffolding().CreateItem(*vm));
                } METHOD_END;
                METHOD(FixedPoint, toDecimal, "ToDecimal", false)
                    NO_PARAMETERS
                BODY(FixedPoint)
                {
                    SETUP_METHOD(FixedPoint, toDecimal);

                    ::Atmos::FixedPoint64 fixedPoint(self->value.Retrieve(), ::Atmos::FixedPoint64::Radix(0));
                    fixedPoint.SetRadixPoint(::Atmos::FixedPoint64::Radix(9), false);
                    vm->retval(static_cast<double>(fixedPoint));
                } METHOD_END;

                FixedPoint::FixedPoint() : value(*this, "value")
                {}

                FixedPoint::FixedPoint(const FixedPoint &arg) : value(*this, arg.value)
                {}

                FixedPoint::FixedPoint(FixedPoint &&arg) : value(*this, std::move(arg.value))
                {}

                FixedPoint& FixedPoint::operator=(FixedPoint &&arg)
                {
                    Definition::operator=(std::move(arg));
                    value = std::move(arg.value);
                    return *this;
                }

                CLASS_NAME(Pair, "Atmos_Pair");
                CONSTRUCTOR(Pair)
                    PARAMETERS(new Parameter<::Falcon::Item>("first"), new Parameter<::Falcon::Item>("second"))
                BODY(Pair)
                {
                    SETUP_CONSTRUCTOR(Pair);
                    self->first.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Falcon::Item>("first")->obj);
                    self->second.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Falcon::Item>("second")->obj);
                } BODY_END;
                DEFINE_CONSTRUCTOR_2_PROPERTIES(Pair, first, second);
            }

            namespace Position
            {
                CLASS_NAME(GridPosition, "AtmosPosition_GridPosition");
                // GridPosition constructor
                CONSTRUCTOR(GridPosition)
                    PARAMETERS(new Parameter<::Atmos::GridPosition::ValueT>("x"), new Parameter<::Atmos::GridPosition::ValueT>("y"), new Parameter<::Atmos::GridPosition::ValueT>("z"))
                    BODY(GridPosition)
                {
                    SETUP_CONSTRUCTOR(GridPosition);
                    self->x.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::GridPosition::ValueT>("x")->obj);
                    self->y.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::GridPosition::ValueT>("y")->obj);
                    self->z.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::GridPosition::ValueT>("z")->obj);
                } BODY_END;
                // Distance method
                METHOD(GridPosition, distance, "Distance", false)
                    PARAMETERS(new Parameter<GridPosition>("other"))
                    BODY(GridPosition)
                {
                    SETUP_METHOD(GridPosition, distance);

                    auto &other = selfMethod.GetParameter<GridPosition>("other")->obj;
                    vm->retval(std::abs(other.x.Retrieve() - self->x.Retrieve()) +
                               std::abs(other.y.Retrieve() - self->x.Retrieve()) +
                               std::abs(other.z.Retrieve() - self->x.Retrieve()));
                } METHOD_END;
                METHOD(GridPosition, directionTo, "DirectionTo", false)
                    PARAMETERS(new Parameter<GridPosition>("to"))
                BODY(GridPosition)
                {
                    SETUP_METHOD(GridPosition, directionTo);

                    ::Atmos::GridPosition from(self->x.Retrieve(), self->y.Retrieve(), self->z.Retrieve());
                    ::Atmos::GridPosition other((*selfMethod.GetParameter<GridPosition>("to"))->x.Retrieve(),
                                                (*selfMethod.GetParameter<GridPosition>("to"))->y.Retrieve(),
                                                (*selfMethod.GetParameter<GridPosition>("to"))->z.Retrieve());
                    vm->retval(static_cast<::Falcon::int64>(from.DetermineDirection(other).Get()));
                } METHOD_END;
                METHOD(GridPosition, convertToPosition3D, "ConvertToPosition3D", false)
                    NO_PARAMETERS
                BODY(GridPosition)
                {
                    SETUP_METHOD(GridPosition, convertToPosition3D);

                    Position3D::Scaffolding().x.Set(::Atmos::GridPosition::DimensionToPosition(self->x.Retrieve()));
                    Position3D::Scaffolding().y.Set(::Atmos::GridPosition::DimensionToPosition(self->y.Retrieve()));
                    Position3D::Scaffolding().z.Set(::Atmos::GridPosition::DimensionToPosition(self->z.Retrieve()));
                    vm->retval(Position3D::Scaffolding().CreateItem(*vm));
                } METHOD_END;
                DEFINE_CONSTRUCTOR_3_PROPERTIES(GridPosition, x, y, z);

                CLASS_NAME(Position3D, "AtmosPosition_Position3D");
                // GridPosition constructor
                CONSTRUCTOR(Position3D)
                    PARAMETERS(new Parameter<::Atmos::Position3D::ValueT>("x"), new Parameter<::Atmos::Position3D::ValueT>("y"), new Parameter<::Atmos::Position3D::ValueT>("z"))
                BODY(Position3D)
                {
                    SETUP_CONSTRUCTOR(Position3D);
                    self->x.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Position3D::ValueT>("x")->obj);
                    self->y.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Position3D::ValueT>("y")->obj);
                    self->z.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Position3D::ValueT>("z")->obj);
                } BODY_END;
                METHOD(Position3D, distance, "Distance", false)
                    PARAMETERS(new Parameter<Position3D>("other"))
                    BODY(Position3D)
                {
                    SETUP_METHOD(Position3D, distance);

                    auto &other = selfMethod.GetParameter<Position3D>("other")->obj;
                    vm->retval(std::abs(other.x.Retrieve() - self->x.Retrieve()) +
                               std::abs(other.y.Retrieve() - self->x.Retrieve()) +
                               std::abs(other.z.Retrieve() - self->x.Retrieve()));
                } METHOD_END;
                METHOD(Position3D, convertToGridPosition, "ConvertToGridPosition", false)
                    NO_PARAMETERS
                BODY(Position3D)
                {
                    SETUP_METHOD(Position3D, convertToGridPosition);

                    GridPosition::Scaffolding().x.Set(::Atmos::GridPosition::DimensionFromPosition(self->x.Retrieve()));
                    GridPosition::Scaffolding().y.Set(::Atmos::GridPosition::DimensionFromPosition(self->y.Retrieve()));
                    GridPosition::Scaffolding().z.Set(::Atmos::GridPosition::DimensionFromPosition(self->z.Retrieve()));
                    vm->retval(GridPosition::Scaffolding().CreateItem(*vm));
                } METHOD_END;
                DEFINE_CONSTRUCTOR_3_PROPERTIES(Position3D, x, y, z);

                CLASS_NAME(Direction, "AtmosPosition_Direction");
                    STATIC_PROPERTY_SPLIT(Direction, ::Atmos::Direction::ValueT, left, "Left", ::Atmos::Direction::ValueT::LEFT, Readonly(true));
                    STATIC_PROPERTY_SPLIT(Direction, ::Atmos::Direction::ValueT, up, "Up", ::Atmos::Direction::ValueT::UP, Readonly(true));
                    STATIC_PROPERTY_SPLIT(Direction, ::Atmos::Direction::ValueT, right, "Right", ::Atmos::Direction::ValueT::RIGHT, Readonly(true));
                    STATIC_PROPERTY_SPLIT(Direction, ::Atmos::Direction::ValueT, down, "Down", ::Atmos::Direction::ValueT::DOWN, Readonly(true));
            }

            namespace Input
            {
#define ACTION_ID_CREATOR(varName, falconName, enumName)                                                                    \
STATIC_PROPERTY_SPLIT(ActionID, ::Atmos::Input::ActionID, varName, falconName, ::Atmos::Input::ActionID::enumName, true);

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
                    STATIC_PROPERTY_SPLIT(Epoch, ::Atmos::TimeValueEpoch, minutes, "Minutes", ::Atmos::TimeValueEpoch::MINUTES, true);
                    STATIC_PROPERTY_SPLIT(Epoch, ::Atmos::TimeValueEpoch, seconds, "Seconds", ::Atmos::TimeValueEpoch::SECONDS, true);
                    STATIC_PROPERTY_SPLIT(Epoch, ::Atmos::TimeValueEpoch, milliseconds, "Milliseconds", ::Atmos::TimeValueEpoch::MILLISECONDS, true);
                    STATIC_PROPERTY_SPLIT(Epoch, ::Atmos::TimeValueEpoch, microseconds, "Microseconds", ::Atmos::TimeValueEpoch::MICROSECONDS, true);
                    STATIC_PROPERTY_SPLIT(Epoch, ::Atmos::TimeValueEpoch, nanoseconds, "Nanoseconds", ::Atmos::TimeValueEpoch::NANOSECONDS, true);

                CLASS_NAME(TimeValue, "AtmosTime_Value");
                CONSTRUCTOR(TimeValue)
                    PARAMETERS(new Parameter<General::FixedPoint>("value"), new Parameter<::Atmos::TimeValueEpoch>("epoch"))
                BODY(TimeValue)
                {
                    SETUP_CONSTRUCTOR(TimeValue);
                    self->value.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<General::FixedPoint>("value")->obj);
                    self->epoch.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::TimeValueEpoch>("epoch")->obj);
                } BODY_END;
                DEFINE_CONSTRUCTOR_2_PROPERTIES(TimeValue, value, epoch);

                // FrameTimer constructor
                CLASS_NAME(FrameTimer, "AtmosTime_FrameTimer")
                CONSTRUCTOR(FrameTimer)
                    PARAMETERS(new Parameter<TimeValue>("goal", false), new Parameter<::Atmos::TimeValueEpoch>("epoch", false))
                BODY(FrameTimer)
                {
                    SETUP_CONSTRUCTOR(FrameTimer);

                    // Set is running
                    self->isRunning.SetPropertyTo(*vm, vm->self(), false);

                    auto epochPass = selfMethod.GetParameter<::Atmos::TimeValueEpoch>("epoch");

                    // Epoch to use for both the goal and the time value
                    ::Atmos::TimeValueEpoch useEpoch = epochPass->WasSet() ? epochPass->obj : ::Atmos::TimeValueEpoch::SECONDS;
                    // Set epoch
                    self->epoch.SetPropertyTo(*vm, vm->self(), useEpoch);

                    // Set start
                    TimeValue::Scaffolding().epoch.Set(useEpoch);
                    General::FixedPoint::Scaffolding().value.Set(0);
                    TimeValue::Scaffolding().value.Set(General::FixedPoint::Scaffolding());
                    self->startValue.SetPropertyTo(*vm, vm->self(), TimeValue::Scaffolding());

                    // Set goal
                    // Use the previous epoch
                    TimeValue::Scaffolding().epoch.Set(useEpoch);
                    auto goalPass = selfMethod.GetParameter<TimeValue>("goal");

                    // Set the fixed point value of the time value
                    General::FixedPoint::Scaffolding().value.Set((goalPass->WasSet()) ? goalPass->obj.value.Retrieve().value.Retrieve() : 0);
                    TimeValue::Scaffolding().value.Set(General::FixedPoint::Scaffolding());
                    // Set goal to a time value
                    self->goal.SetPropertyTo(*vm, vm->self(), TimeValue::Scaffolding());
                } BODY_END;
                METHOD(FrameTimer, start, "Start", false)
                    NO_PARAMETERS
                BODY(FrameTimer)
                {
                    SETUP_METHOD(FrameTimer, start);

                    // Set the is running
                    self->isRunning.SetPropertyTo(*vm, vm->self(), true);

                    // Set the starting point
                    auto epochSelf = self->epoch.Retrieve();
                    auto timeValue = ::Atmos::Act::Time::GetFrameTime();
                    timeValue.Convert(epochSelf);

                    // Set the scaffolding of the TimeValue's epoch and value
                    TimeValue::Scaffolding().epoch.Set(epochSelf);
                    General::FixedPoint::Scaffolding().value.Set(timeValue.Get().GetRawValue());
                    TimeValue::Scaffolding().value.Set(General::FixedPoint::Scaffolding());
                    // Set the start value to an instance of the TimeValue
                    self->startValue.SetPropertyTo(*vm, vm->self(), TimeValue::Scaffolding());
                } METHOD_END;
                METHOD(FrameTimer, stop, "Stop", false)
                    NO_PARAMETERS
                BODY(FrameTimer)
                {
                    SETUP_METHOD(FrameTimer, stop);

                    // Set the is running
                    self->isRunning.SetPropertyTo(*vm, vm->self(), false);

                    // Set the starting point
                    // Set the scaffolding of the TimeValue's epoch and value
                    TimeValue::Scaffolding().epoch.Set(self->epoch.Retrieve());
                    General::FixedPoint::Scaffolding().value.Set(0);
                    TimeValue::Scaffolding().value.Set(General::FixedPoint::Scaffolding());
                    // Set the start value to an instance of the TimeValue
                    self->startValue.SetPropertyTo(*vm, vm->self(), TimeValue::Scaffolding());
                } METHOD_END;
                METHOD(FrameTimer, isActive, "IsActive", false)
                    NO_PARAMETERS
                BODY(FrameTimer)
                {
                    SETUP_METHOD(FrameTimer, isActive);

                    vm->retval(self->isRunning.Retrieve());
                } METHOD_END;
                METHOD(FrameTimer, getElapsed, "GetElapsed", false)
                    NO_PARAMETERS
                BODY(FrameTimer)
                {
                    SETUP_METHOD(FrameTimer, getElapsed);

                    // Get the raw ending time value
                    auto epochUse = self->epoch.Retrieve();
                    auto timeValue = ::Atmos::Act::Time::GetFrameTime();
                    timeValue.Convert(epochUse);
                    auto endingRawValue = timeValue.Get().GetRawValue();

                    // Get the raw starting time value
                    auto startingRawValue = self->startValue.Retrieve().value.Retrieve().value.Retrieve();

                    // Push the elapsed
                    TimeValue::Scaffolding().epoch.Set(epochUse);
                    General::FixedPoint::Scaffolding().value.Set(endingRawValue - startingRawValue);
                    TimeValue::Scaffolding().value.Set(General::FixedPoint::Scaffolding());
                    vm->retval(TimeValue::Scaffolding().CreateItem(*vm));
                } METHOD_END;
                METHOD(FrameTimer, hasReachedGoal, "HasReachedGoal", false)
                    NO_PARAMETERS
                BODY(FrameTimer)
                {
                    SETUP_METHOD(FrameTimer, hasReachedGoal);

                    // Get the raw ending time value
                    auto epochUse = self->epoch.Retrieve();
                    auto timeValue = ::Atmos::Act::Time::GetFrameTime();
                    timeValue.Convert(epochUse);
                    auto endingRawValue = timeValue.Get().GetRawValue();

                    // Get the raw starting time value
                    auto startingRawValue = self->startValue.Retrieve().value.Retrieve().value.Retrieve();

                    // Get the raw goal time value
                    auto &goalRawValue = self->goal.Retrieve().value.Retrieve().value.Retrieve();

                    // Push if we've reached the goal yet
                    auto elapsed = endingRawValue - startingRawValue;
                    vm->retval(elapsed >= goalRawValue);
                } METHOD_END;

                FrameTimer::FrameTimer() : PROPERTY_SPLIT(isRunning, "_isRunning"), PROPERTY_SPLIT(startValue, "_start"), PROPERTY_SPLIT(goal, "goal"), PROPERTY_SPLIT(epoch, "_epoch")
                {}

                FrameTimer::FrameTimer(const FrameTimer &arg) : COPY_PROPERTY(isRunning), COPY_PROPERTY(startValue), COPY_PROPERTY(goal), COPY_PROPERTY(epoch)
                {}

                FrameTimer::FrameTimer(FrameTimer &&arg) : MOVE_PROPERTY(isRunning), MOVE_PROPERTY(startValue), MOVE_PROPERTY(goal), MOVE_PROPERTY(epoch)
                {}

                FrameTimer& FrameTimer::operator=(FrameTimer &&arg)
                {
                    Definition::operator=(std::move(arg));
                    isRunning = std::move(arg.isRunning);
                    start = std::move(arg.start);
                    goal = std::move(arg.goal);
                    epoch = std::move(arg.epoch);
                    return *this;
                }
            }

            namespace Modulator
            {
                // ModulatorType
                CLASS_NAME(Type, "AtmosModulator_Type");
                    STATIC_PROPERTY_SPLIT(Type, Name, sprite, ::Atmos::Modulator::Description::Sprite.name, ::Atmos::Modulator::Description::Sprite.name, true);
                    STATIC_PROPERTY_SPLIT(Type, Name, spriteOffset, ::Atmos::Modulator::Description::SpriteOffset.name, ::Atmos::Modulator::Description::SpriteOffset.name, true);
                    STATIC_PROPERTY_SPLIT(Type, Name, sound, ::Atmos::Modulator::Description::Sound.name, ::Atmos::Modulator::Description::Sound.name, true);
                    STATIC_PROPERTY_SPLIT(Type, Name, soundOffset, ::Atmos::Modulator::Description::SoundOffset.name, ::Atmos::Modulator::Description::SoundOffset.name, true);
                    STATIC_PROPERTY_SPLIT(Type, Name, avEffect, ::Atmos::Modulator::Description::AVEffect.name, ::Atmos::Modulator::Description::AVEffect.name, true);
                    STATIC_PROPERTY_SPLIT(Type, Name, avEffectOffset, ::Atmos::Modulator::Description::AVEffectOffset.name, ::Atmos::Modulator::Description::AVEffectOffset.name, true);
                    STATIC_PROPERTY_SPLIT(Type, Name, senseComponent, ::Atmos::Modulator::Description::SenseComponent.name, ::Atmos::Modulator::Description::SenseComponent.name, true);

                // ModulatorTrackType
                CLASS_NAME(TrackType, "AtmosModulator_TrackType");
                    STATIC_PROPERTY_SPLIT(TrackType, Name, positionX, ::Atmos::Modulator::Description::Track::PositionX.name, ::Atmos::Modulator::Description::Track::PositionX.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, positionY, ::Atmos::Modulator::Description::Track::PositionY.name, ::Atmos::Modulator::Description::Track::PositionY.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, positionZ, ::Atmos::Modulator::Description::Track::PositionZ.name, ::Atmos::Modulator::Description::Track::PositionZ.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, scalingX, ::Atmos::Modulator::Description::Track::ScalingX.name, ::Atmos::Modulator::Description::Track::ScalingX.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, scalingY, ::Atmos::Modulator::Description::Track::ScalingY.name, ::Atmos::Modulator::Description::Track::ScalingY.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, scalingZ, ::Atmos::Modulator::Description::Track::ScalingZ.name, ::Atmos::Modulator::Description::Track::ScalingZ.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, rotationX, ::Atmos::Modulator::Description::Track::RotationX.name, ::Atmos::Modulator::Description::Track::RotationX.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, rotationY, ::Atmos::Modulator::Description::Track::RotationY.name, ::Atmos::Modulator::Description::Track::RotationY.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, rotationZ, ::Atmos::Modulator::Description::Track::RotationZ.name, ::Atmos::Modulator::Description::Track::RotationZ.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, colorA, ::Atmos::Modulator::Description::Track::ColorA.name, ::Atmos::Modulator::Description::Track::ColorA.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, colorR, ::Atmos::Modulator::Description::Track::ColorR.name, ::Atmos::Modulator::Description::Track::ColorR.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, colorG, ::Atmos::Modulator::Description::Track::ColorG.name, ::Atmos::Modulator::Description::Track::ColorG.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, colorB, ::Atmos::Modulator::Description::Track::ColorB.name, ::Atmos::Modulator::Description::Track::ColorB.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, index, ::Atmos::Modulator::Description::Track::Index.name, ::Atmos::Modulator::Description::Track::Index.name, true);
                    STATIC_PROPERTY_SPLIT(TrackType, Name, volume, ::Atmos::Modulator::Description::Track::Volume.name, ::Atmos::Modulator::Description::Track::Volume.name, true);

                // Modulator
                CLASS_NAME(Modulator, "AtmosModulator");
                CONSTRUCTOR(Modulator)
                    PARAMETERS(new Parameter<::Atmos::Modulator::Controller::ID>("modID"))
                BODY(Modulator)
                {
                    SETUP_CONSTRUCTOR(Modulator);

                    self->modID.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Modulator::Controller::ID>("modID")->obj);
                } BODY_END;
                METHOD(Modulator, createTrack, "CreateTrack", false)
                    PARAMETERS(new Parameter<Name>("name"))
                BODY(Modulator)
                {
                    SETUP_METHOD(Modulator, createTrack);
                    auto name = selfMethod.GetParameter<Name>("name");
                    auto &retrievedModID = self->modID.Retrieve();

                    ::Atmos::Modulator::Observer mod(GameEnvironment::GetModulatorController().Find(retrievedModID));
                    if (!mod)
                    {
                        // Log error if not found
                        Logger::Log(AddTracebackToString(*vm, "A modulator was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("ID", retrievedModID) });
                        return;
                    }

                    auto &madeTrack = GameEnvironment::GenerateModulatorTrack(mod->GetGeneratorName(), **name);
                    if (!madeTrack)
                    {
                        // Log error if not made
                        Logger::Log(AddTracebackToString(*vm, "A modulator track was attempted to be created with an invalid name."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Track Name", name->obj) });
                        return;
                    }

                    // Make track
                    auto madeTrackID = mod->AddTrack(std::move(madeTrack));
                    // Setup the Track class, then create it as an item and return
                    Track::Scaffolding().modID.Set(retrievedModID);
                    Track::Scaffolding().trackID.Set(madeTrackID);
                    vm->retval(Track::Scaffolding().CreateItem(*vm));
                } METHOD_END;
                METHOD(Modulator, forceStop, "ForceStop", false)
                    NO_PARAMETERS
                BODY(Modulator)
                {
                    SETUP_METHOD(Modulator, forceStop);

                    auto &retrievedModID = self->modID.Retrieve();

                    ::Atmos::Modulator::Observer mod(GameEnvironment::GetModulatorController().Find(retrievedModID));
                    if (!mod)
                    {
                        // Log error if not found
                        Logger::Log(AddTracebackToString(*vm, "A modulator was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("ID", retrievedModID) });
                        return;
                    }

                    GameEnvironment::GetModulatorController().ForceStop(mod);
                } METHOD_END;
                DEFINE_CONSTRUCTOR_1_PROPERTIES(Modulator, modID);

                // ModulatorTrack
                CLASS_NAME(Track, "AtmosModulator_Track");
                CONSTRUCTOR(Track)
                    PARAMETERS(new Parameter<::Atmos::Modulator::Controller::ID>("modID"), new Parameter<::Atmos::Modulator::ModulatorBase::TrackID>("trackID"))
                BODY(Track)
                {
                    SETUP_CONSTRUCTOR(Track);

                    self->modID.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Modulator::Controller::ID>("modID")->obj);
                    self->trackID.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Modulator::ModulatorBase::TrackID>("trackID")->obj);
                } BODY_END;
                METHOD(Track, createNode, "CreateNode", false)
                    NO_PARAMETERS
                BODY(Track)
                {
                    SETUP_METHOD(Track, createNode);
                    auto &retrievedModID = self->modID.Retrieve();
                    auto &retrievedTrackID = self->trackID.Retrieve();

                    ::Atmos::Modulator::Observer foundMod = GameEnvironment::GetModulatorController().Find(retrievedModID);
                    if (!foundMod)
                    {
                        // Log error if not found
                        Logger::Log(AddTracebackToString(*vm, "A modulator was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Mod ID", retrievedModID) });
                        return;
                    }

                    auto foundTrack = foundMod->FindTrack(retrievedTrackID);
                    if (!foundTrack)
                    {
                        // Log error if not made
                        Logger::Log(AddTracebackToString(*vm, "A modulator track was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Track ID", retrievedTrackID) });
                        return;
                    }

                    // Make node
                    auto madeNodeID = foundTrack->AddNode();
                    // Setup the TrackNode class, then create it as an item and return
                    TrackNode::Scaffolding().modID.Set(retrievedModID);
                    TrackNode::Scaffolding().trackID.Set(retrievedTrackID);
                    TrackNode::Scaffolding().nodeID.Set(madeNodeID);
                    vm->retval(TrackNode::Scaffolding().CreateItem(*vm));
                } METHOD_END;
                METHOD(Track, setDefaultStartValue, "SetDefaultStartValue", false)
                    PARAMETERS(new Parameter<::function::Variant<std::int64_t, float>>("value"))
                BODY(Track)
                {
                    SETUP_METHOD(Track, setDefaultStartValue);

                    auto &retrievedModID = self->modID.Retrieve();
                    auto &retrievedTrackID = self->trackID.Retrieve();

                    ::Atmos::Modulator::Observer foundMod = GameEnvironment::GetModulatorController().Find(retrievedModID);
                    if (!foundMod)
                    {
                        // Log error if not found
                        Logger::Log(AddTracebackToString(*vm, "A modulator was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Mod ID", retrievedModID) });
                        return;
                    }

                    auto foundTrack = foundMod->FindTrack(retrievedTrackID);
                    if (!foundTrack)
                    {
                        // Log error if not made
                        Logger::Log(AddTracebackToString(*vm, "A modulator track was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Track ID", retrievedTrackID) });
                        return;
                    }

                    // Setup the default starting state
                    auto &value = *selfMethod.GetParameter<::function::Variant<std::int64_t, float>>("value");

                    ::Atmos::Modulator::Value state;
                    if (value->Is<std::int64_t>())
                        state.Set(value->Get<std::int64_t>());
                    else
                        state.Set(value->Get<float>());
                    foundTrack->SetDefaultStartValue(state);
                } METHOD_END;
                METHOD(Track, resetDefaultStartValue, "ResetDefaultStartValue", false)
                    NO_PARAMETERS
                BODY(Track)
                {
                    SETUP_METHOD(Track, setDefaultStartValue);

                    auto &retrievedModID = self->modID.Retrieve();
                    auto &retrievedTrackID = self->trackID.Retrieve();

                    ::Atmos::Modulator::Observer foundMod = GameEnvironment::GetModulatorController().Find(retrievedModID);
                    if (!foundMod)
                    {
                        // Log error if not found
                        Logger::Log(AddTracebackToString(*vm, "A modulator was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Mod ID", retrievedModID) });
                        return;
                    }

                    auto foundTrack = foundMod->FindTrack(retrievedTrackID);
                    if (!foundTrack)
                    {
                        // Log error if not made
                        Logger::Log(AddTracebackToString(*vm, "A modulator track was attempted to be found with an invalid ID."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Track ID", retrievedTrackID) });
                        return;
                    }

                    // Reset the default starting value
                    foundTrack->ResetDefaultStartValue();
                } METHOD_END;
                DEFINE_CONSTRUCTOR_2_PROPERTIES(Track, modID, trackID);

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
                CONSTRUCTOR(TrackNode)
                    PARAMETERS(new Parameter<::Atmos::Modulator::Controller::ID>("modID"), new Parameter<::Atmos::Modulator::ModulatorBase::TrackID>("trackID"), new Parameter<::Atmos::Modulator::TrackBase::NodeID>("nodeID"))
                BODY(TrackNode)
                {
                    SETUP_CONSTRUCTOR(TrackNode);
                    self->modID.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Modulator::Controller::ID>("modID")->obj);
                    self->trackID.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Modulator::ModulatorBase::TrackID>("trackID")->obj);
                    self->nodeID.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Modulator::TrackBase::NodeID>("nodeID")->obj);
                } BODY_END;
                METHOD(TrackNode, setInterpolationLinear, "SetInterpolationLinear", false)
                    NO_PARAMETERS
                BODY(TrackNode)
                {
                    SETUP_METHOD(TrackNode, setInterpolationLinear);

                    ::Atmos::Modulator::TrackNode *node = nullptr;
                    if (!RetrieveNode(*vm, node, self->modID.Retrieve(), self->trackID.Retrieve(), self->nodeID.Retrieve()))
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
                    SETUP_METHOD(TrackNode, setInterpolationOscillation);

                    ::Atmos::Modulator::TrackNode *node = nullptr;
                    if (!RetrieveNode(*vm, node, self->modID.Retrieve(), self->trackID.Retrieve(), self->nodeID.Retrieve()))
                    {
                        vm->retnil();
                        return;
                    }

                    auto acceleration = selfMethod.GetParameter<::Atmos::Modulator::TrackNodeInterpolation::Acceleration>("acceleration");
                    auto rate = selfMethod.GetParameter<::Atmos::Modulator::TrackNodeInterpolation::Rate>("rate");

                    node->SetInterpolation(::Atmos::Modulator::TrackNodeInterpolation(**acceleration, **rate));
                } METHOD_END;
                METHOD(TrackNode, setEndState, "SetEndState", false)
                    PARAMETERS(new Parameter<::function::Variant<std::int64_t, float>>("value"))
                BODY(TrackNode)
                {
                    SETUP_METHOD(TrackNode, setEndState);

                    ::Atmos::Modulator::TrackNode *node = nullptr;
                    if (!RetrieveNode(*vm, node, self->modID.Retrieve(), self->trackID.Retrieve(), self->nodeID.Retrieve()))
                    {
                        vm->retnil();
                        return;
                    }

                    // Setup the end state
                    auto &value = selfMethod.GetParameter<::function::Variant<std::int64_t, float>>("value")->obj;

                    ::Atmos::Modulator::Value endState;
                    if (value.Is<std::int64_t>())
                        endState.Set(value.Get<std::int64_t>());
                    else
                        endState.Set(value.Get<float>());
                    node->SetEndState(endState);
                } METHOD_END;
                METHOD(TrackNode, setTimeTaken, "SetTimeTaken", false)
                    PARAMETERS(new Parameter<Time::TimeValue>("value"))
                BODY(TrackNode)
                {
                    SETUP_METHOD(TrackNode, setTimeTaken);

                    ::Atmos::Modulator::TrackNode *node = nullptr;
                    if (!RetrieveNode(*vm, node, self->modID.Retrieve(), self->trackID.Retrieve(), self->nodeID.Retrieve()))
                    {
                        vm->retnil();
                        return;
                    }

                    // Setup the time taken
                    auto value = selfMethod.GetParameter<Time::TimeValue>("value");
                    FixedPoint64 fixedPoint(RadixPoint(9));
                    fixedPoint.SetRawValue((*value)->value.Retrieve().value.Retrieve());
                    ::Atmos::TimeValue timeValue(fixedPoint, (*value)->epoch.Retrieve());

                    node->SetTimeTaken(timeValue);
                } METHOD_END;
                DEFINE_CONSTRUCTOR_3_PROPERTIES(TrackNode, modID, trackID, nodeID);
            }

            namespace Sense
            {
                CLASS_NAME(Sprite, "AtmosSense_Sprite")
                CONSTRUCTOR(Sprite)
                    PARAMETERS(new Parameter<ScriptLocatorManager::ID>("locator"))
                BODY(Sprite)
                {
                    SETUP_CONSTRUCTOR(Sprite);
                    self->locator.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<ScriptLocatorManager::ID>("locator")->obj);
                } BODY_END;
                METHOD(Sprite, setIndex, "SetIndex", false)
                    PARAMETERS(new Parameter<::Atmos::Sprite::Index>("set"))
                BODY(Sprite)
                {
                    SETUP_METHOD(Sprite, setIndex);

                    auto foundSprite = ScriptLocatorManager::Find<SenseComponentSpriteLocator::TypeLocating>(self->locator.Retrieve());
                    if (!foundSprite)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A sprite was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE);
                        return;
                    }

                    auto setIndex = selfMethod.GetParameter<::Atmos::Sprite::Index>("set");
                    foundSprite->Get().SetIndex(**setIndex);
                } METHOD_END;
                METHOD(Sprite, getIndex, "GetIndex", false)
                    PARAMETERS(new Parameter<::Atmos::Sprite::Index>("set"))
                BODY(Sprite)
                {
                    SETUP_METHOD(Sprite, getIndex);

                    auto foundSprite = ScriptLocatorManager::Find<SenseComponentSpriteLocator::TypeLocating>(self->locator.Retrieve());
                    if (!foundSprite)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A sprite was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE);
                        return;
                    }

                    vm->retval(foundSprite->Get().GetIndex());
                } METHOD_END;
                METHOD(Sprite, startModulatorWith, "StartModulatorWith", false)
                    PARAMETERS(new Parameter<Modulator::Modulator>("start"), new Parameter<bool>("force", false, false))
                BODY(Sprite)
                {
                    SETUP_METHOD(Sprite, startModulatorWith);

                    auto start = selfMethod.GetParameter<Modulator::Modulator>("start");
                    auto force = selfMethod.GetParameter<bool>("force");

                    auto foundSprite = ScriptLocatorManager::Find<SenseComponentSpriteLocator::TypeLocating>(self->locator.Retrieve());
                    if (!foundSprite)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A sprite was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE);
                        return;
                    }

                    // Get modulator
                    auto modID = (*start)->modID.Retrieve();
                    auto modulator = ::Atmos::GameEnvironment::GetModulatorController().Find(modID);
                    if (!modulator)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A modulator was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Modulator ID", modID) });
                        return;
                    }

                    // Make sure modulator is the right type
                    if (modulator->GetGeneratorName() != ::Atmos::Modulator::Description::SpriteOffset.name)
                    {
                        Logger::Log(AddTracebackToString(*vm, "The modulator is the incorrect type."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Modulator ID", modID),
                            NameValuePair("Required Type", ::Atmos::Modulator::Description::SpriteOffset.name),
                            NameValuePair("Actual Type", modulator->GetGeneratorName()) });
                        return;
                    }

                    // If forcing wasn't set...
                    if (!force->WasSet() || !force->obj)
                    {
                        // And there's a modulator working the object
                        if (GameEnvironment::GetModulatorController().IsModulatorWorkingObject(foundSprite))
                            // Return silently
                            return;
                    }

                    // Finally, attach the modulator
                    auto castedModulator = static_cast<decltype(::Atmos::Modulator::Description::SpriteOffset)::ModulatorT*>(modulator.Get());
                    castedModulator->Start(*foundSprite);
                } METHOD_END;

                Sprite::Sprite() : PROPERTY_SPLIT(locator, "_locator")
                {}

                Sprite::Sprite(const Sprite &arg) : COPY_PROPERTY(locator)
                {}

                Sprite::Sprite(Sprite &&arg) : MOVE_PROPERTY(locator)
                {}

                Sprite& Sprite::operator=(Sprite &&arg)
                {
                    Definition::operator=(std::move(arg));
                    locator = std::move(arg.locator);
                    return *this;
                }
            }

            namespace Ent
            {
                CLASS_NAME(ComponentType, "AtmosEntity_ComponentType");
                    STATIC_PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, action, "Action", ::Atmos::Ent::ActionComponent::TypeName(), true);
                    STATIC_PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, battle, "Battle", ::Atmos::Ent::BattleComponent::TypeName(), true);
                    STATIC_PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, ai, "AI", ::Atmos::Ent::AIComponent::TypeName(), true);
                    STATIC_PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, avatar, "Avatar", ::Atmos::Ent::AvatarComponent::TypeName(), true);
                    STATIC_PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, character, "Character", ::Atmos::Ent::CharacterComponent::TypeName(), true);
                    STATIC_PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, combat, "Combat", ::Atmos::Ent::CombatComponent::TypeName(), true);
                    STATIC_PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, dialogue, "Dialogue", ::Atmos::Ent::DialogueComponent::TypeName(), true);
                    STATIC_PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, general, "General", ::Atmos::Ent::GeneralComponent::TypeName(), true);
                    STATIC_PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, inventory, "Inventory", ::Atmos::Ent::InventoryComponent::TypeName(), true);
                    STATIC_PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, movement, "Movement", ::Atmos::Ent::MovementComponent::TypeName(), true);
                    STATIC_PROPERTY_SPLIT(ComponentType, ::Atmos::Ent::ComponentTypeName, sense, "Sense", ::Atmos::Ent::SenseComponent::TypeName(), true);

                // ActionComponent
                CLASS_NAME(ActionComponent, "AtmosEntity_ActionComponent");
                CONSTRUCTOR(ActionComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(ActionComponent)
                {
                    SETUP_CONSTRUCTOR(ActionComponent);
                    self->entity.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Entity::ValueT>("entity")->obj);
                } BODY_END;
                DEFINE_CONSTRUCTOR_1_PROPERTIES(ActionComponent, entity);

                // BattleComponent
                CLASS_NAME(BattleComponent, "AtmosEntity_BattleComponent");
                CONSTRUCTOR(BattleComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(BattleComponent)
                {
                    SETUP_CONSTRUCTOR(BattleComponent);
                    self->entity.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Entity::ValueT>("entity")->obj);
                } BODY_END;
                DEFINE_CONSTRUCTOR_1_PROPERTIES(BattleComponent, entity);

                // AIComponent
                CLASS_NAME(AIComponent, "AtmosEntity_AIComponent");
                CONSTRUCTOR(AIComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(AIComponent)
                {
                    SETUP_CONSTRUCTOR(AIComponent);
                    self->entity.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Entity::ValueT>("entity")->obj);
                } BODY_END;
                DEFINE_CONSTRUCTOR_1_PROPERTIES(AIComponent, entity);

                // AvatarComponent
                CLASS_NAME(AvatarComponent, "AtmosEntity_AvatarComponent");
                CONSTRUCTOR(AvatarComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(AvatarComponent)
                {
                    SETUP_CONSTRUCTOR(AvatarComponent);
                    self->entity.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Entity::ValueT>("entity")->obj);
                } BODY_END;
                METHOD(AvatarComponent, getGold, "GetGold", false)
                    NO_PARAMETERS
                BODY(AvatarComponent)
                {
                    SETUP_METHOD(AvatarComponent, getGold);

                    // Get entity
                    auto &entity = self->entity.Retrieve();

                    // Get avatar component
                    auto avatarComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::AvatarComponent>(entity);
                    if (!avatarComponent)
                        vm->retval(0);

                    vm->retval(static_cast<std::int64_t>(avatarComponent->gold));
                } METHOD_END;
                DEFINE_CONSTRUCTOR_1_PROPERTIES(AvatarComponent, entity);

                // CharacterComponent
                CLASS_NAME(CharacterComponent, "AtmosEntity_CharacterComponent");
                CONSTRUCTOR(CharacterComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(CharacterComponent)
                {
                    SETUP_CONSTRUCTOR(CharacterComponent);
                    self->entity.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Entity::ValueT>("entity")->obj);
                } BODY_END;
                DEFINE_CONSTRUCTOR_1_PROPERTIES(CharacterComponent, entity);

                // CombatComponent
                CLASS_NAME(CombatComponent, "AtmosEntity_CombatComponent");
                CONSTRUCTOR(CombatComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(CombatComponent)
                {
                    SETUP_CONSTRUCTOR(CombatComponent);
                    self->entity.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Entity::ValueT>("entity")->obj);
                } BODY_END;
                DEFINE_CONSTRUCTOR_1_PROPERTIES(CombatComponent, entity);

                // DialogueComponent
                CLASS_NAME(DialogueComponent, "AtmosEntity_DialogueComponent");
                CONSTRUCTOR(DialogueComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(DialogueComponent)
                {
                    SETUP_CONSTRUCTOR(DialogueComponent);
                    self->entity.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Entity::ValueT>("entity")->obj);
                } BODY_END;
                DEFINE_CONSTRUCTOR_1_PROPERTIES(DialogueComponent, entity);

                // GeneralComponent
                CLASS_NAME(GeneralComponent, "AtmosEntity_GeneralComponent");
                CONSTRUCTOR(GeneralComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(GeneralComponent)
                {
                    SETUP_CONSTRUCTOR(GeneralComponent);
                    self->entity.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Entity::ValueT>("entity")->obj);
                } BODY_END;
                DEFINE_CONSTRUCTOR_1_PROPERTIES(GeneralComponent, entity);

                // InventoryComponent
                CLASS_NAME(InventoryComponent, "AtmosEntity_InventoryComponent");
                CONSTRUCTOR(InventoryComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(InventoryComponent)
                {
                    SETUP_CONSTRUCTOR(InventoryComponent);
                    self->entity.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Entity::ValueT>("entity")->obj);
                } BODY_END;
                METHOD(InventoryComponent, getTotalItemCount, "GetTotalItemCount", false)
                    NO_PARAMETERS
                BODY(InventoryComponent)
                {
                    SETUP_METHOD(InventoryComponent, getTotalItemCount);

                    // Get entity
                    auto &entity = self->entity.Retrieve();

                    // Get inventory component
                    auto inventoryComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::InventoryComponent>(entity);
                    if (!inventoryComponent)
                        vm->retval(0);

                    vm->retval(static_cast<std::int64_t>(inventoryComponent->GetTotalCount()));
                } METHOD_END;
                DEFINE_CONSTRUCTOR_1_PROPERTIES(InventoryComponent, entity);

                // MovementComponent
                CLASS_NAME(MovementComponent, "AtmosEntity_MovementComponent");
                CONSTRUCTOR(MovementComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(MovementComponent)
                {
                    SETUP_CONSTRUCTOR(MovementComponent);
                    self->entity.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Entity::ValueT>("entity")->obj);
                } BODY_END;
                METHOD(MovementComponent, moveDirection, "MoveDirection", false)
                    PARAMETERS(new Parameter<::Atmos::Direction::ValueT>("direction"))
                BODY(MovementComponent)
                {
                    SETUP_METHOD(MovementComponent, moveDirection);

                    auto direction = selfMethod.GetParameter<::Atmos::Direction::ValueT>("direction");

                    // Get entity
                    auto &entity = self->entity.Retrieve();

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
                    SETUP_METHOD(MovementComponent, moveInstant);

                    auto position = selfMethod.GetParameter<Position::GridPosition>("position");

                    // Get entity
                    auto &entity = self->entity.Retrieve();

                    // Get movement component
                    auto movementComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::MovementComponent>(entity);
                    if (!movementComponent)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A movement component was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)) });
                        return;
                    }

                    ::Atmos::GridPosition giveToPositionSystem((*position)->x.Retrieve(), (*position)->y.Retrieve(), (*position)->z.Retrieve());
                    ::Atmos::Ent::PositionSystem::MoveEntityInstant(entity, giveToPositionSystem);
                } METHOD_END;
                METHOD(MovementComponent, canMove, "CanMove", false)
                    PARAMETERS(new Parameter<Position::GridPosition>("position", false))
                BODY(MovementComponent)
                {
                    SETUP_METHOD(MovementComponent, canMove);

                    auto position = selfMethod.GetParameter<Position::GridPosition>("position");

                    // Get entity
                    auto &entity = self->entity.Retrieve();

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
                        ::Atmos::GridPosition positionToPass((*position)->x.Retrieve(), (*position)->y.Retrieve(), (*position)->z.Retrieve());
                        vm->retval(::Atmos::Ent::PositionSystem::CanMove(*movementComponent, positionToPass));
                    }
                } METHOD_END;
                METHOD(MovementComponent, completeMovementSetup, "CompleteMovementSetup", false)
                    PARAMETERS(new Parameter<Modulator::Modulator>("modulator"), new Parameter<bool>("force", false, false))
                BODY(MovementComponent)
                {
                    SETUP_METHOD(MovementComponent, completeMovementSetup);

                    auto modulatorPass = selfMethod.GetParameter<Modulator::Modulator>("modulator");
                    auto force = selfMethod.GetParameter<bool>("force");

                    // Get entity
                    auto &entity = self->entity.Retrieve();

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
                    auto modID = (*modulatorPass)->modID.Retrieve();
                    auto modulator = ::Atmos::GameEnvironment::GetModulatorController().Find(modID);
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
                            Logger::NameValueVector{ NameValuePair("Modulator ID", modID),
                            NameValuePair("Required Type", ::Atmos::Modulator::Description::SenseComponent.name),
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
                DEFINE_CONSTRUCTOR_1_PROPERTIES(MovementComponent, entity);

                // SenseComponent
                CLASS_NAME(SenseComponent, "AtmosEntity_SenseComponent");
                CONSTRUCTOR(SenseComponent)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("entity"))
                BODY(SenseComponent)
                {
                    SETUP_CONSTRUCTOR(SenseComponent);
                    self->entity.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Entity::ValueT>("entity")->obj);
                } BODY_END;
                METHOD(SenseComponent, getPosition, "GetPosition", false)
                    NO_PARAMETERS
                BODY(SenseComponent)
                {
                    SETUP_METHOD(SenseComponent, getPosition);

                    // Get entity
                    auto &entity = self->entity.Retrieve();
                    // Get sense component
                    auto senseComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::SenseComponent>(entity);
                    if (!senseComponent)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A sense component was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)) });
                        return;
                    }

                    Position::Position3D::Scaffolding().x.Set(senseComponent->position.GetX());
                    Position::Position3D::Scaffolding().y.Set(senseComponent->position.GetY());
                    Position::Position3D::Scaffolding().z.Set(senseComponent->position.GetZ());
                    vm->retval(Position::Position3D::Scaffolding().CreateItem(*vm));
                } METHOD_END;
                METHOD(SenseComponent, getSprite, "GetSprite", false)
                    PARAMETERS(new Parameter<::Atmos::Ent::SenseComponent::SpriteList::ID>("ID"))
                BODY(SenseComponent)
                {
                    SETUP_METHOD(SenseComponent, getSprite);

                    // Get entity
                    auto &entity = self->entity.Retrieve();
                    // Get sense component
                    auto senseComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::SenseComponent>(entity);
                    if (!senseComponent)
                    {
                        Logger::Log(AddTracebackToString(*vm, "A sense component was required but wasn't found."),
                            Logger::Type::ERROR_MODERATE,
                            Logger::NameValueVector{ NameValuePair("Entity", static_cast<size_t>(entity)) });
                        return;
                    }

                    auto id = selfMethod.GetParameter<::Atmos::Ent::SenseComponent::SpriteList::ID>("ID");
                    auto foundSprite = senseComponent->FindSprite(**id);
                    if (!foundSprite)
                    {
                        vm->retnil();
                        return;
                    }

                    Sense::Sprite::Scaffolding().locator.Set(ScriptLocatorManager::Add(*new SenseComponentSpriteLocator(entity, **id, *ScriptController::Current())));
                    vm->retval(Sense::Sprite::Scaffolding().CreateItem(*vm));
                } METHOD_END;
                METHOD(SenseComponent, getStagedPosition, "GetStagedPosition", false)
                    NO_PARAMETERS
                BODY(SenseComponent)
                {
                    SETUP_METHOD(SenseComponent, getStagedPosition);

                    // Get entity
                    auto &entity = self->entity.Retrieve();
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
                    if (!stagedPosition)
                    {
                        Logger::Log(AddTracebackToString(*vm, "There is no staged position."),
                            Logger::Type::ERROR_MODERATE);
                        return;
                    }

                    Position::Position3D::Scaffolding().x.Set(::Atmos::GridPosition::DimensionToPosition(stagedPosition->position.GetX()));
                    Position::Position3D::Scaffolding().y.Set(::Atmos::GridPosition::DimensionToPosition(stagedPosition->position.GetY()));
                    Position::Position3D::Scaffolding().z.Set(::Atmos::GridPosition::DimensionToPosition(stagedPosition->position.GetZ()));
                    vm->retval(Position::Position3D::Scaffolding().CreateItem(*vm));
                } METHOD_END;
                METHOD(SenseComponent, getStagedDirection, "GetStagedDirection", false)
                    NO_PARAMETERS
                BODY(SenseComponent)
                {
                    SETUP_METHOD(SenseComponent, getStagedDirection);

                    // Get entity
                    auto &entity = self->entity.Retrieve();
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
                    if (stagedPosition)
                    {
                        vm->retval(FalconVariableTraitsEnum<Direction::ValueT>::CreateItem(*vm, stagedPosition->direction.Get()));
                        return;
                    }

                    Optional<::Atmos::Ent::PositionSystem::StagedDirection> stagedDirection = ::Atmos::Ent::PositionSystem::GetStagedDirection(entity);
                    if (!stagedDirection)
                    {
                        Logger::Log(AddTracebackToString(*vm, "There is no staged direction."),
                            Logger::Type::ERROR_MODERATE);
                        return;
                    }

                    vm->retval(FalconVariableTraitsEnum<Direction::ValueT>::CreateItem(*vm, stagedDirection->direction.Get()));
                } METHOD_END;
                METHOD(SenseComponent, hasStagedPosition, "HasStagedPosition", false)
                    NO_PARAMETERS
                BODY(SenseComponent)
                {
                    SETUP_METHOD(SenseComponent, hasStagedPosition);

                    // Get entity
                    auto &entity = self->entity.Retrieve();
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
                    vm->retval(stagedPosition.IsValid());
                } METHOD_END;
                METHOD(SenseComponent, hasStagedDirection, "HasStagedDirection", false)
                    NO_PARAMETERS
                BODY(SenseComponent)
                {
                    SETUP_METHOD(SenseComponent, hasStagedDirection);

                    // Get entity
                    auto &entity = self->entity.Retrieve();
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
                    if (stagedPosition)
                    {
                        vm->retval(true);
                        return;
                    }

                    Optional<::Atmos::Ent::PositionSystem::StagedDirection> stagedDirection = ::Atmos::Ent::PositionSystem::GetStagedDirection(entity);
                    vm->retval(stagedDirection.IsValid());
                } METHOD_END;
                DEFINE_CONSTRUCTOR_1_PROPERTIES(SenseComponent, entity);

                // Entity
                CLASS_NAME(Entity, "AtmosEntity");
                CONSTRUCTOR(Entity)
                    PARAMETERS(new Parameter<::Atmos::Entity::ValueT>("value"))
                BODY(Entity)
                {
                    SETUP_CONSTRUCTOR(Entity);
                    self->value.SetPropertyTo(*vm, vm->self(), selfMethod.GetParameter<::Atmos::Entity::ValueT>("value")->obj);
                } BODY_END;
                // Has component
                METHOD(Entity, hasComponent, "HasComponent", false)
                    PARAMETERS(new Parameter<::Atmos::Ent::ComponentTypeName>("typeName"))
                BODY(Entity)
                {
                    SETUP_METHOD(Entity, hasComponent);

                    auto typeName = selfMethod.GetParameter<::Atmos::Ent::ComponentTypeName>("typeName");
                    // Get value
                    auto &entity = self->value.Retrieve();
                    // Return if we could find the component
                    vm->retval(GetCurrentEntities()->FindComponent(entity, **typeName));
                } METHOD_END;
                DEFINE_CONSTRUCTOR_1_PROPERTIES(Entity, value);

                template<class ComponentClassT>
                bool AttemptCreateComponent(::Atmos::Entity entity, const ::Atmos::Ent::ComponentTypeName &check, const ::Atmos::Ent::ComponentTypeName &checkAgainst, Falcon::VMachine &vm)
                {
                    if (check != checkAgainst)
                        return false;

                    ComponentClassT::Scaffolding().entity.Set(entity);
                    vm.retval(ComponentClassT::Scaffolding().CreateItem(vm));
                    return true;
                }
                // Get component
                METHOD(Entity, getComponent, "GetComponent", false)
                    PARAMETERS(new Parameter<::Atmos::Ent::ComponentTypeName>("typeName"))
                BODY(Entity)
                {
                    SETUP_METHOD(Entity, getComponent);

                    auto typeName = selfMethod.GetParameter<::Atmos::Ent::ComponentTypeName>("typeName");
                    // Get value
                    auto &entity = self->value.Retrieve();
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
                METHOD(Entity, setDirection, "SetDirection", false)
                    PARAMETERS(new Parameter<::Atmos::Direction::ValueT>("set"))
                BODY(Entity)
                {
                    SETUP_METHOD(Entity, setDirection);

                    // Get value
                    auto &entity = self->value.Retrieve();

                    auto foundDirection = GetCurrentEntities()->GetDirection(entity);
                    if (!foundDirection)
                        return;

                    auto set = selfMethod.GetParameter<::Atmos::Direction::ValueT>("set");
                    GetCurrentEntities()->FindComponent<::Atmos::Ent::GeneralComponent>(entity)->direction = **set;
                } METHOD_END;
                METHOD(Entity, getDirection, "GetDirection", false)
                    NO_PARAMETERS
                BODY(Entity)
                {
                    SETUP_METHOD(Entity, getDirection);

                    // Get value
                    auto &entity = self->value.Retrieve();

                    auto foundDirection = GetCurrentEntities()->GetDirection(entity);
                    if (!foundDirection)
                    {
                        vm->retnil();
                        return;
                    }

                    vm->retval(static_cast<std::int64_t>(foundDirection->Get()));
                } METHOD_END;
                METHOD(Entity, getPosition, "GetPosition", false)
                    NO_PARAMETERS
                BODY(Entity)
                {
                    SETUP_METHOD(Entity, getPosition);

                    // Get value
                    auto &entity = self->value.Retrieve();

                    auto foundPosition = GetCurrentEntities()->GetPosition(entity);
                    if (!foundPosition)
                    {
                        vm->retnil();
                        return;
                    }

                    Position::GridPosition::Scaffolding().x.Set(foundPosition->GetX());
                    Position::GridPosition::Scaffolding().y.Set(foundPosition->GetY());
                    Position::GridPosition::Scaffolding().z.Set(foundPosition->GetZ());
                    vm->retval(Position::GridPosition::Scaffolding().CreateItem(*vm));
                } METHOD_END;
            }
        }
    }
}