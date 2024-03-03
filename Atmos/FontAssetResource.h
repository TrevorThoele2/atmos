#pragma once

#include "AssetResourceTraits.h"

namespace Atmos::Asset::Resource
{
    class Font
    {
    public:
        virtual ~Font() = 0;
    protected:
        Font() = default;
    };

    template<>
    struct Traits<Font>
    {
        static String ArcaTypeName() { return "Atmos::Asset::Resource::Font"; }
    };
}