#include "TextCore.h"

namespace Atmos::Render
{
    TextCore::TextCore(const String& string, Arca::Index<Asset::Font> asset, float wrapWidth, bool bold, bool italics) :
        string(string), font(asset), wrapWidth(wrapWidth), bold(bold), italics(italics)
    {}
}