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
        static inline const TypeName typeName = "Atmos::Render::ImageCore";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::ImageCore> final
    {
    public:
        using ObjectT = Atmos::Render::ImageCore;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("asset", object.asset);
            archive("assetIndex", object.assetIndex);
            archive("material", object.material);
            archive("color", object.color);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Render::ImageCore, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::ImageCore>;
    };
}