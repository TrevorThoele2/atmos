#pragma once

#include <Arca/Command.h>

#include "ImageCore.h"

namespace Atmos::Render
{
    struct ChangeImageCore
    {
        Arca::RelicID id = 0;

        std::optional<Arca::Index<Asset::Image>> asset;
        std::optional<ImageCore::Index> assetIndex;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ChangeImageCore>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Render::ChangeImageCore";
    };
}