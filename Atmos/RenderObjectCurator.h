#pragma once

#include <Arca/Curator.h>

#include "Camera.h"
#include "MainSurface.h"
#include "AxisAlignedBox3D.h"
#include "Work.h"

namespace Atmos::Render
{
    class ObjectCurator : public Arca::Curator
    {
    public:
        ObjectCurator(Init init);
        virtual ~ObjectCurator() = 0;

        void Handle(const Work& command);
    protected:
        virtual void WorkImpl(
            Spatial::AxisAlignedBox3D cameraBox,
            Spatial::Point2D cameraTopLeft,
            const MainSurface& mainSurface) = 0;

        [[nodiscard]] static int ToRenderSpace(Spatial::Space space);

        [[nodiscard]] static Spatial::Point3D ToRenderPoint(Spatial::Point3D position, Spatial::Point2D cameraTopLeft, Spatial::Space space);
        [[nodiscard]] static Spatial::Point3D ToWorldPoint(Spatial::Point3D position, Spatial::Point2D cameraTopLeft);
        [[nodiscard]] static Spatial::Point3D ToScreenPoint(Spatial::Point3D position);
    private:
        Arca::Index<Camera> camera;
    };
}