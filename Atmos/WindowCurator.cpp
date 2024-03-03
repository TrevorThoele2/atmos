#include "WindowCurator.h"

#include <Arca/Reliquary.h>

#include "WindowSizeChanged.h"

#include "Camera.h"
#include "ScaleBounds.h"

namespace Atmos::Window
{
    Curator::Curator(Init init) : Arca::Curator(init), information(init.owner.Find<Information>())
    {}

    void Curator::Handle(const ChangeSize& command)
    {
        ChangeWindowSize(command.size);
        Owner().Raise(SizeChanged{ command.size });
    }

    void Curator::ChangeWindowSize(const Spatial::Size2D& size)
    {
        information->window->ChangeSize(size);

        const auto camera = Owner().Find<Render::Camera>();
        Owner().Do(Spatial::ScaleBounds{
            camera.ID(),
            Spatial::Scalers2D
            {
                Spatial::Scalers2D::Value(size.width),
                Spatial::Scalers2D::Value(size.height)
            } });
    }
}