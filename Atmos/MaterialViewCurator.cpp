#include "MaterialViewCurator.h"

#include "GraphicsManager.h"

namespace Atmos::Render
{
    MaterialViewCurator::MaterialViewCurator(Init init) :
        Curator(init), camera(init.owner)
    {
        Owner().ExecuteOn<Arca::MatrixFormed<Matrix>>(
            [this](const Arca::MatrixFormed<Matrix>& signal)
            {
                OnViewFormed(signal);
            });

        Owner().ExecuteOn<Arca::MatrixDissolved<Matrix>>(
            [this](const Arca::MatrixDissolved<Matrix>& signal)
            {
                OnViewDissolved(signal);
            });
    }

    void MaterialViewCurator::Work()
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
                static_cast<Size3D::Value>(camera->Size().width),
                static_cast<Size3D::Value>(camera->Size().height),
                std::numeric_limits<Size3D::Value>::max()
            }
        };

        auto materialViews = octree.AllWithin(queryBox);
        for (auto& index : materialViews)
        {
            auto& core = *std::get<0>(*index->value);
            auto& bounds = *std::get<1>(*index->value);

            auto position = bounds.Position();
            position.x -= camera->ScreenSides().Left();
            position.y -= camera->ScreenSides().Top();

            MaterialRender render
            {
                core.material.Get(),
                position,
                bounds.Size(),
                core.color,
                core.materialSlice,
                core.patchShader
            };
            graphics->StageRender(render);
        }
    }

    void MaterialViewCurator::Handle(const ChangeMaterialViewCore& command)
    {
        const auto index = Arca::ShardIndex<MaterialViewCore>(command.id, Owner());
        if (!index)
            return;

        auto data = Data(index);
        if (command.material)
        {
            data->material = *command.material;
            CalculateMaterialSlice(*data);
        }

        if (command.index)
        {
            data->materialIndex = *command.index;
            CalculateMaterialSlice(*data);
        }

        if (command.color)
            data->color = *command.color;

        if (command.patchShader)
            data->patchShader = *command.patchShader;
    }

    void MaterialViewCurator::CalculateMaterialSlice(MaterialViewCore& core)
    {
        auto& material = core.material;
        auto& materialSlice = core.materialSlice;
        auto& materialIndex = core.materialIndex;

        if (!material || !material->Image())
        {
            materialSlice.Top(0);
            materialSlice.Bottom(0);
            materialSlice.Left(0);
            materialSlice.Right(0);
            return;
        }

        const auto columns = material->Columns();
        const auto rows = material->Rows();

        auto column = materialIndex % columns;
        if (column == 0)
            column = columns;
        --column;

        auto row = static_cast<int>(std::ceil(static_cast<float>(materialIndex) / static_cast<float>(columns)));
        --row;

        const auto indexWidth = static_cast<float>(material->Image()->Width() / columns);
        const auto indexHeight = static_cast<float>(material->Image()->Height() / rows);

        materialSlice.Top(row * indexHeight);
        materialSlice.Bottom(row * indexHeight + indexHeight);
        materialSlice.Left(column * indexWidth);
        materialSlice.Right(column * indexWidth + indexWidth);
    }

    void MaterialViewCurator::OnViewFormed(const Arca::MatrixFormed<Matrix>& view)
    {
        octree.Add(view.index.ID(), view.index, BoxFor(view.index));
    }

    void MaterialViewCurator::OnViewDissolved(const Arca::MatrixDissolved<Matrix>& view)
    {
        octree.Remove(view.index.ID(), BoxFor(view.index));
    }

    AxisAlignedBox3D MaterialViewCurator::BoxFor(const MatrixIndex& view)
    {
        const auto& bounds = *std::get<1>(*view);
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