#pragma once

#include "AssetResourceTraits.h"

namespace Atmos::Asset::Resource
{
    class Shader
    {
    public:
        virtual ~Shader() = 0;
    protected:
        Shader() = default;
    };

    template<>
    struct Traits<Shader>
    {
        static String ArcaTypeName() { return "Atmos::Asset::Resource::Shader"; }
    };
}