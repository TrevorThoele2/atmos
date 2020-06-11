#pragma once

namespace Atmos::Asset
{
    class ImageData
    {
    public:
        virtual ~ImageData() = 0;
    protected:
        ImageData() = default;
    };
}