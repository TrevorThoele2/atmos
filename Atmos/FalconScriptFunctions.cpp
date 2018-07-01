
#include "FalconScriptFunctions.h"
#include "FalconScriptObjects.h"

#include "CurrentField.h"
#include "ModulatorObserver.h"
#include "GameEnvironment.h"
#include "ActionFunctions.h"
#include "AvatarSystem.h"

#include <falcon/engine.h>

#define FUNCTION(objectName, falconName)    \
    Prototype objectName(falconName, 

#define PARAMETERS(...)                     \
    std::vector<Prototype::ParameterBase*>{__VA_ARGS__

#define NO_PARAMETERS                       \
    std::vector<Prototype::ParameterBase*>{

#define BODY                                                        \
    }, static_cast<Prototype::FalconFuncT>([](Falcon::VMachine *vm)

#define BODY_END    \
    ))

#define SETUP_FUNCTION(selfObj)                             \
auto &selfFunc = selfObj;                                   \
if (!selfFunc.Setup(*vm))                                   \
    return;

// Doesn't return if the function wasn't setup
#define SETUP_FUNCTION_CONTINUE(selfObj)    \
auto &selfFunc = selfObj;                   \
selfFunc.Setup(*vm);

namespace Atmos
{
    namespace Fal
    {
        namespace Functions
        {
            namespace General
            {
                FUNCTION(Suspend, "Atmos_Suspend")
                    NO_PARAMETERS
                BODY
                {
                    ScriptController::Current()->Suspend();
                } BODY_END;

                FUNCTION(LogMessage, "Atmos_LogMessage")
                    PARAMETERS(new Prototype::Parameter<String>("message"), new Prototype::Parameter<Logger::Type>("type", false))
                BODY
                {
                    SETUP_FUNCTION(LogMessage);

                    auto message = selfFunc.GetParameter<String>("message");
                    auto type = selfFunc.GetParameter<Logger::Type>("type");
                    
                    if (!type->WasSet() || **type == Logger::Type::INFORMATION)
                        Logger::Log(**message, Logger::Type::INFORMATION);
                    else
                        Logger::Log(AddTracebackToString(*vm, **message), **type);
                } BODY_END;

                FUNCTION(RandomBool, "Atmos_RandomBool")
                    PARAMETERS(new Prototype::Parameter<double>("probability", false))
                BODY
                {
                    SETUP_FUNCTION_CONTINUE(RandomBool);

                    auto probability = selfFunc.GetParameter<double>("probability");
                    if (!probability->WasSet())
                        vm->retval(Act::Random::GenerateBool());
                    else
                        vm->retval(Act::Random::GenerateBool(**probability));
                } BODY_END;

                FUNCTION(RandomInteger, "Atmos_RandomInteger")
                    PARAMETERS(new Prototype::Parameter<std::int64_t>("floor"), new Prototype::Parameter<std::int64_t>("ceiling"))
                BODY
                {
                    SETUP_FUNCTION_CONTINUE(RandomInteger);

                    typedef std::int64_t T;

                    auto floor = selfFunc.GetParameter<T>("floor");
                    auto ceiling = selfFunc.GetParameter<T>("ceiling");
                    if (!floor->WasSet() || !ceiling->WasSet())
                        vm->retval(Act::Random::GenerateIntFullRange<T>());
                    else
                        vm->retval(Act::Random::GenerateInt<T>(**floor, **ceiling));
                } BODY_END;
            }

            namespace Position
            {
                FUNCTION(FindPath, "Atmos_FindPath")
                    PARAMETERS(new Prototype::Parameter<Classes::Position::GridPosition>("from"), new Prototype::Parameter<Classes::Position::GridPosition>("to"))
                BODY
                {
                    SETUP_FUNCTION(FindPath);

                    auto &from = selfFunc.GetParameter<Classes::Position::GridPosition>("from")->obj;
                    auto &to = selfFunc.GetParameter<Classes::Position::GridPosition>("to")->obj;

                    ::Atmos::GridPosition atmosFrom(from.x.Retrieve(), from.y.Retrieve(), from.z.Retrieve());
                    ::Atmos::GridPosition atmosTo(to.x.Retrieve(), to.y.Retrieve(), to.z.Retrieve());

                    auto &path = ::Atmos::Act::Position::FindPath(atmosFrom, atmosTo);

                    auto sendPath = new Falcon::CoreArray();
                    while (!path.empty())
                    {
                        auto &top = path.top();
                        Classes::Position::GridPosition::Scaffolding().x.Set(top.GetX());
                        Classes::Position::GridPosition::Scaffolding().y.Set(top.GetY());
                        Classes::Position::GridPosition::Scaffolding().z.Set(top.GetZ());
                        sendPath->append(Classes::Position::GridPosition::Scaffolding().CreateItem(*vm));
                        path.pop();
                    }

                    vm->retval(sendPath);
                } BODY_END;
            }

            namespace Input
            {
                FUNCTION(IsActionActive, "AtmosInput_IsActionActive")
                    PARAMETERS(new Prototype::Parameter<::Atmos::Input::ActionID>("action"))
                BODY
                {
                    SETUP_FUNCTION(IsActionActive);
                    auto action = selfFunc.GetParameter<::Atmos::Input::ActionID>("action");

                    vm->retval(Act::Input::IsActionActive(action->obj));
                } BODY_END;
                FUNCTION(IsActionPressed, "AtmosInput_IsActionPressed")
                    PARAMETERS(new Prototype::Parameter<::Atmos::Input::ActionID>("action"))
                BODY
                {
                    SETUP_FUNCTION(IsActionPressed);
                    auto action = selfFunc.GetParameter<::Atmos::Input::ActionID>("action");

                    vm->retval(Act::Input::IsActionPressed(action->obj));
                } BODY_END;
                FUNCTION(IsActionDepressed, "AtmosInput_IsActionDepressed")
                    PARAMETERS(new Prototype::Parameter<::Atmos::Input::ActionID>("action"))
                BODY
                {
                    SETUP_FUNCTION(IsActionDepressed);
                    auto action = selfFunc.GetParameter<::Atmos::Input::ActionID>("action");

                    vm->retval(Act::Input::IsActionDepressed(action->obj));
                } BODY_END;
            }

            namespace Modulator
            {
                FUNCTION(Create, "AtmosModulator_Create")
                    PARAMETERS(new Prototype::Parameter<Name>("name"))
                BODY
                {
                    SETUP_FUNCTION(Create);
                    auto name = selfFunc.GetParameter<Name>("name");

                    // Create the modulator
                    ::Atmos::Modulator::Observer madeMod(GameEnvironment::GenerateModulator(**name));
                    if (!madeMod)
                    {
                        // Log error if not created
                        Logger::Log(AddTracebackToString(*vm, "A modulator was attempted to be created with an invalid name."),
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Name", **name) });
                        return;
                    }

                    // Return the ID for the new modulator
                    Classes::Modulator::Modulator::Scaffolding().modID.Set(GameEnvironment::GetModulatorController().Attach(madeMod));
                    vm->retval(Classes::Modulator::Modulator::Scaffolding().CreateItem(*vm));
                } BODY_END;
            }

            namespace Speech
            {
                FUNCTION(SetCharacters, "AtmosSpeech_SetCharacters")
                    PARAMETERS(new Prototype::Parameter<String>("output"))
                BODY
                {
                    SETUP_FUNCTION(SetCharacters);

                    auto output = selfFunc.GetParameter<String>("output");
                    Act::Speech::SetCharacters(output->obj);
                } BODY_END;

                FUNCTION(AppendCharacters, "AtmosSpeech_AppendCharacters")
                    PARAMETERS(new Prototype::Parameter<String>("append"))
                BODY
                {
                    SETUP_FUNCTION(AppendCharacters);

                    auto append = selfFunc.GetParameter<String>("append");
                    Act::Speech::AppendCharacters(append->obj);
                } BODY_END;

                FUNCTION(ClearCharacters, "AtmosSpeech_ClearCharacters")
                    NO_PARAMETERS
                BODY
                {
                    SETUP_FUNCTION(ClearCharacters);

                    Act::Speech::ClearCharacters();
                } BODY_END;

                FUNCTION(ActivateInput, "AtmosSpeech_ActivateInput")
                    PARAMETERS(new Prototype::Parameter<std::vector<String>>("strings"))
                BODY
                {
                    SETUP_FUNCTION(ActivateInput);

                    auto strings = selfFunc.GetParameter<std::vector<String>>("strings");
                    std::vector<String> copy(strings->obj);

                    Act::Speech::ActivateInput(std::move(copy));

                } BODY_END;

                FUNCTION(DeactivateInput, "AtmosSpeech_DeactivateInput")
                    NO_PARAMETERS
                BODY
                {
                    SETUP_FUNCTION(DeactivateInput);

                    Act::Speech::DeactivateInput();

                } BODY_END;

                FUNCTION(GetInputPosition, "AtmosSpeech_GetInputPosition")
                    NO_PARAMETERS
                BODY
                {
                    SETUP_FUNCTION(GetInputPosition);

                    vm->retval(Act::Speech::GetInputPosition());
                } BODY_END;

                FUNCTION(Leave, "AtmosSpeech_Leave")
                    NO_PARAMETERS
                BODY
                {
                    SETUP_FUNCTION(Leave);

                    Act::Speech::LeaveSpeech();
                } BODY_END;
            }

            namespace Shop
            {
                FUNCTION(Enter, "AtmosShop_Enter")
                    PARAMETERS(new Prototype::Parameter<bool>("buying"))
                BODY
                {
                    SETUP_FUNCTION(Enter);

                    auto buying = selfFunc.GetParameter<bool>("buying");

                    Act::Speech::EnterShop(**buying);
                } BODY_END;

                FUNCTION(Leave, "AtmosShop_Leave")
                    NO_PARAMETERS
                BODY
                {
                    SETUP_FUNCTION(Leave);

                    Act::Speech::LeaveShop();
                } BODY_END;

                FUNCTION(IsActive, "AtmosShop_IsActive")
                    NO_PARAMETERS
                BODY
                {
                    SETUP_FUNCTION(IsActive);

                    vm->retval(Act::Speech::IsShopActive());
                } BODY_END;
            }

            namespace Party
            {
                FUNCTION(Add, "AtmosParty_Add")
                    PARAMETERS(new Prototype::Parameter<::Atmos::Fal::Classes::Ent::Entity>("entity"))
                BODY
                {
                    SETUP_FUNCTION(Add);

                    auto entity = selfFunc.GetParameter<::Atmos::Fal::Classes::Ent::Entity>("entity");

                    Act::Ent::AddToPlayerParty((*entity)->value.Retrieve());
                } BODY_END;

                FUNCTION(IsEntityIn, "AtmosParty_IsEntityIn")
                    PARAMETERS(new Prototype::Parameter<::Atmos::Fal::Classes::Ent::Entity>("entity"))
                BODY
                {
                    SETUP_FUNCTION(IsEntityIn);

                    auto entity = selfFunc.GetParameter<::Atmos::Fal::Classes::Ent::Entity>("entity");

                    vm->retval(Act::Ent::IsEntityInParty((*entity)->value.Retrieve()));
                } BODY_END;
            }

            namespace Entity
            {
                FUNCTION(GetAvatar, "AtmosEntity_GetAvatar")
                    NO_PARAMETERS
                BODY
                {
                    SETUP_FUNCTION(GetAvatar);

                    auto got = ::Atmos::Ent::AvatarSystem::GetAvatar();
                    if (!got)
                        ::Atmos::Fal::Classes::Ent::Entity::Scaffolding().value.Set(::Atmos::Ent::nullEntity);
                    else
                        ::Atmos::Fal::Classes::Ent::Entity::Scaffolding().value.Set(got->GetOwnerEntity());

                    vm->retval(::Atmos::Fal::Classes::Ent::Entity::Scaffolding().CreateItem(*vm));
                } BODY_END;
            }
        }
    }
}