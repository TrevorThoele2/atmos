#include "Camera.h"

namespace Atmos::Render
{
    Camera::Camera(Init init) : ClosedTypedRelic(init)
    {}

    AxisAlignedBox2D Camera::ScreenSides() const
    {
        return AxisAlignedBox2D
        {
            center,
            Size2D
            {
                static_cast<Size2D::Value>(size.width),
                static_cast<Size2D::Value>(size.height)
            }
        };
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Render::Camera, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.center);
        archive(object.size);
        archive(object.zoom);
    }
}