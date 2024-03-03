#pragma once

#include <memory>

namespace Atmos::Asset
{
    class ScriptAssetData
    {
    public:
        ScriptAssetData() = default;

        virtual ~ScriptAssetData() = 0;

        [[nodiscard]] virtual std::unique_ptr<ScriptAssetData> Clone() const = 0;
    };
}