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
        static constexpr ObjectType objectType = ObjectType::Shard;
        static const inline TypeName typeName = "Atmos::Render::ImageCore";
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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("asset", object.asset);
            format("assetIndex", object.assetIndex);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Render::ImageCore, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::ImageCore>;
    };
}