#pragma once

#include "AngelScriptFunctionManagement.h"
#include "AngelScriptParameters.h"

#include <Chroma/Iterate.h>
#include <Chroma/FunctionTraits.h>

namespace Atmos::Scripting::Angel
{
    struct GlobalManagement
    {
        template<auto function, auto... parameterProviders>
        static void Function(asIScriptGeneric* generic);
    private:
        template<class Function>
        using FunctionArgs = typename Chroma::FunctionTraits<Function>::ParameterPackT::
            template Transform<Detail::SterilizedItem>::Type;

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                sizeof...(parameterProviders) == 0 &&
                std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count == 0, int> = 0>
        static void HandleFunction(asIScriptGeneric* generic)
        {
            function();
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                sizeof...(parameterProviders) == 0 &&
                std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count != 0, int> = 0>
        static void HandleFunction(asIScriptGeneric* generic)
        {
            using FunctionArgs = FunctionArgs<decltype(function)>;

            auto args = FunctionArgs::TupleT();
            Chroma::IterateRange<size_t, Detail::PullValuesFromParametersIterator, FunctionArgs::count - 1>(args, *generic);

            auto doCall = [](auto&& ... args)
            {
                function( std::forward<decltype(args)>(args)...);
            };
            std::apply(doCall, args);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                sizeof...(parameterProviders) == 0 &&
                !std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count == 0, int> = 0>
        static void HandleFunction(asIScriptGeneric* generic)
        {
            auto result = function();

            PushToReturn(result, *generic);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                sizeof...(parameterProviders) == 0 &&
                !std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count != 0, int> = 0>
        static void HandleFunction(asIScriptGeneric* generic)
        {
            using FunctionArgs = FunctionArgs<decltype(function)>;

            auto args = FunctionArgs::TupleT();
            Chroma::IterateRange<size_t, Detail::PullValuesFromParametersIterator, FunctionArgs::count - 1>(args, *generic);

            auto doCall = [](auto&& ... args)
            {
                return function(std::forward<decltype(args)>(args)...);
            };
            auto result = std::apply(doCall, args);

            PushToReturn(result, *generic);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                sizeof...(parameterProviders) != 0 &&
                std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count == 0, int> = 0>
        static void HandleFunction(asIScriptGeneric* generic)
        {
            function();
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                sizeof...(parameterProviders) != 0 &&
                std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count != 0, int> = 0>
        static void HandleFunction(asIScriptGeneric* generic)
        {
            using ArgumentsPack = Chroma::VariadicNonTypeTemplate<parameterProviders...>;
            using Arguments = typename ArgumentsPack
                ::template TransformToType<Detail::ParameterProviderToParameterType>::Type;

            auto args = Arguments::TupleT();
            Chroma::IterateRange<size_t, Detail::FromParameterProvidersIterator, sizeof...(parameterProviders) - 1>(args, *generic, ArgumentsPack{});

            auto doCall = [](auto&& ... args)
            {
                function(std::forward<decltype(args)>(args)...);
            };
            std::apply(doCall, args);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                sizeof...(parameterProviders) != 0 &&
                !std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count == 0, int> = 0>
        static void HandleFunction(asIScriptGeneric* generic)
        {
            auto result = function();

            PushToReturn(result, *generic);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                sizeof...(parameterProviders) != 0 &&
                !std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count != 0, int> = 0>
        static void HandleFunction(asIScriptGeneric* generic)
        {
            using ArgumentsPack = Chroma::VariadicNonTypeTemplate<parameterProviders...>;
            using Arguments = typename ArgumentsPack
                ::template TransformToType<Detail::ParameterProviderToParameterType>::Type;

            auto args = Arguments::TupleT();
            Chroma::IterateRange<size_t, Detail::FromParameterProvidersIterator, sizeof...(parameterProviders) - 1>(args, *generic, ArgumentsPack{});

            auto doCall = [](auto&& ... args)
            {
                return function(std::forward<decltype(args)>(args)...);
            };
            auto result = std::apply(doCall, args);

            PushToReturn(result, *generic);
        }
    };

    template<auto function, auto... parameterProviders>
    void GlobalManagement::Function(asIScriptGeneric* generic)
    {
        HandleFunction<function, parameterProviders...>(generic);
    }
}