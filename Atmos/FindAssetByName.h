#pragma once

#include <Arca/Command.h>
#include "String.h"
#include <Arca/Index.h>
#include <Arca/TypeFor.h>

namespace Atmos::Asset
{
    template<class T>
    struct FindByName
    {
        String name;

        explicit FindByName(const String& name) : name(name)
        {}
    };
}

namespace Arca
{
    template<class T>
    struct Traits<Atmos::Asset::FindByName<T>>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Asset::FindByName<" + TypeFor<T>().name + ">";
        using Result = Index<T>;
    };
}