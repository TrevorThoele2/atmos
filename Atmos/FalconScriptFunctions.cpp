
#include "FalconScriptFunctions.h"
#include "FalconScriptObjects.h"

#include "CurrentField.h"
#include "ModulatorObserver.h"
#include "GameEnvironment.h"
#include "ActionFunctions.h"

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
                // Suspend
                FUNCTION(Suspend, "Atmos_Suspend")
                    NO_PARAMETERS
                BODY
                {
                    ScriptController::Current()->Suspend();
                } BODY_END;

                // Log message
                FUNCTION(LogMessage, "Atmos_LogMessage")
                    PARAMETERS(new Prototype::Parameter<String>("message"), new Prototype::Parameter<Logger::Type>("type", false))
                BODY
                {
                    SETUP_FUNCTION(LogMessage);

                    auto message = selfFunc.GetParameter<String>("message");
                    auto type = selfFunc.GetParameter<Logger::Type>("type");
                    
                    if (!type->WasSet())
                        Logger::Log(message->value, Logger::Type::INFORMATION);
                    else
                        Logger::Log(message->value, type->value);
                } BODY_END;

                // Random bool
                FUNCTION(RandomBool, "Atmos_RandomBool")
                    PARAMETERS(new Prototype::Parameter<double>("probability", false))
                BODY
                {
                    SETUP_FUNCTION_CONTINUE(RandomBool);

                    auto probability = selfFunc.GetParameter<double>("probability");
                    if (!probability->WasSet())
                        vm->retval(Act::Random::GenerateBool());
                    else
                        vm->retval(Act::Random::GenerateBool(probability->value));
                } BODY_END;

                // Random integer
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
                        vm->retval(Act::Random::GenerateInt<T>(floor->value, ceiling->value));
                } BODY_END;
            }

            namespace Modulator
            {
                // Create modulator
                FUNCTION(Create, "AtmosModulator_Create")
                    PARAMETERS(new Prototype::Parameter<Name>("name"))
                BODY
                {
                    SETUP_FUNCTION(Create);
                    auto name = selfFunc.GetParameter<Name>("name");

                    // Create the modulator
                    ::Atmos::Modulator::Observer madeMod(GameEnvironment::GenerateModulator(name->value));
                    if (!madeMod)
                    {
                        // Log error if not created
                        Logger::Log("A modulator was attempted to be created with an invalid name.",
                            Logger::Type::ERROR_LOW,
                            Logger::NameValueVector{ NameValuePair("Name", name->value) });
                        return;
                    }

                    // Return the ID for the new modulator
                    Classes::Modulator::Modulator::modID.value = GameEnvironment::AttachModulator(madeMod);
                    vm->retval(Classes::Modulator::Modulator::CreateItem(*vm));
                } BODY_END;
            }
        }
    }
}