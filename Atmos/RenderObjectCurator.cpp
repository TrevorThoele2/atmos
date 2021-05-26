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

    int ObjectCurator::ToRenderSpace(Spatial::BoundsSpace space)
    {
        switch(space)
        {
        case Spatial::BoundsSpace::World:
            return 0;
        case Spatial::BoundsSpace::Screen:
            return 1;
        }

        throw std::runtime_error("Unknown Spatial::BoundsSpace value.");
    }

    Spatial::Point3D ObjectCurator::ToRenderPoint(Spatial::Point3D position, Spatial::Point2D cameraTopLeft, Spatial::BoundsSpace space)
    {
        switch (space)
        {
        case Spatial::BoundsSpace::World:
            return ToWorldPoint(position, cameraTopLeft);
        case Spatial::BoundsSpace::Screen:
            return ToScreenPoint(position);
        }

        throw std::runtime_error("Unknown Spatial::BoundsSpace value.");
    }

    Spatial::Point3D ObjectCurator::ToWorldPoint(Spatial::Point3D position, Spatial::Point2D cameraTopLeft)
    {
        return Spatial::Point3D
        {
            position.x - cameraTopLeft.x,
            position.y - cameraTopLeft.y,
            position.z
        };
    }

    Spatial::Point3D ObjectCurator::ToScreenPoint(Spatial::Point3D position)
    {
        return position;
    }
}