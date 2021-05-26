#pragma once

#include <Atmos/TextManager.h>

class MockTextManager final : public Atmos::Render::TextManager
{
public:
    [[nodiscard]] std::unique_ptr<Atmos::Asset::Resource::Font> CreateFontResource(
        const Atmos::Buffer& buffer, const Atmos::Name& name) const override;

    [[nodiscard]] Atmos::Render::TextData DataFor(
        Atmos::Asset::Resource::Font& font,
        const Atmos::String& string,
        float wrapWidth,
        bool bold,
        bool italics) override;
};