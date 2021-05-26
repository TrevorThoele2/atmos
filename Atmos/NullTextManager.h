#pragma once

#include "TextManager.h"

namespace Atmos::Render
{
    class NullTextManager final : public TextManager
    {
    public:
        [[nodiscard]] std::unique_ptr<Asset::Resource::Font> CreateFontResource(
            const Buffer& buffer, const Name& name) const override;

        [[nodiscard]] TextData DataFor(
            Asset::Resource::Font& font, const String& string, float wrapWidth, bool bold, bool italics) override;
    };
}