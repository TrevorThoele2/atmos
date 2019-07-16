#include "Size2D.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    Size2D::Size2D() :
        baseWidth(0.0f), baseHeight(0.0f)
    {}

    Size2D::Size2D(Value baseWidth, Value baseHeight) :
        baseWidth(baseWidth), baseHeight(baseHeight), widthScaler(1.0f), heightScaler(1.0f)
    {}

    Size2D::Size2D(Value baseWidth, Value baseHeight, Scaler widthScaler, Scaler heightScaler) :
        baseWidth(baseWidth), baseHeight(baseHeight), widthScaler(widthScaler), heightScaler(heightScaler)
    {}

    Size2D::Size2D(const Size2D& arg) :
        baseWidth(arg.baseWidth), baseHeight(arg.baseHeight), widthScaler(arg.widthScaler), heightScaler(arg.heightScaler)
    {}

    Size2D& Size2D::operator=(const Size2D& arg)
    {
        baseWidth = arg.baseWidth;
        baseHeight = arg.baseHeight;
        widthScaler = arg.widthScaler;
        heightScaler = arg.heightScaler;
        return *this;
    }

    bool Size2D::operator==(const Size2D& arg) const
    {
        return scaledWidth == arg.scaledWidth && scaledHeight == arg.scaledHeight;
    }

    bool Size2D::operator!=(const Size2D& arg) const
    {
        return !(*this == arg);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Size2D, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseWidth);
        archive(object.baseHeight);
        archive(object.widthScaler);
        archive(object.heightScaler);
    }
}