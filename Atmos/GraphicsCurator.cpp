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
            MutablePointer(loop)->FileData()->Release();

        auto mainSurface = MutablePointer<MainSurface>();
        mainSurface->Release();
        for (auto& loop : Owner().Batch<AncillarySurface>())
            MutablePointer(loop)->Release();

        for (auto& loop : Owner().Batch<Canvas>())
            MutablePointer(loop)->Release();

        manager->Reconstruct();

        for (auto& loop : shaderAssetBatch)
            MutablePointer(loop)->FileData()->Reset();

        mainSurface->Reset();
        for (auto& loop : Owner().Batch<AncillarySurface>())
            MutablePointer(loop)->Reset();

        for (auto& loop : Owner().Batch<Canvas>())
            MutablePointer(loop)->Reset();
    }
}
