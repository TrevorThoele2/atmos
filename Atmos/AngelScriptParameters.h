#pragma once

#include <angelscript.h>
#include <tuple>

#include "AngelScriptFunctionManagement.h"

#include <Chroma/FunctionTraits.h>
#include <Chroma/VariadicNonTypeTemplate.h>

namespace Atmos::Scripting::Angel
{
    namespace Detail
    {
        template<class T>
        struct SterilizedItem
        {
            using Type = std::remove_const_t<std::remove_reference_t<T>>;
        };

        template<auto ParameterProvider>
        struct ParameterProviderToParameterType
        {
            using Type = typename Chroma::FunctionTraits<decltype(ParameterProvider)>::ReturnT;
        };

        template<size_t i>
        struct PullValuesFromParametersIterator
        {
            template<class... Args>
            static void Do(std::tuple<Args...>& args, asIScriptGeneric& generic)
            {
                using ItemT = typename SterilizedItem<std::tuple_element_t<i, std::tuple<Args...>>>::Type;
                std::get<i>(args) = PullFromParameter<ItemT>(i, generic);
            }
        };

        template<size_t i>
        struct FromParameterProvidersIterator
        {
            template<class ParameterProviders, class... Args>
            static void Do(std::tuple<Args...>& args, asIScriptGeneric& generic, ParameterProviders)
            {
                using ParameterProvider = typename ParameterProviders::template Parameter<i>;
                auto parameterProvider = ParameterProvider::nonType;
                std::get<i>(args) = parameterProvider(generic);
            }
        };

        template<class T, Chroma::VariadicTemplateSize index>
        struct ToParameterProvidersIterator
        {
            static constexpr auto nonType = &PullFromParameter<index, T>;
        };
    }
}