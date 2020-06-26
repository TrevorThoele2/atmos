#pragma once

#include <memory>
#include "ScriptAssetResource.h"

#include "String.h"

namespace Atmos::Script
{
    class Manager
    {
    public:
        virtual ~Manager() = 0;

        [[nodiscard]] virtual std::unique_ptr<Asset::Resource::Script> CreateAssetResource(String name, String fileData) = 0;
    };
}