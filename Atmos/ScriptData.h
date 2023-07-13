#pragma once

#include "ScriptAsset.h"
#include "ScriptParameters.h"
#include "ScriptResource.h"

namespace Atmos::Scripting
{
    struct ScriptData
    {
        Arca::Index<Asset::Script> asset;
        String executeName;
        Parameters parameters;
        std::unique_ptr<Resource> resource;
        bool isSuspended = false;

        ScriptData() = default;
        ScriptData(const Arca::Index<Asset::Script>& asset, const String& executeName, const Parameters& parameters);
        ScriptData(ScriptData&& arg) noexcept;
        ScriptData& operator=(ScriptData&& arg) noexcept;

        bool operator==(const ScriptData& arg) const;
        bool operator!=(const ScriptData& arg) const;
    };
}