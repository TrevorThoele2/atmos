#pragma once

#include "ArcaShardIncludes.h"

#include "FontAsset.h"

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
        TextCore() = default;
        TextCore(const String& string, Arca::Index<Asset::Font> asset, float wrapWidth, bool bold, bool italics);

        bool operator==(const TextCore& arg) const;
        bool operator!=(const TextCore& arg) const;
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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("asset", object.font);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Render::TextCore, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::TextCore>;
    };
}