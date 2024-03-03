#include "MockTextManager.h"

class MockFontAssetResource final : public Atmos::Asset::Resource::Font
{};

std::unique_ptr<Atmos::Asset::Resource::Font> MockTextManager::CreateFontResource(
    const Atmos::Buffer& buffer, const Atmos::Name& name) const
{
    return std::make_unique<MockFontAssetResource>();
}

Atmos::Render::TextData MockTextManager::DataFor(
    Atmos::Asset::Resource::Font&, const Atmos::String&, float, bool, bool)
{
    return {{}, Atmos::Spatial::Size2D{1, 1}};
}