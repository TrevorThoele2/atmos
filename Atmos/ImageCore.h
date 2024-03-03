#pragma once

#include "ArcaShardIncludes.h"

#include "ImageAsset.h"

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
    public:
        ImageCore() = default;
        ImageCore(Arca::Index<Asset::Image> asset, Index assetIndex);

        bool operator==(const ImageCore& arg) const;
        bool operator!=(const ImageCore& arg) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ImageCore>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static TypeName TypeName() { return "Atmos::Render::ImageCore"; }
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
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Render::ImageCore, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::ImageCore>;
    };
}