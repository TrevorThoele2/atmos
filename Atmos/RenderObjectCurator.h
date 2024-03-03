#pragma once

#include <Arca/Curator.h>

#include "Camera.h"
#include "AxisAlignedBox3D.h"
#include "MainSurface.h"
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
            Arca::Index<MainSurface> mainSurface) = 0;
    private:
        Arca::Index<Camera> camera;
        Arca::Index<MainSurface> mainSurface;
    };
}