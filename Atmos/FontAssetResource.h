#pragma once

#include "AssetResourceTraits.h"

#include "Size2D.h"

namespace Atmos::Asset::Resource
{
    class Font
    {
    public:
        virtual ~Font() = 0;

        [[nodiscard]] virtual Spatial::Size2D Size(const String& string, bool bold, bool italics) const = 0;
    protected:
        Font() = default;
    };

    template<>
    struct Traits<Font>
    {
        static String ArcaTypeName() { return "Atmos::Asset::Resource::Font"; }
    };
}