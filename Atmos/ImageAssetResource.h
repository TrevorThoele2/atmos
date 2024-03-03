#pragma once

#include "ImageAssetSize.h"

namespace Atmos::Asset::Resource
{
    class Image
    {
    public:
        virtual ~Image() = 0;

        [[nodiscard]] virtual ImageSize Size() const = 0;
    protected:
        Image() = default;
    };
}