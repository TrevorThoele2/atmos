#pragma once

#include <Arca/Command.h>
#include "String.h"
#include <Arca/Index.h>
#include <Arca/TypeFor.h>

namespace Atmos::Asset
{
    template<class T>
    struct Find
    {
        String name;

        explicit Find(const String& name) : name(name)
        {}
    };
}

namespace Arca
{
    template<class T>
    struct Traits<Atmos::Asset::Find<T>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "FindAsset<" + TypeFor<T>().name + ">";
        using Result = Index<T>;
    };
}