#include "TextCurator.h"

#include "RenderText.h"
#include "UpdateText.h"

#include "ColorChanged.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Render
{
    TextCurator::TextCurator(Init init, TextManager& manager, GraphicsManager& graphicsManager) :
        ObjectCurator(init), manager(&manager), graphicsManager(&graphicsManager)
    {
        Owner().On<Arca::MatrixFormed<Matrix>>([this](const Arca::MatrixFormed<Matrix>& signal)
        {
            ChangeGraphics(*signal.index.Get());
            matrices.emplace(signal.index.ID(), signal.index);
        });

        Owner().On<Arca::MatrixDissolved<Matrix>>([this](const Arca::MatrixDissolved<Matrix>& signal)
        {
            matrices.erase(signal.index.ID());
        });

        Owner().On<ColorChanged>([this](const ColorChanged& signal)
        {
            const auto matrix = matrices.find(signal.id);
            if (matrix != matrices.end())
                ChangeGraphics(*matrix->second.Get());
        });
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
            {
                const auto matrix = Owner().Find<Matrix>(command.id);
                if (matrix)
                    ChangeGraphics(*matrix.Get());
            }
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
        
        for (auto index = indices.begin(); index != indices.end(); ++index)
            StageRender(index.ID(), *index, cameraTopLeft, mainSurface);
    }

    void TextCurator::StageRender(
        Arca::RelicID id, const Matrix::ReferenceTuple& tuple, Spatial::Point2D cameraTopLeft, const MainSurface& mainSurface)
    {
        const auto& renderCore = *std::get<0>(tuple);
        const auto& core = *std::get<1>(tuple);
        const auto& bounds = *std::get<2>(tuple);

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

            const RenderText render
            {
                resource,
                slice,
                material,
                position,
                size,
                rotation,
                color,
                ToRenderSpace(boundsSpace),
                mainSurface.Resource()
            };
            graphicsManager->Stage(render);
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

    void TextCurator::ChangeGraphics(const Matrix::ReferenceTuple& tuple)
    {
        const auto core = MutablePointer().Of(std::get<1>(tuple));
        const auto font = MutablePointer().Of(core->font);
        const auto string = core->string;
        const auto wrapWidth = core->wrapWidth;
        const auto bold = core->bold;
        const auto italics = core->italics;

        if (font)
        {
            const auto [buffer, size] = manager->DataFor(*font->Resource(), string, wrapWidth, bold, italics);

            const auto bounds = MutablePointer().Of(std::get<2>(tuple));
            bounds->BaseSize(size);

            if (buffer.empty() || size.width == 0 || size.height == 0)
                core->resource = {};
            else if (!core->resource)
            {
                auto resource = graphicsManager->CreateTextResource(buffer, size);
                core->resource = std::move(resource);
            }
            else
                graphicsManager->Stage(UpdateText{core->resource.get(), buffer, size});
        }
    }
}