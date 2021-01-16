#pragma once

#include "AssetResourceTraits.h"

namespace Atmos::Asset::Resource
{
    class Audio
    {
    public:
        virtual ~Audio() = 0;
    protected:
        Audio() = default;
    };

    template<>
    struct Traits<Audio>
    {
        static String ArcaTypeName() { return "Atmos::Asset::Resource::Audio"; }
    };
}