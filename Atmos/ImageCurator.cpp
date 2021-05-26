#include "ImageCurator.h"

namespace Atmos::Render
{
    ImageCurator::ImageCurator(Init init) : ObjectCurator(init)
    {
        Owner().On<Arca::MatrixFormed<WorldMatrix>>(
            [this](const Arca::MatrixFormed<WorldMatrix>& signal)
            {
                OnCreated(signal);
            });

        Owner().On<Arca::MatrixDissolved<WorldMatrix>>(
            [this](const Arca::MatrixDissolved<WorldMatrix>& signal)
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
            auto& renderCore = *std::get<0>(*index->value);
            auto& core = *std::get<1>(*index->value);
            auto& bounds = *std::get<2>(*index->value);
            const auto asset = core.asset.Get();
            const auto material = renderCore.material;
            if (!asset || !material || !asset->Resource())
                continue;

            const auto boundsSpace = bounds.Space();
            const auto assetIndex = core.assetIndex;
            const auto position = ToRenderPoint(bounds.Position(), cameraTopLeft, boundsSpace);
            const auto rotation = bounds.Rotation();
            const auto color = renderCore.color;

            const auto resource = const_cast<Asset::Resource::Image*>(asset->Resource());

            const auto viewSlice = Arca::Index<ViewSlice>(index->id, Owner());
            const auto assetSlice = asset->Slice(assetIndex);
            const auto [size, slice] = ViewSliceDependent(viewSlice, assetSlice, bounds.Size(), bounds.Scalers());

            const ImageRender render
            {
                resource,
                slice,
                material,
                position,
                size,
                rotation,
                color,
                ToRenderSpace(boundsSpace)
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
            if (bounds)
                bounds->BaseSize(baseSize);
        }

        if (command.assetIndex)
            core->assetIndex = *command.assetIndex;
    }

    std::vector<Arca::RelicID> ImageCurator::Handle(const FindImagesByBox& command) const
    {
        auto indices = octree.AllWithin(command.box);
        std::vector<Arca::RelicID> returnValue;
        returnValue.reserve(indices.size());
        for (auto& index : indices)
            returnValue.push_back(index->id);
        return returnValue;
    }

    void ImageCurator::OnCreated(const Arca::MatrixFormed<WorldMatrix>& signal)
    {
        octree.Add(signal.index.ID(), signal.index, BoxFor(signal.index));
    }

    void ImageCurator::OnDestroying(const Arca::MatrixDissolved<WorldMatrix>& signal)
    {
        octree.Remove(signal.index.ID(), BoxFor(signal.index));
    }

    Spatial::AxisAlignedBox3D ImageCurator::BoxFor(const WorldIndex& index)
    {
        const auto& bounds = *std::get<2>(*index);
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

    std::tuple<Spatial::Size2D, Spatial::AxisAlignedBox2D> ImageCurator::ViewSliceDependent(
        Arca::Index<ViewSlice> viewSlice,
        const Spatial::AxisAlignedBox2D& assetSlice,
        const Spatial::Size2D& boundsSize,
        const Spatial::Scalers2D& scalers)
    {
        if (viewSlice)
        {
            const auto viewSliceBox = viewSlice->box;
            const auto slice = Spatial::ToAxisAlignedBox2D(
                std::max(assetSlice.Left(), viewSliceBox.Left() / scalers.x),
                std::max(assetSlice.Top(), viewSliceBox.Top() / scalers.y),
                std::min(assetSlice.Right(), viewSliceBox.Right() / scalers.x),
                std::min(assetSlice.Bottom(), viewSliceBox.Bottom() / scalers.y));
            return { viewSliceBox.size, slice };
        }
        else
            return { boundsSize, assetSlice };
    }
}