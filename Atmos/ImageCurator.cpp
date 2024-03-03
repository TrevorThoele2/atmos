#include "ImageCurator.h"

#include "MainSurface.h"

namespace Atmos::Render
{
    ImageCurator::ImageCurator(Init init) :
        Curator(init), camera(init.owner)
    {
        Owner().ExecuteOn<Arca::CreatedKnown<ImageCore>>(
            [this](const Arca::CreatedKnown<ImageCore>& signal)
            {
                OnCoreCreated(signal);
            });

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

        auto images = octree.AllWithin(queryBox);

        const auto mainSurface = Arca::Index<MainSurface>(Owner());

        for (auto& index : images)
        {
            auto& core = *std::get<0>(*index->value);
            auto& bounds = *std::get<1>(*index->value);

            const auto boundsPosition = bounds.Position();

            const ImageRender render
            {
                core.asset.Get(),
                core.assetSlice,
                Position3D
                {
                    boundsPosition.x - cameraLeft,
                    boundsPosition.y - cameraTop,
                    boundsPosition.z
                },
                bounds.Size(),
                core.color,
                core.material.Get()
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
            CalculateAssetSlice(*core);
        }

        if (command.assetIndex)
        {
            core->assetIndex = *command.assetIndex;
            CalculateAssetSlice(*core);
        }

        if (command.color)
            core->color = *command.color;

        if (command.material)
            core->material = *command.material;
    }

    void ImageCurator::OnCoreCreated(const Arca::CreatedKnown<ImageCore>& created)
    {
        const auto index = MutablePointer().Of(created.reference);
        CalculateAssetSlice(*index);
    }

    void ImageCurator::CalculateAssetSlice(ImageCore& core)
    {
        auto& asset = core.asset;
        auto& assetSlice = core.assetSlice;
        auto& assetIndex = core.assetIndex;

        if (!asset)
        {
            assetSlice.Top(0);
            assetSlice.Bottom(0);
            assetSlice.Left(0);
            assetSlice.Right(0);
            return;
        }

        const auto columns = asset->Columns() > 0 ? asset->Columns() : 1;
        const auto rows = asset->Rows() ? asset->Rows() : 1;

        auto column = assetIndex % columns;
        if (column == 0)
            column = columns;
        --column;

        auto row = static_cast<int>(std::ceil(static_cast<float>(assetIndex) / static_cast<float>(columns)));
        --row;

        const auto indexWidth = static_cast<float>(asset->Width() / columns);
        const auto indexHeight = static_cast<float>(asset->Height() / rows);

        assetSlice.Top(row * indexHeight);
        assetSlice.Bottom(row * indexHeight + indexHeight);
        assetSlice.Left(column * indexWidth);
        assetSlice.Right(column * indexWidth + indexWidth);
    }

    void ImageCurator::OnViewFormed(const Arca::MatrixFormed<Matrix>& view)
    {
        octree.Add(view.index.ID(), view.index, BoxFor(view.index));
    }

    void ImageCurator::OnViewDissolved(const Arca::MatrixDissolved<Matrix>& view)
    {
        octree.Remove(view.index.ID(), BoxFor(view.index));
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