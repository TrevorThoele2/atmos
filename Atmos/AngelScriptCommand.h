#pragma once

#include <angelscript.h>

#include "AngelScriptRegistration.h"
#include "AngelScriptGlobalRegistration.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptFunctionTraits.h"
#include "AngelScriptPrimitive.h"

#include <Arca/Reliquary.h>

namespace Atmos::Scripting::Angel
{
    namespace Detail
    {
        template<auto toArca>
        void OnCommand(asIScriptGeneric* generic)
        {
            using TraitsT = FunctionTraits<toArca>;
            using Argument = typename SterilizedItem<typename TraitsT::ParameterPackT::template Parameter<0>::Type>::Type;
            using FromArca = typename TraitsT::ReturnT;

            auto fromAngelScript = PullFromParameter<0, Argument>(*generic);

            const FromArca fromArca = toArca(std::move(fromAngelScript));

            auto reliquary = UserData::RequiredReliquaryFrom(*generic);
            reliquary->Do(fromArca);
        }

        template<auto toArca, auto toReturn>
        void OnCommand(asIScriptGeneric* generic)
        {
            using TraitsT = FunctionTraits<toArca>;
            using Argument = typename SterilizedItem<typename TraitsT::ParameterPackT::template Parameter<0>::Type>::Type;
            using FromArca = typename TraitsT::ReturnT;

            auto fromAngelScript = PullFromParameter<0, Argument>(*generic);

            const FromArca fromArca = toArca(std::move(fromAngelScript));

            auto reliquary = UserData::RequiredReliquaryFrom(*generic);
            auto result = reliquary->Do(fromArca);

            const auto fromResult = toReturn(std::move(result), *reliquary);

            PushToReturn(fromResult, *generic);
        }

        template<auto toReturn>
        void OnCommandStub(asIScriptGeneric* generic)
        {
            using TraitsT = FunctionTraits<toReturn>;
            using Argument = typename SterilizedItem<typename TraitsT::ParameterPackT::template Parameter<0>::Type>::Type;
            using FromArca = typename TraitsT::ReturnT;

            const auto fromAngelScript = PullFromParameter<0, Argument>(*generic);

            const auto reliquary = UserData::RequiredReliquaryFrom(*generic);
            const auto fromResult = toReturn(fromAngelScript, *reliquary);

            PushToReturn(fromResult, *generic);
        }

        template<auto function, class ParametersT, class ReturnT>
        void RegisterCommandHandlerCommon(asIScriptEngine& engine, DocumentationManager& documentationManager)
        {
            static_assert(ParametersT::count == 1, "toArca must take a single parameter.");

            using Argument = typename SterilizedItem<typename ParametersT::template Parameter<0>::Type>::Type;
            using ArgumentTypeRegistration = Registration<Argument>;
            const auto commandClassName = CreateName({ ArgumentTypeRegistration::containingNamespace }, ArgumentTypeRegistration::name);

            using ReturnTypeRegistration = Registration<std::remove_pointer_t<ReturnT>>;
            const auto returnNameToken = CreateName({ ReturnTypeRegistration::containingNamespace }, ReturnTypeRegistration::name);
            const auto returnName = ReturnTypeRegistration::objectType == ObjectType::Handle
                ? returnNameToken + "@"
                : returnNameToken;

            GlobalRegistration(CreateName({ "Arca" }, "Reliquary"))
                .Function(function, returnName, "Do", { commandClassName + " command" })
                .Actualize(engine, documentationManager);
        }
    }

    template<auto toArca>
    void RegisterCommandHandler(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        using ParameterPackT = typename FunctionTraits<toArca>::ParameterPackT;
        Detail::RegisterCommandHandlerCommon<&Detail::OnCommand<toArca>, ParameterPackT, void>(
            engine, documentationManager);
    }

    template<auto toArca, auto toReturn>
    void RegisterCommandHandler(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        using ParameterPackT = typename FunctionTraits<toArca>::ParameterPackT;
        using ReturnT = typename FunctionTraits<toReturn>::ReturnT;
        Detail::RegisterCommandHandlerCommon<&Detail::OnCommand<toArca, toReturn>, ParameterPackT, ReturnT>(
            engine, documentationManager);
    }

    template<auto toReturn, class ArgumentT>
    void RegisterCommandHandlerStub(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        using ParameterPackT = Chroma::ParameterPack<ArgumentT>;
        using ReturnT = typename FunctionTraits<toReturn>::ReturnT;
        Detail::RegisterCommandHandlerCommon<&Detail::OnCommandStub<toReturn>, ParameterPackT, ReturnT>(
            engine, documentationManager);
    }
}