#pragma once

#include "AssetResourceTraits.h"
#include "Size2D.h"

namespace Atmos::Asset::Resource
{
    class Image
    {
    public:
        virtual ~Image() = 0;

        [[nodiscard]] virtual Spatial::Size2D Size() const = 0;
    protected:
        Image() = default;
    };

    template<>
    struct Traits<Image>
    {
        static String ArcaTypeName() { return "Atmos::Asset::Resource::Image"; }
    };
}