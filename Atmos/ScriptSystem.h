#pragma once

#include "AssetSystem.h"

#include "ScriptAsset.h"

namespace Atmos
{
    class ScriptSystem : public AssetSystem<ScriptAsset>
    {
    public:
        ScriptSystem(ObjectManager& manager);
        ScriptSystem(const ::Inscription::Table<ScriptSystem>& table);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::ScriptSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}