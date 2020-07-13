#include "CameraCurator.h"

#include "Camera.h"

namespace Atmos::Render
{
    CameraCurator::CameraCurator(Init init) :
        Curator(init), camera(init.owner),
        debugCenterX(
            [this](Debug::Statistics& statistics)
            {
                statistics.window.cameraCenterX = camera->Position().x;
            },
            MutablePointer()),
        debugCenterY(
            [this](Debug::Statistics& statistics)
            {
                statistics.window.cameraCenterY = camera->Position().y;
            },
            MutablePointer())
    {}

    void CameraCurator::Work()
    {
        debugCenterX.Set();
        debugCenterY.Set();
    }
}
