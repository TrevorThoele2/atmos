#pragma once

#include "AssetSystem.h"

#include "ShaderAsset.h"

#include "Serialization.h"

namespace Atmos
{
    class ShaderSystem : public AssetSystem<ShaderAsset>
    {
    public:
        ShaderSystem(ObjectManager& manager);
        ShaderSystem(const ::Inscription::Table<ShaderSystem>& table);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::ShaderSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}