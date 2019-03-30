
#pragma once

#include "AssetSystem.h"

#include "ImageAsset.h"

#include "Serialization.h"

namespace Atmos
{
    class ImageSystem : public AssetSystem<ImageAsset>
    {
    public:
        ImageSystem(ObjectManager& manager);
        ImageSystem(const ::Inscription::Table<ImageSystem>& table);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::ImageSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}