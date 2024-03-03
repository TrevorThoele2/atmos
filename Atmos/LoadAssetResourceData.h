#pragma once

#include <Arca/Command.h>

#include <variant>
#include "FilePath.h"
#include "Buffer.h"

#include "AssetResourceTraits.h"

namespace Atmos::Asset::Resource
{
    template<class T>
    struct LoadData
    {
        std::variant<File::Path, Buffer> from;

        LoadData(const File::Path& from);
        LoadData(const Buffer& from);
    };

    template<class T>
    LoadData<T>::LoadData(const File::Path& from) : from(from)
    {}

    template<class T>
    LoadData<T>::LoadData(const Buffer& from) : from(from)
    {}

    template<class T>
    struct LoadedData;
}

namespace Arca
{
    template<class T>
    struct Traits<Atmos::Asset::Resource::LoadData<T>>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName =
            "Atmos::Asset::Resource::LoadData<" + Atmos::Asset::Resource::Traits<T>::ArcaTypeName() + ">";
        using Result = Atmos::Asset::Resource::LoadedData<T>;
    };
}