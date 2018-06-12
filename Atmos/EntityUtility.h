#pragma once

#include <tuple>
#include <Function\VariadicTemplate.h>
#include <Function\Iterate.h>

namespace Atmos
{
    namespace Ent
    {
        class ComponentBase;

        template<class T>
        struct MultipleComponentTransform
        {
            typedef T* Type;
        };

        template<class T>
        struct MultipleComponentTransform<T*>
        {
            typedef T Type;
        };

        template<class... Args>
        using MultipleComponents = std::tuple<Args...>;

        template<function::VariadicTemplateSize I>
        struct MultipleComponentsValidity
        {
            template<class... Args>
            static ComponentBase* Check(const MultipleComponents<Args...> &multipleComponents)
            {
                return std::get<I>(multipleComponents);
            }
        };

        template<class... Args>
        bool AllComponentsValid(const MultipleComponents<Args...> &check)
        {
            return function::IterateRangeCheckStop<function::VariadicTemplateSize, MultipleComponentsValidity, ComponentBase*, sizeof...(Args) - 1, 0>(nullptr, check) != nullptr;
        }
    }
}