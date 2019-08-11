#include "Size3D.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    Size3D::Size3D() :
        baseWidth(0.0f), baseHeight(0.0f), baseDepth(0.0f), widthScaler(1.0f), heightScaler(1.0f), depthScaler(1.0f)
    {}

    Size3D::Size3D(Value baseWidth, Value baseHeight, Value baseDepth) :
        baseWidth(baseWidth), baseHeight(baseHeight), baseDepth(baseDepth),
        widthScaler(1.0f), heightScaler(1.0f), depthScaler(1.0f)
    {}

    Size3D::Size3D(Value baseWidth, Value baseHeight, Value baseDepth, Scaler widthScaler, Scaler heightScaler, Scaler depthScaler) :
        baseWidth(baseWidth), baseHeight(baseHeight), baseDepth(baseDepth),
        widthScaler(widthScaler), heightScaler(heightScaler), depthScaler(depthScaler)
    {}

    Size3D::Size3D(const Size3D& arg) :
        baseWidth(arg.baseWidth), baseHeight(arg.baseHeight), baseDepth(arg.baseDepth),
        widthScaler(arg.widthScaler), heightScaler(arg.heightScaler), depthScaler(arg.depthScaler)
    {}

    Size3D& Size3D::operator=(const Size3D& arg)
    {
        baseWidth = arg.baseWidth;
        baseHeight = arg.baseHeight;
        baseDepth = arg.baseDepth;
        widthScaler = arg.widthScaler;
        heightScaler = arg.heightScaler;
        depthScaler = arg.depthScaler;
        return *this;
    }

    bool Size3D::operator==(const Size3D& arg) const
    {
        return scaledWidth == arg.scaledWidth && scaledHeight == arg.scaledHeight && scaledDepth == arg.scaledDepth;
    }

    bool Size3D::operator!=(const Size3D& arg) const
    {
        return !(*this == arg);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Size3D, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseWidth);
        archive(object.baseHeight);
        archive(object.baseDepth);
        archive(object.widthScaler);
        archive(object.heightScaler);
        archive(object.depthScaler);
    }

    void Scribe<::Atmos::Size3D, BinaryArchive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}