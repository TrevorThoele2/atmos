#pragma once

#include "AngelScriptUserData.h"
#include "AngelScriptFunctionManagement.h"
#include "AngelScriptParameters.h"
#include "AngelScriptName.h"

#include <Chroma/Iterate.h>
#include <Chroma/FunctionTraits.h>
#include <Chroma/VariadicNonTypeTemplate.h>

namespace Atmos::Scripting::Angel
{
    namespace Detail
    {
        template<class Object, class... Args>
        void DoGenerateObject(Object* memory, Args&& ... args)
        {
            new(memory) Object{ std::forward<Args>(args)... };
        }

        template<class Object, class Function, class... Args>
        void DoCallVoidMethod(Object& object, Function function, Args ... args)
        {
            (object.*function)(std::forward<Args>(args)...);
        }

        template<class Object, class Function, class... Args>
        decltype(auto) DoCallMethod(Object& object, Function function, Args ... args)
        {
            return (object.*function)(std::forward<Args>(args)...);
        }

        template<class Object, class... Args>
        struct CallGenerateObjectForwarder
        {
            static void Do(void* memory, std::tuple<Args...>&& args)
            {
                std::apply(Detail::DoGenerateObject<Object, Args...>, std::tuple_cat(std::forward_as_tuple(static_cast<Object*>(memory)), std::move(args)));
            }
        };

        template<class Object, class Function, class... Args>
        struct CallVoidMethodForwarder
        {
            static void Do(Object& object, Function function, const std::tuple<Args...>& args)
            {
                std::apply(Detail::DoCallVoidMethod<Function, Args...>, std::tuple_cat(std::forward_as_tuple(std::ref(object), function), args));
            }
        };

        template<class Object, class Function, class Return, class... Args>
        struct CallMethodForwarder
        {
            static Return Do(Object& object, Function function, const std::tuple<Args...>& args)
            {
                return std::apply(Detail::DoCallMethod<Function, Args...>, std::tuple_cat(std::forward_as_tuple(std::ref(object), function), args));
            }
        };

        template<size_t i>
        struct SkipFirstIterator
        {
            static inline const bool value = i != 0;
        };
    }

    template<class T>
    struct ObjectManagement
    {
        static void GenerateDefaultValue(asIScriptGeneric* generic);
        template<class... Args>
        static void GenerateValueBasic(asIScriptGeneric* generic);
        template<auto... parameterProviders>
        static void GenerateValue(asIScriptGeneric* generic);
        template<class... Args>
        static void GenerateKnownValue(asIScriptGeneric* generic, Args&& ... args);
        static void GenerateValueFromCopy(asIScriptGeneric* generic);

        static void DestructValue(asIScriptGeneric* generic);

        static void CopyAssign(asIScriptGeneric* generic);
        template<class From>
        static void Assign(asIScriptGeneric* generic);

        static void Equals(asIScriptGeneric* generic);
        static void Compare(asIScriptGeneric* generic);

        static void Add(asIScriptGeneric* generic);
        template<class Arg>
        static void Add(asIScriptGeneric* generic);
        static void Subtract(asIScriptGeneric* generic);
        template<class Arg>
        static void Subtract(asIScriptGeneric* generic);
        static void Multiply(asIScriptGeneric* generic);
        template<class Arg>
        static void Multiply(asIScriptGeneric* generic);
        static void Divide(asIScriptGeneric* generic);
        template<class Arg>
        static void Divide(asIScriptGeneric* generic);
        static void Modulo(asIScriptGeneric* generic);
        template<class Arg>
        static void Modulo(asIScriptGeneric* generic);
        static void CompoundAdd(asIScriptGeneric* generic);
        template<class Arg>
        static void CompoundAdd(asIScriptGeneric* generic);
        static void CompoundSubtract(asIScriptGeneric* generic);
        template<class Arg>
        static void CompoundSubtract(asIScriptGeneric* generic);
        static void CompoundMultiply(asIScriptGeneric* generic);
        template<class Arg>
        static void CompoundMultiply(asIScriptGeneric* generic);
        static void CompoundDivide(asIScriptGeneric* generic);
        template<class Arg>
        static void CompoundDivide(asIScriptGeneric* generic);
        static void CompoundModulo(asIScriptGeneric* generic);
        template<class Arg>
        static void CompoundModulo(asIScriptGeneric* generic);

