#pragma once

#include "ArcaShardIncludes.h"

#include <Arca/RelicIndex.h>
#include "MaterialAsset.h"
#include "Color.h"

namespace Atmos::Render
{
    struct RenderCore
    {
        Arca::Index<Asset::Material> material;
        Color color;

        RenderCore() = default;
        RenderCore(Arca::Index<Asset::Material> material, const Color& color);

        bool operator==(const RenderCore& arg) const;
        bool operator!=(const RenderCore& arg) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::RenderCore>
    {
        static constexpr ObjectType objectType = ObjectType::Shard;
        static const inline TypeName typeName = "Atmos::Render::RenderCore";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::RenderCore> final
    {
    public:
        using ObjectT = Atmos::Render::RenderCore;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("material", object.material);
            format("color", object.color);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Render::RenderCore, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::RenderCore>;
    };
}