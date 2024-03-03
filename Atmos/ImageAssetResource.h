#pragma once

namespace Atmos::Asset::Resource
{
    class Image
    {
    public:
        virtual ~Image() = 0;
    protected:
        Image() = default;
    };
}