        static void Negation(asIScriptGeneric* generic);
        static void PrefixIncrement(asIScriptGeneric* generic);
        static void PostfixIncrement(asIScriptGeneric* generic);
        static void PrefixDecrement(asIScriptGeneric* generic);
        static void PostfixDecrement(asIScriptGeneric* generic);

        template<class To>
        static void Cast(asIScriptGeneric* generic);

        template<auto function, auto... parameterProviders>
        static void Method(asIScriptGeneric* generic);
    public:
        static void RequiredSpecializationTemplateCallback(asIScriptGeneric* generic);

        static void UndefinedTemplate(asIScriptGeneric* generic);
    private:
        template<class Function>
        using FunctionTraits = Chroma::FunctionTraits<Function>;
        template<class Function>
        using FunctionArgs = typename FunctionTraits<Function>::ParameterPackT;
        template<class Function>
        using SkippedFunctionArgs = typename FunctionTraits<Function>::ParameterPackT::template filter_by_indices<Detail::SkipFirstIterator>;

        template<class Arguments>
        using BoundGenerateObjectForwarder = typename Arguments::
            template Prepend<T>::Type::
            template ForwardArguments<Detail::CallGenerateObjectForwarder>::Type;

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) == 0 &&
                std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                SkippedFunctionArgs<decltype(function)>::count == 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            const auto memory = static_cast<T*>(generic->GetObject());
            function(*memory);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) == 0 &&
                std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                SkippedFunctionArgs<decltype(function)>::count != 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            using FunctionArgs = SkippedFunctionArgs<decltype(function)>;

            auto args = FunctionArgs::TupleT();
            Chroma::IterateRange<size_t, Detail::PullValuesFromParametersIterator, FunctionArgs::count - 1>(args, *generic);

            const auto memory = static_cast<T*>(generic->GetObject());
            auto doCall = [memory](auto&& ... args)
            {
                function(*memory, std::forward<decltype(args)>(args)...);
            };
            std::apply(doCall, args);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) == 0 &&
                !std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                SkippedFunctionArgs<decltype(function)>::count == 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            const auto memory = static_cast<T*>(generic->GetObject());
            auto result = function(*memory);

            PushToReturn(result, *generic);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) == 0 &&
                !std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                SkippedFunctionArgs<decltype(function)>::count != 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            using FunctionArgs = SkippedFunctionArgs<decltype(function)>;

            auto args = FunctionArgs::TupleT();
            Chroma::IterateRange<size_t, Detail::PullValuesFromParametersIterator, FunctionArgs::count - 1>(args, *generic);

            const auto memory = static_cast<T*>(generic->GetObject());
            auto doCall = [memory](auto&& ... args)
            {
                return function(*memory, std::forward<decltype(args)>(args)...);
            };
            auto result = std::apply(doCall, args);

            PushToReturn(result, *generic);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) != 0 &&
                std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                SkippedFunctionArgs<decltype(function)>::count == 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            const auto memory = static_cast<T*>(generic->GetObject());
            function(*memory, function)();
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) != 0 &&
                std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                SkippedFunctionArgs<decltype(function)>::count != 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            using ArgumentsPack = Chroma::VariadicNonTypeTemplate<parameterProviders...>;
            using Arguments = typename ArgumentsPack
                ::template TransformToType<Detail::ParameterProviderToParameterType>::Type;

            auto args = Arguments::TupleT();
            Chroma::IterateRange<size_t, Detail::FromParameterProvidersIterator, sizeof...(parameterProviders) - 1>(args, *generic, ArgumentsPack{});

            const auto memory = static_cast<T*>(generic->GetObject());
            auto doCall = [memory](auto&& ... args)
            {
                function(*memory, std::forward<decltype(args)>(args)...);
            };
            std::apply(doCall, args);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) != 0 &&
                !std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                SkippedFunctionArgs<decltype(function)>::count == 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            const auto memory = static_cast<T*>(generic->GetObject());
            auto result = function(*memory);

            PushToReturn(result, *generic);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) != 0 &&
                !std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                SkippedFunctionArgs<decltype(function)>::count != 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            using ArgumentsPack = Chroma::VariadicNonTypeTemplate<parameterProviders...>;
            using Arguments = typename ArgumentsPack
                ::template TransformToType<Detail::ParameterProviderToParameterType>::Type;

            auto args = Arguments::TupleT();
            Chroma::IterateRange<size_t, Detail::FromParameterProvidersIterator, sizeof...(parameterProviders) - 1>(args, *generic, ArgumentsPack{});

            const auto memory = static_cast<T*>(generic->GetObject());
            auto doCall = [memory](auto&& ... args)
            {
                return function(*memory, std::forward<decltype(args)>(args)...);
            };
            auto result = std::apply(doCall, args);

            PushToReturn(result, *generic);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                !Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) == 0 &&
                std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count == 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            const auto memory = static_cast<T*>(generic->GetObject());
            (memory->*function)();
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                !Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) == 0 &&
                std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count != 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            using FunctionArgs = FunctionArgs<decltype(function)>;

            auto args = FunctionArgs::TupleT();
            Chroma::IterateRange<size_t, Detail::PullValuesFromParametersIterator, FunctionArgs::count - 1>(args, *generic);

            const auto memory = static_cast<T*>(generic->GetObject());
            auto doCall = [memory](auto&& ... args)
            {
                (memory->*function)(std::forward<decltype(args)>(args)...);
            };
            std::apply(doCall, args);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                !Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) == 0 &&
                !std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count == 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            const auto memory = static_cast<T*>(generic->GetObject());
            auto result = (memory->*function)();

            PushToReturn(result, *generic);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                !Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) == 0 &&
                !std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count != 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            using FunctionArgs = FunctionArgs<decltype(function)>;

            auto args = FunctionArgs::TupleT();
            Chroma::IterateRange<size_t, Detail::PullValuesFromParametersIterator, FunctionArgs::count - 1>(args, *generic);

            const auto memory = static_cast<T*>(generic->GetObject());
            auto doCall = [memory](auto&& ... args)
            {
                return (memory->*function)(std::forward<decltype(args)>(args)...);
            };
            auto result = std::apply(doCall, args);

            PushToReturn(result, *generic);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                !Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) != 0 &&
                std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count == 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            const auto memory = static_cast<T*>(generic->GetObject());
            (memory->*function)();
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                !Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) != 0 &&
                std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count != 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            using ArgumentsPack = Chroma::VariadicNonTypeTemplate<parameterProviders...>;
            using Arguments = typename ArgumentsPack
                ::template TransformToType<Detail::ParameterProviderToParameterType>::Type;

            auto args = Arguments::TupleT();
            Chroma::IterateRange<size_t, Detail::FromParameterProvidersIterator, sizeof...(parameterProviders) - 1>(args, *generic, ArgumentsPack{});

            const auto memory = static_cast<T*>(generic->GetObject());
            auto doCall = [memory](auto&& ... args)
            {
                (memory->*function)(std::forward<decltype(args)>(args)...);
            };
            std::apply(doCall, args);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                !Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) != 0 &&
                !std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count == 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            const auto memory = static_cast<T*>(generic->GetObject());
            auto result = (memory->*function)();

            PushToReturn(result, *generic);
        }

        template<
            auto function,
            auto... parameterProviders,
            std::enable_if_t<
                !Chroma::FunctionTraits<decltype(function)>::isFunction &&
                sizeof...(parameterProviders) != 0 &&
                !std::is_same_v<typename Chroma::FunctionTraits<decltype(function)>::ReturnT, void> &&
                FunctionArgs<decltype(function)>::count != 0, int> = 0>
        static void HandleMethod(asIScriptGeneric* generic)
        {
            using ArgumentsPack = Chroma::VariadicNonTypeTemplate<parameterProviders...>;
            using Arguments = typename ArgumentsPack
                ::template TransformToType<Detail::ParameterProviderToParameterType>::Type;

            auto args = Arguments::TupleT();
            Chroma::IterateRange<size_t, Detail::FromParameterProvidersIterator, sizeof...(parameterProviders) - 1>(args, *generic, ArgumentsPack{});

            const auto memory = static_cast<T*>(generic->GetObject());
            auto doCall = [memory](auto&& ... args)
            {
                return (memory->*function)(std::forward<decltype(args)>(args)...);
            };
            auto result = std::apply(doCall, args);

            PushToReturn(result, *generic);
        }
    };

    template<class T>
    void ObjectManagement<T>::GenerateDefaultValue(asIScriptGeneric* generic)
    {
        const auto self = static_cast<T*>(generic->GetObject());
        Detail::DoGenerateObject(self);
    }

    template<class T>
    template<class... Args>
    void ObjectManagement<T>::GenerateValueBasic(asIScriptGeneric* generic)
    {
        auto args = std::tuple<Args...>();
        Chroma::IterateRange<size_t, Detail::PullValuesFromParametersIterator, sizeof...(Args) - 1>(args, *generic);

        const auto self = static_cast<T*>(generic->GetObject());
        std::apply(Detail::DoGenerateObject<T, Args...>, std::tuple_cat(std::forward_as_tuple(self), args));
    }

    template<class T>
    template<auto... parameterProviders>
    void ObjectManagement<T>::GenerateValue(asIScriptGeneric* generic)
    {
        using ArgumentsPack = Chroma::VariadicNonTypeTemplate<parameterProviders...>;
        using Arguments = typename ArgumentsPack
            ::template TransformToType<Detail::ParameterProviderToParameterType>::Type;

        auto args = Arguments::TupleT();
        if constexpr (sizeof...(parameterProviders) > 0)
            Chroma::IterateRange<size_t, Detail::FromParameterProvidersIterator, sizeof...(parameterProviders) - 1>(args, *generic, ArgumentsPack{});

        const auto self = static_cast<T*>(generic->GetObject());
        BoundGenerateObjectForwarder<Arguments>::Do(self, std::move(args));
    }

    template<class T>
    template<class... Args>
    void ObjectManagement<T>::GenerateKnownValue(asIScriptGeneric* generic, Args&& ... args)
    {
        const auto self = static_cast<T*>(generic->GetObject());
        Detail::DoGenerateObject(self, std::forward<Args>(args)...);
    }

    template<class T>
    void ObjectManagement<T>::GenerateValueFromCopy(asIScriptGeneric* generic)
    {
        auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        new (self) T(*arg);
    }

    template<class T>
    void ObjectManagement<T>::DestructValue(asIScriptGeneric* generic)
    {
        auto self = static_cast<T*>(generic->GetObject());
        self->~T();
    }

    template<class T>
    void ObjectManagement<T>::CopyAssign(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        *self = *arg;
        PushToReturn(self, *generic);
    }

    template<class T>
    template<class From>
    void ObjectManagement<T>::Assign(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<From*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        *self = *arg;
        PushToReturn(self, *generic);
    }

    template<class T>
    void ObjectManagement<T>::Equals(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        const auto result = *self == *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::Compare(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());

        if (*arg == *self)
            PushToReturn(0, *generic);
        else if (*arg < *self)
            PushToReturn(1, *generic);
        else
            PushToReturn(-1, *generic);
    }

    template<class T>
    void ObjectManagement<T>::Add(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self + *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    template<class Arg>
    void ObjectManagement<T>::Add(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<Arg*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self + *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::Subtract(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self - *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    template<class Arg>
    void ObjectManagement<T>::Subtract(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<Arg*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self - *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::Multiply(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = (*self) * (*arg);
        PushToReturn(result, *generic);
    }

    template<class T>
    template<class Arg>
    void ObjectManagement<T>::Multiply(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<Arg*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = (*self) * (*arg);
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::Divide(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self / *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    template<class Arg>
    void ObjectManagement<T>::Divide(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<Arg*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self / *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::Modulo(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self % *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    template<class Arg>
    void ObjectManagement<T>::Modulo(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<Arg*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self % *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::CompoundAdd(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self += *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    template<class Arg>
    void ObjectManagement<T>::CompoundAdd(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<Arg*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self += *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::CompoundSubtract(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self -= *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    template<class Arg>
    void ObjectManagement<T>::CompoundSubtract(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<Arg*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self -= *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::CompoundMultiply(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self *= *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    template<class Arg>
    void ObjectManagement<T>::CompoundMultiply(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<Arg*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self *= *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::CompoundDivide(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self /= *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    template<class Arg>
    void ObjectManagement<T>::CompoundDivide(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<Arg*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self /= *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::CompoundModulo(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<T*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self %= *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    template<class Arg>
    void ObjectManagement<T>::CompoundModulo(asIScriptGeneric* generic)
    {
        const auto arg = static_cast<Arg*>(generic->GetArgObject(0));
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = *self %= *arg;
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::Negation(asIScriptGeneric* generic)
    {
        const auto self = static_cast<T*>(generic->GetObject());
        auto result = -(*self);
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::PrefixIncrement(asIScriptGeneric* generic)
    {
        auto self = static_cast<T*>(generic->GetObject());
        auto result = ++(*self);
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::PostfixIncrement(asIScriptGeneric* generic)
    {
        auto self = static_cast<T*>(generic->GetObject());
        auto result = (*self)++;
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::PrefixDecrement(asIScriptGeneric* generic)
    {
        auto self = static_cast<T*>(generic->GetObject());
        auto result = --(*self);
        PushToReturn(result, *generic);
    }

    template<class T>
    void ObjectManagement<T>::PostfixDecrement(asIScriptGeneric* generic)
    {
        auto self = static_cast<T*>(generic->GetObject());
        auto result = (*self)--;
        PushToReturn(result, *generic);
    }

    template<class T>
    template<class To>
    void ObjectManagement<T>::Cast(asIScriptGeneric* generic)
    {
        const auto self = static_cast<T*>(generic->GetObject());
        const auto casted = static_cast<To>(*self);
        PushToReturn(casted, *generic);
    }

    template<class T>
    template<auto function, auto... parameterProviders>
    void ObjectManagement<T>::Method(asIScriptGeneric* generic)
    {
        HandleMethod<function, parameterProviders...>(generic);
    }

    template<class T>
    void ObjectManagement<T>::RequiredSpecializationTemplateCallback(asIScriptGeneric* generic)
    {
        const auto typeInfo = PullFromParameter<0, asITypeInfo*>(*generic);
        const auto subTypeID = typeInfo->GetSubTypeId();
        const auto subTypeInfo = generic->GetEngine()->GetTypeInfoById(subTypeID);
        const auto subTypeName = CreateName({ subTypeInfo->GetNamespace() }, subTypeInfo->GetName());

        auto userData = UserData::RequiredFrom(*generic);

        const auto ableToCreate = userData->registeredTypes.find(subTypeName) != userData->registeredTypes.end();

        PushToReturn(ableToCreate, *generic);
    }

    template<class T>
    void ObjectManagement<T>::UndefinedTemplate(asIScriptGeneric* generic)
    {
        throw Error("The general template for this type is undefined.");
    }
}