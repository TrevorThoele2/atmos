#include "Surface.h"

namespace Atmos::Render
{
    void Surface::StageRender(const ImageRender& imageRender) const
    {
        Resource()->StageRender(imageRender);
    }

    void Surface::StageRender(const LineRender& lineRender) const
    {
        Resource()->StageRender(lineRender);
    }

    void Surface::StageRender(const RegionRender& regionRender) const
    {
        Resource()->StageRender(regionRender);
    }

    void Surface::DrawFrame() const
    {
        Resource()->DrawFrame(init.owner, core->backgroundColor);
    }

    Spatial::ScreenSize Surface::Size() const
    {
        return Resource()->Size();
    }

    auto Surface::Resource() const -> ResourceT*
    {
        return core->resource.get();
    }

    Surface::Surface(Arca::RelicInit init, ResourcePtr&& resource) :
        core(init.Create<Core>(std::move(resource))),
        init(init)
    {
        init.owner.On<Arca::CreatedKnown<Asset::Material>>(
            [this](const Arca::CreatedKnown<Asset::Material>& signal)
            {
                Resource()->OnMaterialCreated(signal.index);
            });
        init.owner.On<Arca::DestroyingKnown<Asset::Material>>(
            [this](const Arca::DestroyingKnown<Asset::Material>& signal)
            {
                Resource()->OnMaterialDestroying(signal.index);
            });
    }
}