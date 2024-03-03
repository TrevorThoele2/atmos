#include "NullTextManager.h"

namespace Atmos::Render
{
    class FontAssetResourceImplementation final : public Asset::Resource::Font
    {
    public:
        FontAssetResourceImplementation() = default;

        [[nodiscard]] Render::LoadedGlyph Load(char32_t codepoint, bool bold, bool italics) const override
        {
            return {};
        }

        [[nodiscard]] float MaximumGlyphHeight() const override
        {
            return 0.0f;
        }
    };

    std::unique_ptr<Asset::Resource::Font> NullTextManager::CreateFontResource(
        const Buffer& buffer, const Name& name) const
    {
        return std::make_unique<FontAssetResourceImplementation>();
    }
}