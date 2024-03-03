#pragma once

#include <Atmos/TextManager.h>

class MockTextManager final : public Atmos::Render::TextManager
{
public:
    [[nodiscard]] std::unique_ptr<Atmos::Asset::Resource::Font> CreateFontResource(
        const Atmos::Buffer& buffer, const Atmos::Name& name) const override;
};