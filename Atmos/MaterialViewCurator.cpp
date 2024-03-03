#include "MaterialViewCurator.h"

#include "GraphicsManager.h"

namespace Atmos::Render
{
    void MaterialViewCurator::PostConstructImplementation()
    {
        Owner().ExecuteOn<Arca::MatrixFormed<MaterialMatrix>>(
            [this](const Arca::MatrixFormed<MaterialMatrix>& signal)
            {
                OnMaterialFormed(signal);
            });

        Owner().ExecuteOn<Arca::MatrixDissolved<MaterialMatrix>>(
            [this](const Arca::MatrixDissolved<MaterialMatrix>& signal)
            {
                OnMaterialDissolved(signal);
            });

        camera = Arca::GlobalIndex<Camera>(Owner());
    }

    void MaterialViewCurator::WorkImplementation(Stage& stage)
    {
        auto graphics = &**Arca::ComputedIndex<GraphicsManager*>(Owner());

        const AxisAlignedBox3D queryBox
        {
            Position3D
            {
                camera->ViewOrigin().x,
                camera->ViewOrigin().y,
                0
            },
            Size3D
            {
                camera->Size().width,
                camera->Size().height,
                std::numeric_limits<Size3D::Value>::max()
            }
        };

        for(auto& index : octree.AllInside(queryBox))
        {
            auto& core = *std::get<0>(*index.value);
            auto& bounds = *std::get<1>(*index.value);

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

    void MaterialViewCurator::OnMaterialFormed(const Arca::MatrixFormed<MaterialMatrix>& matrix)
    {
        octree.Add(matrix.index.ID(), matrix.index, BoxFor(matrix.index));
    }

    void MaterialViewCurator::OnMaterialDissolved(const Arca::MatrixDissolved<MaterialMatrix>& matrix)
    {
        octree.Remove(matrix.index.ID(), BoxFor(matrix.index));
    }

    AxisAlignedBox3D MaterialViewCurator::BoxFor(const MaterialIndex& matrix)
    {
        const auto& bounds = *std::get<1>(*matrix);
        return AxisAlignedBox3D
        {
            bounds.Position(),
            Size3D
            {
                bounds.Size().width,
                bounds.Size().height,
                1
            }
        };
    }
}