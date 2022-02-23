#include "RenderObjectCurator.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Render
{
    ObjectCurator::ObjectCurator(Init init) :
        Curator(init), camera(init.owner.Find<Camera>())
    {}

    ObjectCurator::~ObjectCurator() = default;

    void ObjectCurator::Handle(const Work&)
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

        const auto mainSurface = Owner().Find<MainSurface>();
        WorkImpl(cameraBox, cameraTopLeft, *mainSurface);
    }

    int ObjectCurator::ToRenderSpace(Spatial::Space space)
    {
        switch(space)
        {
        case Spatial::Space::World:
            return 0;
        case Spatial::Space::Screen:
            return 1;
        }

        throw std::runtime_error("Unknown Spatial::Space value.");
    }

    Spatial::Point3D ObjectCurator::ToRenderPoint(Spatial::Point3D position, Spatial::Point2D cameraTopLeft, Spatial::Space space)
    {
        switch (space)
        {
        case Spatial::Space::World:
            return ToWorldPoint(position, cameraTopLeft);
        case Spatial::Space::Screen:
            return ToScreenPoint(position);
        }

        throw std::runtime_error("Unknown Spatial::Space value.");
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

    Spatial::AxisAlignedBox2D ObjectCurator::ViewSliceBox(Arca::Index<ViewSlice> viewSlice)
    {
        auto viewSliceValue = viewSlice.Get();
        return viewSliceValue
            ? viewSliceValue->box
            : Spatial::AxisAlignedBox2D
            {
                Spatial::Point2D{},
                Spatial::Size2D{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()}
            };
    }
}