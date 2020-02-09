#pragma once

#include "ScriptAssetData.h"

#include "String.h"

namespace Atmos::Script
{
    class Manager
    {
    public:
        virtual ~Manager() = 0;

        [[nodiscard]] virtual std::unique_ptr<Asset::ScriptAssetData> CreateAssetData(String name, String fileData) = 0;
    };
}