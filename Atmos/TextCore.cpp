#include "TextCore.h"

namespace Atmos::Render
{
    TextCore::TextCore(const String& string, Arca::Index<Asset::Font> asset, float wrapWidth, bool bold, bool italics) :
        string(string), font(asset), wrapWidth(wrapWidth), bold(bold), italics(italics)
    {}

    bool TextCore::operator==(const TextCore& arg) const
    {
        return string == arg.string
            && IndexEqualsValue(font, arg.font)
            && wrapWidth == arg.wrapWidth
            && bold == arg.bold
            && italics == arg.italics;
    }

    bool TextCore::operator!=(const TextCore& arg) const
    {
        return !(*this == arg);
    }
}