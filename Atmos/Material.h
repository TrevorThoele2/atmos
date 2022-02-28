#pragma once

#include "ScriptAsset.h"

namespace Atmos::Render
{
    class Material
    {
    public:
        Arca::Index<Asset::Script> script;

        bool operator==(const Material& arg) const;
        bool operator!=(const Material& arg) const;
    };
}