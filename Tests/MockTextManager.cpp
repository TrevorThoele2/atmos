#include "MockTextManager.h"

#include "MockFontAssetResource.h"

std::unique_ptr<Atmos::Asset::Resource::Font> MockTextManager::CreateFontResource(
    const Atmos::Buffer& buffer, const Atmos::Name& name) const
{
    return std::make_unique<MockFontAssetResource>();
}