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
        static const ObjectType objectType = ObjectType::Shard;
        static TypeName TypeName() { return "Atmos::Render::RenderCore"; }
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
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("material", object.material);
            archive("color", object.color);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Render::RenderCore, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::RenderCore>;
    };
}