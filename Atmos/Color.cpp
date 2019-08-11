#include "Color.h"

#include <Inscription\Scribe.h>

namespace Atmos::Render
{
    Color::Color() : alpha(255), red(255), green(255), blue(255)
    {}

    Color::Color(Value alpha, Value red, Value green, Value blue) : alpha(alpha), red(red), green(green), blue(blue)
    {}

    Color::Color(const Color& arg) : alpha(arg.alpha), red(arg.red), green(arg.green), blue(arg.blue)
    {}

    Color& Color::operator=(const Color& arg)
    {
        alpha = arg.alpha;
        red = arg.red;
        green = arg.green;
        blue = arg.blue;
        return *this;
    }

    bool Color::operator==(const Color& arg) const
    {
        return alpha == arg.alpha && red == arg.red && green == arg.green && blue == arg.blue;
    }

    bool Color::operator!=(const Color& arg) const
    {
        return !(*this == arg);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Render::Color, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.alpha);
        archive(object.red);
        archive(object.green);
        archive(object.blue);
    }

    void Scribe<::Atmos::Render::Color, BinaryArchive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}