#pragma once

#include <memory>
#include "FontAssetResource.h"
#include "TextData.h"
#include "Name.h"

namespace Atmos::Render
{
    class TextManager
    {
    public:
        virtual ~TextManager() = 0;

        [[nodiscard]] virtual std::unique_ptr<Asset::Resource::Font> CreateFontResource(
            const Buffer& buffer, const Name& name) const = 0;

        [[nodiscard]] virtual TextData DataFor(
            Asset::Resource::Font& font, const String& string, float wrapWidth, bool bold, bool italics) = 0;
    };
}