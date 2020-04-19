#pragma once

#include "String.h"

namespace Atmos::Asset
{
    class ShaderAssetData
    {
    public:
        virtual ~ShaderAssetData() = 0;

        [[nodiscard]] const String& EntryPoint() const;
    protected:
        ShaderAssetData(const String& entryPoint);
    private:
        String entryPoint;
    };
}