#pragma once

#include <Atmos/FontAssetResource.h>

using namespace Atmos;

class MockFontAssetResource final : public Asset::Resource::Font
{
public:
    MockFontAssetResource();

    [[nodiscard]] Render::LoadedGlyph Load(char32_t codepoint, bool bold, bool italics) const override;
    [[nodiscard]] float MaximumGlyphHeight() const override;
};