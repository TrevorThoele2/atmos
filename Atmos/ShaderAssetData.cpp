#include "ShaderAssetData.h"

namespace Atmos::Asset
{
    ShaderAssetData::~ShaderAssetData() = default;

    const String& ShaderAssetData::EntryPoint() const
    {
        return entryPoint;
    }

    ShaderAssetData::ShaderAssetData(const String& entryPoint) : entryPoint(entryPoint)
    {}
}