#include "ImageCurator.h"

namespace Atmos::Render
{
    ImageCurator::ImageCurator(Init init) : ObjectCurator(init)
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

    void ImageCurator::WorkImpl(
        Spatial::AxisAlignedBox3D cameraBox,
        Spatial::Point2D cameraTopLeft,
        Arca::Index<MainSurface> mainSurface)
    {
        auto indices = octree.AllWithin(cameraBox);

        for (auto& index : indices)
        {
            auto& core = *std::get<0>(*index->value);
            auto& bounds = *std::get<1>(*index->value);
            const auto asset = core.asset.Get();
            const auto material = core.material;
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
                material.ID(),
                material.Get(),
                Spatial::Point3D
                {
                    position.x - cameraTopLeft.x,
                    position.y - cameraTopLeft.y,
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
    }

    void ImageCurator::Handle(const ChangeImageMaterialAsset& command)
    {
        const auto index = Arca::Index<ImageCore>(command.id, Owner());
        if (!index)
            return;

        auto core = MutablePointer().Of(index);

        core->material = command.to;
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