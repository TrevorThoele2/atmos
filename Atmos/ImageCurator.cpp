#include "ImageCurator.h"

#include "MainSurface.h"

namespace Atmos::Render
{
    ImageCurator::ImageCurator(Init init) :
        Curator(init), camera(init.owner)
    {
        Owner().ExecuteOn<Arca::MatrixFormed<Matrix>>(
            [this](const Arca::MatrixFormed<Matrix>& signal)
            {
                OnCreated(signal);
            });

        Owner().ExecuteOn<Arca::MatrixDissolved<Matrix>>(
            [this](const Arca::MatrixDissolved<Matrix>& signal)
            {
                OnDestroying(signal);
            });
    }

    void ImageCurator::Work()
    {
        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();

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

        auto indices = octree.AllWithin(queryBox);

        const auto mainSurface = Arca::Index<MainSurface>(Owner());

        for (auto& index : indices)
        {
            auto& core = *std::get<0>(*index->value);
            auto& bounds = *std::get<1>(*index->value);
            if (!core.asset || !core.material)
                continue;

            const auto boundsPosition = bounds.Position();

            const ImageRender render
            {
                core.asset.Get(),
                core.assetIndex,
                core.asset->Slice(core.assetIndex),
                core.material.Get(),
                Position3D
                {
                    boundsPosition.x - cameraLeft,
                    boundsPosition.y - cameraTop,
                    boundsPosition.z
                },
                bounds.Size(),
                bounds.Rotation(),
                core.color
            };
            mainSurface->StageRender(render);
        }
    }

    void ImageCurator::Handle(const ChangeImageCore& command)
    {
        const auto index = Arca::Index<ImageCore>(command.id, Owner());
        if (!index)
            return;

        auto core = MutablePointer().Of(index);
        if (command.asset)
        {
            core->asset = *command.asset;

            const auto baseSize = core->asset
                ? core->asset->SliceSize()
                : Size2D{ 0, 0 };
            auto bounds = MutablePointer().Of<Bounds>(index.ID());
            bounds->BaseSize(baseSize);
        }

        if (command.assetIndex)
            core->assetIndex = *command.assetIndex;

        if (command.color)
            core->color = *command.color;

        if (command.material)
            core->material = *command.material;
    }

    void ImageCurator::OnCreated(const Arca::MatrixFormed<Matrix>& signal)
    {
        octree.Add(signal.index.ID(), signal.index, BoxFor(signal.index));
    }

    void ImageCurator::OnDestroying(const Arca::MatrixDissolved<Matrix>& signal)
    {
        octree.Remove(signal.index.ID(), BoxFor(signal.index));
    }

    AxisAlignedBox3D ImageCurator::BoxFor(const Index& index)
    {
        const auto& bounds = *std::get<1>(*index);
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