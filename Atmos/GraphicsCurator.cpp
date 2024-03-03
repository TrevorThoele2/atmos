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
            Data(loop)->FileData()->Release();

        auto mainSurface = Data<MainSurface>();
        mainSurface->Release();
        for (auto& loop : Owner().Batch<AncillarySurface>())
            Data(loop)->Release();

        for (auto& loop : Owner().Batch<Canvas>())
            Data(loop)->Release();

        (*manager)->Reconstruct();

        for (auto& loop : shaderAssetBatch)
            Data(loop)->FileData()->Reset();

        mainSurface->Reset();
        for (auto& loop : Owner().Batch<AncillarySurface>())
            Data(loop)->Reset();

        for (auto& loop : Owner().Batch<Canvas>())
            Data(loop)->Reset();
    }
}
