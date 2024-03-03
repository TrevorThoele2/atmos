#include "MaterialViewCurator.h"

#include "GraphicsManagerProvider.h"

namespace Atmos::Render
{
    void MaterialViewCurator::PostConstructImplementation()
    {
        toRender = Owner().Batch<Arca::All<MaterialViewCore, Arca::Either<Bounds>>>();
    }

    void MaterialViewCurator::WorkImplementation(Stage& stage)
    {
        auto graphics = &**Owner().Find<GraphicsManagerProvider>();

        for(auto& loop : toRender)
        {
            auto& core = *std::get<0>(loop);
            auto& bounds = *std::get<1>(loop);
            MaterialRender render
            {
                core.Material().Get(),
                bounds.Position(),
                bounds.Size(),
                core.Color(),
                core.MaterialSlice(),
                core.PatchShader()
            };
            graphics->RenderMaterialView(render);
        }
    }
}

namespace Arca
{
    const TypeName Traits<Atmos::Render::MaterialViewCurator>::typeName = "MaterialViewCurator";
}