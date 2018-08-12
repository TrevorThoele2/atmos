#pragma once

#include <tuple>
#include <Chroma\VariadicTemplate.h>
#include <Chroma\Iterate.h>

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

        template<::Chroma::VariadicTemplateSize I>
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
            return ::Chroma::IterateRangeCheckStop<::Chroma::VariadicTemplateSize, MultipleComponentsValidity, ComponentBase*, sizeof...(Args) - 1, 0>(nullptr, check) != nullptr;
        }
    }
}