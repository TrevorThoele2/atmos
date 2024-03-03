#pragma once

#include "AssetResourceTraits.h"

#include "LoadedGlyph.h"
#include "Size2D.h"

namespace Atmos::Asset::Resource
{
    class Font
    {
    public:
        virtual ~Font() = 0;

        [[nodiscard]] virtual Render::LoadedGlyph Load(char32_t codepoint, bool bold, bool italics) const = 0;
        [[nodiscard]] virtual float MaximumGlyphHeight() const = 0;
    protected:
        Font() = default;
    };

    template<>
    struct Traits<Font>
    {
        static String ArcaTypeName() { return "Atmos::Asset::Resource::Font"; }
    };
}