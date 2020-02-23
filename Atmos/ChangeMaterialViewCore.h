#pragma once

#include <Arca/Command.h>

#include "MaterialViewCore.h"

namespace Atmos::Render
{
    struct ChangeMaterialViewCore
    {
        Arca::RelicID id = 0;

        std::optional<Arca::Index<Asset::MaterialAsset>> material;
        std::optional<MaterialViewCore::Index> index;
        std::optional<Color> color;

        std::optional<Arca::Index<Asset::ShaderAsset>> patchShader;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ChangeMaterialViewCore>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "ChangeMaterialViewCore";
    };
}