#include "NullTextManager.h"

namespace Atmos::Render
{
    class FontAssetResourceImplementation final : public Asset::Resource::Font
    {
    public:
        FontAssetResourceImplementation() = default;

        [[nodiscard]] Spatial::Size2D Size(const String& string, bool bold, bool italics) const override
        {
            return {};
        }
    };

    std::unique_ptr<Asset::Resource::Font> NullTextManager::CreateFontResource(
        const Buffer& buffer, const Name& name) const
    {
        return std::make_unique<FontAssetResourceImplementation>();
    }

    TextData NullTextManager::DataFor(Asset::Resource::Font&, const String&, float, bool, bool)
    {
        return {};
    }
}