#include "GraphicsCurator.h"

#include "MainSurface.h"
#include "AncillarySurface.h"

namespace Atmos::Render
{
    GraphicsCurator::GraphicsCurator(Init init) :
        Curator(init),
        manager(init.owner)
    {}

    void GraphicsCurator::Handle(const ReconstructGraphics&)
    {
        auto shaderAssetBatch = Owner().Batch<Asset::ShaderAsset>();
        for (auto& loop : shaderAssetBatch)
            MutablePointer().Of(loop)->FileData()->Release();

        auto mainSurface = MutablePointer().Of<MainSurface>();
        mainSurface->Release();
        for (auto& loop : Owner().Batch<AncillarySurface>())
            MutablePointer().Of(loop)->Release();

        for (auto& loop : Owner().Batch<Canvas>())
            MutablePointer().Of(loop)->Release();

        manager->Reconstruct();

        for (auto& loop : shaderAssetBatch)
            MutablePointer().Of(loop)->FileData()->Reset();

        mainSurface->Reset();
        for (auto& loop : Owner().Batch<AncillarySurface>())
            MutablePointer().Of(loop)->Reset();

        for (auto& loop : Owner().Batch<Canvas>())
            MutablePointer().Of(loop)->Reset();
    }
}
