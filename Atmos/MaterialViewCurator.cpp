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
        const AxisAlignedBox3D queryBox
        {
            Position3D
            {
                camera->center.x,
                camera->center.y,
                0
            },
            Size3D
            {
                static_cast<Size3D::Value>(camera->size.width),
                static_cast<Size3D::Value>(camera->size.height),
                std::numeric_limits<Size3D::Value>::max()
            }
        };

        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();

        auto materialViews = octree.AllWithin(queryBox);

        auto graphics = Arca::Postulate<GraphicsManager*>(Owner()).Get();

        for (auto& index : materialViews)
        {
            auto& core = *std::get<0>(*index->value);
            auto& bounds = *std::get<1>(*index->value);

            const auto boundsPosition = bounds.Position();

            const MaterialRender render
            {
                core.material.Get(),
                Position3D
                {
                    boundsPosition.x - cameraLeft,
                    boundsPosition.y - cameraTop,
                    boundsPosition.z
                },
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
        const auto index = Arca::Index<MaterialViewCore>(command.id, Owner());
        if (!index)
            return;

        auto core = MutablePointer().Of(index);
        if (command.material)
        {
            core->material = *command.material;
            CalculateMaterialSlice(*core);
        }

        if (command.index)
        {
            core->materialIndex = *command.index;
            CalculateMaterialSlice(*core);
        }

        if (command.color)
            core->color = *command.color;

        if (command.patchShader)
            core->patchShader = *command.patchShader;
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

    AxisAlignedBox3D MaterialViewCurator::BoxFor(const Index& view)
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