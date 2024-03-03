#pragma once

#include <Arca/Command.h>

#include <Arca/RelicID.h>
#include "String.h"
#include "FontAsset.h"

namespace Atmos::Render
{
    struct ChangeTextCore
    {
        Arca::RelicID id = 0;

        std::optional<String> string;
        std::optional<Arca::Index<Asset::Font>> font;
        std::optional<float> wrapWidth;
        std::optional<bool> bold;
        std::optional<bool> italics;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ChangeTextCore>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Render::ChangeTextCore"; }
    };
}