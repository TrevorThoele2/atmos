#pragma once

#include <Arca/Shard.h>

#include "MaterialAsset.h"
#include "Color.h"
#include "ShaderAsset.h"
#include "AxisAlignedBox2D.h"

#include <Arca/Serialization.h>

namespace Atmos::Render
{
    class ImageCore
    {
    public:
        using Index = int;
    public:
        Arca::Index<Asset::Image> asset;
        Index assetIndex = 0;
        Arca::Index<Asset::Material> material;

        Color color;
    public:
        ImageCore() = default;
        ImageCore(
            Arca::Index<Asset::Image> asset,
            Index assetIndex,
            Arca::Index<Asset::Material> material,
            Color color);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ImageCore>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static inline const TypeName typeName = "ImageCore";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::ImageCore, BinaryArchive> final
        : public ArcaCompositeScribe<Atmos::Render::ImageCore, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}