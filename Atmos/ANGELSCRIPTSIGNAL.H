#pragma once

#include <angelscript.h>

#include "Script.h"

#include "AngelScriptScriptResource.h"
#include "AngelScriptRegistration.h"
#include "AngelScriptGlobalRegistration.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptArcaUserData.h"
#include "AngelScriptFunctionTraits.h"

#include <Arca/Reliquary.h>

#include <Chroma/FunctionTraits.h>

namespace Atmos::Scripting::Angel
{
    namespace Detail
    {
        Arca::Index<Script> RetrieveCurrentExecuting(Arca::Reliquary& reliquary);

        template<class ArgumentT>
        void RegisterOnFunction(asIScriptGeneric* generic)
        {
            const auto function = reinterpret_cast<asIScriptFunction*>(generic->GetArgAddress(0));

            const auto currentExecutingScript = RetrieveCurrentExecuting(*UserData::RequiredReliquaryFrom(*generic));
            if (!currentExecutingScript)
                return;

            const auto currentExecutingScriptResource = currentExecutingScript->Resource<ScriptResource>();
            if (!currentExecutingScriptResource)
                return;

            const auto functionGroupName = Arca::TypeFor<ArgumentT>().name;
            currentExecutingScriptResource->StoreFunction(functionGroupName, *function);
        }

        template<auto fromArca>
        void RegisterSignalHandlerCommon(asIScriptEngine& engine)
        {
            using ParametersT = typename FunctionTraits<fromArca>::ParameterPackT;
            static_assert(ParametersT::count == 1, "fromArca must take a single parameter.");

            using Argument = typename SterilizedItem<typename ParametersT::template Parameter<0>::Type>::Type;
            using ArgumentTypeRegistration = Registration<Argument>;
            const auto funcDef =
                Chroma::ReplaceString(
                    Chroma::ReplaceString(
                        Chroma::ReplaceString("On" + ArgumentTypeRegistration::name, "<", ""), ">", ""), "::", "");
            GlobalRegistration(Namespaces::Arca::name)
                .Funcdef("void", funcDef, { CreateName({ArgumentTypeRegistration::containingNamespace}, ArgumentTypeRegistration::name) + " signal" })
                .Actualize(engine);
            GlobalRegistration(CreateName({ Namespaces::Arca::name }, "Reliquary"))
                .Function(&RegisterOnFunction<Argument>, "void", "On", std::vector<String>{ funcDef + "@ handler" })
                .Actualize(engine);

            UserData::RequiredFrom(engine)->reliquary->On<Argument>([&engine](const Argument& argument)
                {
                    const auto reliquary = UserData::RequiredFrom(engine)->reliquary;
                    const auto currentRunningScript = RetrieveCurrentExecuting(*reliquary);
                    if (!currentRunningScript)
                        return;

                    auto resource = currentRunningScript->Resource<ScriptResource>();
                    if (!resource)
                        return;

                    const auto toAngelScript = fromArca(argument);

                    const auto functionGroupName = Arca::TypeFor<Argument>().name;
                    const auto prepareContext = [toAngelScript](asIScriptContext& context)
                    {
                        PushToParameter(0, toAngelScript, context);
                    };

                    resource->ExecuteSecondaryAgainstStoredFunctions(functionGroupName, prepareContext);
                });
        }
    }

    template<auto fromArca>
    void RegisterSignalHandler(asIScriptEngine& engine)
    {
        Detail::RegisterSignalHandlerCommon<fromArca>(engine);
    }
}