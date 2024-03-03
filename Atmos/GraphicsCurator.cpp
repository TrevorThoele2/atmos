#include "GraphicsCurator.h"

#include "MainSurface.h"
#include "AncillarySurface.h"

namespace Atmos::Render
{
    GraphicsCurator::GraphicsCurator(Init init) :
        Curator(init),
        manager(init.owner)
    {}

    void GraphicsCurator::Handle(const ReconstructGraphics& reconstructGraphics)
    {
        if (!manager->ShouldReconstruct())
            return;

        auto shaderAssets = MutablePointersOf<Asset::ShaderAsset>();
        auto mainSurface = MutablePointer().Of<MainSurface>();
        auto ancillarySurfaces = MutablePointersOf<AncillarySurface>();
        auto canvases = MutablePointersOf<Canvas>();

        for (auto& asset : shaderAssets)
            asset->FileData()->Release();

        mainSurface->Release();

        for (auto& surface : ancillarySurfaces)
            surface->Release();

        for (auto& canvas : canvases)
            canvas->Release();

        manager->Reconstruct(reconstructGraphics.screenSize);

        if (manager->IsOk())
        {
            for (auto& asset : shaderAssets)
                asset->FileData()->Reset();

            mainSurface->Reset();

            for (auto& surface : ancillarySurfaces)
                surface->Reset();

            for (auto& canvas : canvases)
                canvas->Reset();
        }
    }
}
