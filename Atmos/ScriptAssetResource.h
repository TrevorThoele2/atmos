#pragma once

#include "AssetResourceTraits.h"

namespace Atmos::Asset::Resource
{
    class Script
    {
    public:
        virtual ~Script() = 0;
    protected:
        Script() = default;
    };

    template<>
    struct Traits<Script>
    {
        static String ArcaTypeName() { return "Atmos::Asset::Resource::Script"; }
    };
}