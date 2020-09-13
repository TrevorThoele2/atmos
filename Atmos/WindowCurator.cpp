#include "WindowCurator.h"

#include <Arca/Reliquary.h>

#include "WindowSizeChanged.h"
#include "WindowProvider.h"

#include "Camera.h"
#include "ScaleBounds.h"

namespace Atmos::Window
{
    void Curator::Handle(const ChangeSize& command)
    {
        ChangeWindowSize(command.size);
        Owner().Raise(SizeChanged{ command.size });
    }

    void Curator::ChangeWindowSize(const Spatial::ScreenSize& size)
    {
        window->ChangeSize(size);

        const auto camera = Arca::Index<Render::Camera>(Owner());
        Owner().Do(Spatial::ScaleBounds{
            camera.ID(),
            Spatial::Scalers2D
            {
                Spatial::Scalers2D::Value(size.width),
                Spatial::Scalers2D::Value(size.height)
            } });
    }
}