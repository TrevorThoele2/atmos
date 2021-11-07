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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::ChangeImageCore";
    };
}