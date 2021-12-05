#pragma once

#include <memory>
#include "FontAssetResource.h"
#include "Name.h"

namespace Atmos::Render
{
    class TextManager
    {
    public:
        virtual ~TextManager() = 0;

        [[nodiscard]] virtual std::unique_ptr<Asset::Resource::Font> CreateFontResource(
            const Buffer& buffer, const Name& name) const = 0;
    };
}