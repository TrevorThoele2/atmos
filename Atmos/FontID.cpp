#include "FontID.h"

namespace Atmos::Render
{
    FontID::FontID(const Asset::Resource::Font* resource, bool bold, bool italics) :
        resource(resource), bold(bold), italics(italics)
    {}

    bool FontID::operator==(const FontID& arg) const
    {
        return resource == arg.resource && bold == arg.bold && italics == arg.italics;
    }
}