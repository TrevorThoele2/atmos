#pragma once

#include "ArcaShardIncludes.h"

#include "FontAsset.h"
#include "TextResource.h"

#include <Arca/Serialization.h>

namespace Atmos::Render
{
    class TextCore
    {
    public:
        String string;
        Arca::Index<Asset::Font> font;

        float wrapWidth = 0;

        bool bold = false;
        bool italics = false;
    public:
        using ResourceT = Resource::Text;
        using ResourcePtr = std::unique_ptr<ResourceT>;
        ResourcePtr resource;
    public:
        TextCore() = default;
        TextCore(const String& string, Arca::Index<Asset::Font> asset, float wrapWidth, bool bold, bool italics);

        bool operator==(const TextCore& arg) const;
        bool operator!=(const TextCore& arg) const;

        [[nodiscard]] ResourceT* Resource();
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::TextCore>
    {
        static constexpr ObjectType objectType = ObjectType::Shard;
        static const inline TypeName typeName = "Atmos::Render::TextCore";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::TextCore> final
    {
    public:
        using ObjectT = Atmos::Render::TextCore;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("asset", object.font);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Render::TextCore, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::TextCore>;
    };
}