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
        const auto onCreated = [this](const auto& signal)
        {
            Resource()->OnMaterialCreated(signal.index);
        };

        const auto onDestroying = [this](const auto& signal)
        {
            Resource()->OnMaterialCreated(signal.index);
        };

        init.owner.On<Arca::CreatedKnown<Asset::ImageMaterial>>(onCreated);
        init.owner.On<Arca::CreatedKnown<Asset::LineMaterial>>(onCreated);
        init.owner.On<Arca::CreatedKnown<Asset::RegionMaterial>>(onCreated);
        init.owner.On<Arca::DestroyingKnown<Asset::ImageMaterial>>(onDestroying);
        init.owner.On<Arca::DestroyingKnown<Asset::LineMaterial>>(onDestroying);
        init.owner.On<Arca::DestroyingKnown<Asset::RegionMaterial>>(onDestroying);
    }
}