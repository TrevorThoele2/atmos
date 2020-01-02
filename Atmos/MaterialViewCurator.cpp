#include "MaterialViewCurator.h"

#include "GraphicsManager.h"

namespace Atmos::Render
{
    void MaterialViewCurator::PostConstructImplementation()
    {
        toRender = Owner().Batch<Arca::All<MaterialViewCore, Arca::Either<Bounds>>>();

        camera = Arca::GlobalPtr<Camera>(Owner());
    }

    void MaterialViewCurator::WorkImplementation(Stage& stage)
    {
        auto graphics = &**Arca::ComputedPtr<GraphicsManager*>(Owner());

        for(auto& loop : toRender)
        {
            auto& core = *std::get<0>(loop);
            auto& bounds = *std::get<1>(loop);

            auto position = bounds.Position();
            position.x -= camera->ScreenSides().Left();
            position.y -= camera->ScreenSides().Top();

            MaterialRender render
            {
                core.Material().Get(),
                position,
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