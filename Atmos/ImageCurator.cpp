#include "ImageCurator.h"

#include "RenderImage.h"
#include "StagedRenders.h"

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

        Owner().On<Spatial::BoundsChanged>(
            [this](const Spatial::BoundsChanged& signal)
            {
                OnChanged(signal);
            });
    }

    void ImageCurator::WorkImpl(
        Spatial::AxisAlignedBox3D cameraBox,
        Spatial::Point2D cameraTopLeft,
        const MainSurface& mainSurface)
    {
        const auto indices = worldOctree.AllWithin(cameraBox);

        std::vector<RenderImage> renders;
        renders.reserve(indices.size() + screenList.size());
        for (auto& index : indices)
        {
            const auto render = RenderOf(index->id, *index->value, cameraTopLeft, mainSurface);
            if (render)
                renders.push_back(*render);
        }
        for (auto& index : screenList)
        {
            const auto render = RenderOf(index.ID(), *index, cameraTopLeft, mainSurface);
            if (render)
                renders.push_back(*render);
        }

        auto stagedRenders = MutablePointer().Of<StagedRenders>();
        stagedRenders->images.insert(stagedRenders->images.end(), renders.begin(), renders.end());
    }

    void ImageCurator::Handle(const ChangeImageCore& command)
    {
        const auto index = Owner().Find<ImageCore>(command.id);
        if (index)
        {
            const auto core = MutablePointer().Of(index);
            if (command.asset)
            {
                core->asset = *command.asset;

                const auto baseSize = core->asset
                    ? core->asset->SliceSize()
                    : Spatial::Size2D{ 0, 0 };
                const auto bounds = MutablePointer().Of<Spatial::Bounds>(index.ID());
                if (bounds)
                    bounds->BaseSize(baseSize);
            }

            if (command.assetIndex)
                core->assetIndex = *command.assetIndex;
        }
    }

    std::vector<Arca::RelicID> ImageCurator::Handle(const FindImagesByBox& command) const
    {
        std::vector<Arca::RelicID> ids;

        switch (command.space)
        {
        case Spatial::Space::World:
        {
            const auto indices = worldOctree.AllWithin(command.box);
            ids.reserve(indices.size());
            for (auto& index : indices)
                ids.push_back(index->id);
            break;
        }
        case Spatial::Space::Screen:
            for(auto& index : screenList)
            {
                const auto bounds = BoundsFor(index);
                const auto box = BoxFor(bounds);
                if (Intersects(box, command.box))
                    ids.push_back(index.ID());
            }
            break;
        }

        return ids;
    }

    std::optional<RenderImage> ImageCurator::RenderOf(
        Arca::RelicID id,
        const Index::ReferenceValueT& value,
        Spatial::Point2D cameraTopLeft,
        const MainSurface& mainSurface)
    {
        auto& renderCore = *std::get<0>(value);
        auto& core = *std::get<1>(value);
        auto& bounds = *std::get<2>(value);
        const auto asset = core.asset.Get();
        const auto material = renderCore.material;
        if (asset && material && asset->Resource())
        {
            const auto boundsSpace = bounds.Space();
            const auto assetSlice = asset->Slice(core.assetIndex);

            return RenderImage
            {
                .assetResource = const_cast<Asset::Resource::Image*>(asset->Resource()),
                .assetSlice = assetSlice,
                .viewSlice = ViewSliceBox(Owner().Find<ViewSlice>(id)),
                .material = material,
                .position = ToRenderPoint(bounds.Position(), cameraTopLeft, boundsSpace),
                .size = assetSlice.size,
                .rotation = bounds.Rotation(),
                .scalers = bounds.Scalers(),
                .color = renderCore.color,
                .space = ToRenderSpace(boundsSpace),
                .surface = mainSurface.Resource()
            };
        }
        else
            return {};
    }

    void ImageCurator::OnCreated(const Arca::MatrixFormed<Matrix>& signal)
    {
        const auto index = signal.index;
        const auto bounds = BoundsFor(index);
        switch (bounds.Space())
        {
        case Spatial::Space::World:
            worldOctree.Add(index.ID(), index, BoxFor(bounds));
            break;
        case Spatial::Space::Screen:
            screenList.push_back(index);
            break;
        }
    }

    void ImageCurator::OnDestroying(const Arca::MatrixDissolved<Matrix>& signal)
    {
        const auto index = signal.index;
        const auto bounds = BoundsFor(signal.index);
        switch (bounds.Space())
        {
        case Spatial::Space::World:
            worldOctree.Remove(index.ID(), BoxFor(BoundsFor(index)));
            break;
        case Spatial::Space::Screen:
            const auto iterator = std::remove_if(
                screenList.begin(), screenList.end(), [id = index.ID()](const Index& index) { return index.ID() == id; });
            if (iterator != screenList.end())
                screenList.erase(iterator);
            break;
        }
    }

    void ImageCurator::OnChanged(const Spatial::BoundsChanged& signal)
    {
        const auto index = Owner().Find<Matrix>(signal.id);
        if (index)
        {
            const auto oldBounds = signal.previousBounds;
            if (oldBounds.Space() == Spatial::Space::World)
            {
                const auto newBounds = signal.newBounds;
                worldOctree.Move(signal.id, Owner().Find<Matrix>(signal.id), BoxFor(oldBounds), BoxFor(newBounds));
            }
        }
    }

    Spatial::Bounds ImageCurator::BoundsFor(const Index& index)
    {
        return *std::get<2>(*index);
    }

    Spatial::AxisAlignedBox3D ImageCurator::BoxFor(const Spatial::Bounds& bounds)
    {
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