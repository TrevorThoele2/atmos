#include "ImageCurator.h"

#include "MainSurface.h"

namespace Atmos::Render
{
    ImageCurator::ImageCurator(Init init) :
        Curator(init), camera(init.owner)
    {
        Owner().On<Arca::MatrixFormed<Matrix>>(
            [this](const Arca::MatrixFormed<Matrix>& signal)
            {
                OnCreated(signal);
            });

        Owner().On<Arca::MatrixDissolved<Matrix>>(
            [this](const Arca::MatrixDissolved<Matrix>& signal)
            {
                OnDestroying(signal);
            });
    }

    void ImageCurator::Work()
    {
        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();

        const Spatial::AxisAlignedBox3D queryBox
        {
            Spatial::Point3D
            {
                camera->Position().x,
                camera->Position().y,
                0
            },
            Spatial::Size3D
            {
                static_cast<Spatial::Size3D::Value>(camera->Size().width),
                static_cast<Spatial::Size3D::Value>(camera->Size().height),
                std::numeric_limits<Spatial::Size3D::Value>::max()
            }
        };

        auto indices = octree.AllWithin(queryBox);

        const auto mainSurface = Arca::Index<MainSurface>(Owner());

        for (auto& index : indices)
        {
            auto& core = *std::get<0>(*index->value);
            auto& bounds = *std::get<1>(*index->value);
            const auto asset = core.asset.Get();
            const auto material = core.material.Get();
            if (!asset || !material || !asset->Resource())
                continue;

            const auto assetIndex = core.assetIndex;
            const auto position = bounds.Position();
            const auto size = bounds.Size();
            const auto rotation = bounds.Rotation();
            const auto color = core.color;

            const ImageRender render
            {
                asset,
                assetIndex,
                asset->Slice(assetIndex),
                material,
                Spatial::Point3D
                {
                    position.x - cameraLeft,
                    position.y - cameraTop,
                    position.z
                },
                size,
                rotation,
                color
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
                : Spatial::Size2D{ 0, 0 };
            auto bounds = MutablePointer().Of<Spatial::Bounds>(index.ID());
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

    Spatial::AxisAlignedBox3D ImageCurator::BoxFor(const Index& index)
    {
        const auto& bounds = *std::get<1>(*index);
        return Spatial::AxisAlignedBox3D
        {
            bounds.Position(),
            Spatial::Size3D
            {
                bounds.Size().width,
                bounds.Size().height,
                1
            }
        };
    }
}