#include "TextCurator.h"

#include "TextRender.h"

#include "ColorChanged.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Render
{
    TextCurator::TextCurator(Init init, TextManager& manager, GraphicsManager& graphicsManager) :
        ObjectCurator(init), manager(&manager), graphicsManager(&graphicsManager)
    {
        Owner().On<Arca::MatrixFormed<Matrix>>([this](const Arca::MatrixFormed<Matrix>& command) { CalculateForText(command.index.ID()); });
        Owner().On<ColorChanged>([this](const ColorChanged& command) { CalculateForText(command.id); });
    }

    void TextCurator::Handle(const ChangeTextCore& command)
    {
        bool changed = false;

        const auto attemptChange = [&changed](auto& set, auto& optional)
        {
            if (optional && set != *optional)
            {
                set = *optional;
                changed = true;
            }
        };

        const auto core = MutablePointer().Of<TextCore>(command.id);
        if (core)
        {
            attemptChange(core->string, command.string);
            attemptChange(core->font, command.font);
            attemptChange(core->wrapWidth, command.wrapWidth);
            attemptChange(core->bold, command.bold);
            attemptChange(core->italics, command.italics);

            if (changed)
                CalculateForText(command.id);
        }
    }
    
    std::unique_ptr<Asset::Resource::Font> TextCurator::Handle(const Asset::Resource::Create<Asset::Resource::Font>& command)
    {
        return manager->CreateFontResource(command.buffer, command.name);
    }

    void TextCurator::WorkImpl(
        Spatial::AxisAlignedBox3D cameraBox,
        Spatial::Point2D cameraTopLeft,
        const MainSurface& mainSurface)
    {
        auto indices = Owner().Batch<Matrix>();

        for (auto& index : indices)
            StageRender(
                std::get<0>(index).ID(),
                *std::get<0>(index),
                *std::get<1>(index),
                *std::get<2>(index),
                cameraTopLeft,
                mainSurface);
    }

    void TextCurator::StageRender(
        Arca::RelicID id,
        const RenderCore& renderCore,
        const TextCore& core,
        const Spatial::Bounds& bounds,
        Spatial::Point2D cameraTopLeft,
        const MainSurface& mainSurface)
    {
        const auto asset = core.font.Get();
        const auto material = renderCore.material;
        const auto resource = const_cast<Resource::Text*>(core.resource.get());
        if (asset && material && asset->Resource() && resource)
        {
            const auto boundsSpace = bounds.Space();
            const auto position = ToRenderPoint(bounds.Position(), cameraTopLeft, boundsSpace);
            const auto baseSize = bounds.BaseSize();
            const auto rotation = bounds.Rotation();
            const auto color = renderCore.color;

            const auto viewSlice = Owner().Find<ViewSlice>(id);
            const auto [size, slice] = ViewSliceDependent(viewSlice, baseSize, bounds.Size(), bounds.Scalers());

            const TextRender render
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
            mainSurface.StageRender(render);
        }
    }

    std::tuple<Spatial::Size2D, Spatial::AxisAlignedBox2D> TextCurator::ViewSliceDependent(
        Arca::Index<ViewSlice> viewSlice,
        const Spatial::Size2D& baseSize,
        const Spatial::Size2D& boundsSize,
        const Spatial::Scalers2D& scalers)
    {
        if (viewSlice)
        {
            const auto viewSliceBox = viewSlice->box;
            const auto slice = Spatial::ToAxisAlignedBox2D(
                viewSliceBox.Left() / scalers.x,
                viewSliceBox.Top() / scalers.y,
                viewSliceBox.Right() / scalers.x,
                viewSliceBox.Bottom() / scalers.y);
            return { viewSliceBox.size, slice };
        }
        else
            return { boundsSize, Spatial::ToAxisAlignedBox2D(0, 0, baseSize.width, baseSize.height) };
    }

    void TextCurator::CalculateForText(Arca::RelicID id)
    {
        const auto text = Owner().Find<Matrix>(id);
        if (text)
        {
            const auto tuple = *text;

            const auto core = MutablePointer().Of(std::get<1>(tuple));
            auto font = MutablePointer().Of(core->font);
            const auto string = core->string;
            const auto wrapWidth = core->wrapWidth;
            const auto bold = core->bold;
            const auto italics = core->italics;

            if (font)
            {
                const auto data = manager->DataFor(*font->Resource(), string, wrapWidth, bold, italics);

                auto bounds = MutablePointer().Of(std::get<2>(tuple));
                bounds->BaseSize(data.size);

                if (data.size.width > 0 && data.size.height > 0)
                {
                    auto resource = graphicsManager->CreateTextResource(data.buffer, data.size);
                    core->resource = std::move(resource);
                }
                else
                    core->resource = {};
            }
        }
    }
}