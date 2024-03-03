#include "ScreenSize.h"

namespace Atmos
{
    ScreenSize::ScreenSize(Dimension width, Dimension height) : width(width), height(height)
    {}

    bool ScreenSize::operator==(const ScreenSize& arg) const
    {
        return width == arg.width && height == arg.height;
    }

    bool ScreenSize::operator!=(const ScreenSize& arg) const
    {
        return !(*this == arg);
    }
}

namespace Inscription
{
    void Scribe<Atmos::ScreenSize, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.width);
        archive(object.height);
    }
}