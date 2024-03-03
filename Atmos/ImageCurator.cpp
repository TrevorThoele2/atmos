#include "ImageCurator.h"

#include "RenderImage.h"

namespace Atmos::Render
{
    ImageCurator::ImageCurator(Init init, GraphicsManager& graphicsManager) : ObjectCurator(init), graphicsManager(&graphicsManager)
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

        for (auto& index : indices)
            StageRender(index->id, *index->value, cameraTopLeft, mainSurface);
        for (auto& index : screenList)
            StageRender(index.ID(), *index, cameraTopLeft, mainSurface);
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

    void ImageCurator::StageRender(
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
            const auto assetIndex = core.assetIndex;
            const auto position = ToRenderPoint(bounds.Position(), cameraTopLeft, boundsSpace);
            const auto rotation = bounds.Rotation();
            const auto scalers = bounds.Scalers();
            const auto color = renderCore.color;

            const auto resource = const_cast<Asset::Resource::Image*>(asset->Resource());
            
            const auto assetSize = asset->Size();
            const auto assetSliceCenter = asset->Slice(assetIndex).center;
            const auto assetSliceSize = asset->SliceSize();
            const auto assetSliceStandard = ViewSliceClamp(
                Owner().Find<ViewSlice>(id),
                Spatial::ToAxisAlignedBox2D(0, 0, assetSliceSize.width * scalers.x, assetSliceSize.height * scalers.y));
            const auto assetSlice = Spatial::AxisAlignedBox2D{
                assetSliceStandard.center + Spatial::Point2D{
                    assetSliceCenter.x - assetSliceSize.width / 2,
                    assetSliceCenter.y - assetSliceSize.height / 2},
                assetSliceStandard.size };
            const auto slice = Spatial::ScaleOf(
                assetSlice, Spatial::ToAxisAlignedBox2D(0, 0, assetSize.width, assetSize.height));

            const RenderImage render
            {
                resource,
                slice,
                material,
                position,
                rotation,
                scalers,
                color,
                ToRenderSpace(boundsSpace),
                mainSurface.Resource()
            };
            graphicsManager->Stage(render);
        }
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