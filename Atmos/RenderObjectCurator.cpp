#include "RenderObjectCurator.h"

namespace Atmos::Render
{
    ObjectCurator::ObjectCurator(Init init) :
        Curator(init), camera(init.owner), mainSurface(init.owner)
    {}

    ObjectCurator::~ObjectCurator() = default;

    void ObjectCurator::Handle(const Work& command)
    {
        const auto cameraLeft = camera->Sides().Left();
        const auto cameraTop = camera->Sides().Top();
        const auto cameraPosition = camera->Position();
        const auto cameraSize = camera->Size();

        const Spatial::AxisAlignedBox3D cameraBox
        {
            Spatial::Point3D
            {
                cameraPosition.x,
                cameraPosition.y,
                0
            },
            Spatial::Size3D
            {
                static_cast<Spatial::Size3D::Value>(cameraSize.width),
                static_cast<Spatial::Size3D::Value>(cameraSize.height),
                std::numeric_limits<Spatial::Size3D::Value>::max()
            }
        };

        const Spatial::Point2D cameraTopLeft{ cameraLeft, cameraTop };

        WorkImpl(cameraBox, cameraTopLeft, mainSurface);
    }
}