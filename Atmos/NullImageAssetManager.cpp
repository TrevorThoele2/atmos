#include "NullImageAssetManager.h"

namespace Atmos::Asset
{
    Resource::Loaded<Resource::Image> NullImageManager::Load(const File::Path&)
    {
        return {};
    }

    Resource::Loaded<Resource::Image> NullImageManager::Load(const Buffer&)
    {
        return {};
    }
}