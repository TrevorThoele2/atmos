#include "TextCurator.h"

#include "RenderText.h"

#include "ColorChanged.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Render
{
    TextCurator::TextCurator(Init init, TextManager& manager, GraphicsManager& graphicsManager) :
        ObjectCurator(init), manager(&manager), graphicsManager(&graphicsManager)
    {
        Owner().On<Arca::MatrixFormed<Matrix>>([this](const Arca::MatrixFormed<Matrix>& signal)
        {
            AttemptChangeBaseSize(*signal.index.Get());
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
                AttemptChangeBaseSize(*matrix->second.Get());
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
                    AttemptChangeBaseSize(*matrix.Get());
            }
        }
    }
    
    std::unique_ptr<Asset::Resource::Font> TextCurator::Handle(const Asset::Resource::Create<Asset::Resource::Font>& command)
    {
        return manager->CreateFontResource(command.buffer, command.name);
    }

    Spatial::Size2D TextCurator::Handle(const TextBaseSize& command)
    {
        const auto font = Owner().Find<Asset::Font>(command.font);
        if (!font)
            return {};

        auto& fontResource = *const_cast<Asset::Resource::Font*>(font->Resource());
        return graphicsManager->TextBaseSize(
            command.string,
            fontResource,
            command.bold,
            command.italics,
            command.wrapWidth);
    }

    void TextCurator::WorkImpl(
        Spatial::AxisAlignedBox3D cameraBox,
        Spatial::Point2D cameraTopLeft,
        const MainSurface& mainSurface)
    {
        for (auto& matrix : matrices)
            StageRender(matrix.second.ID(), *matrix.second, cameraTopLeft, mainSurface);
    }

    void TextCurator::StageRender(
        Arca::RelicID id, const Matrix::ReferenceTuple& tuple, Spatial::Point2D cameraTopLeft, const MainSurface& mainSurface)
    {
        const auto& renderCore = *std::get<0>(tuple);
        const auto& core = *std::get<1>(tuple);
        const auto& bounds = *std::get<2>(tuple);

        const auto font = core.font.Get();
        const auto material = renderCore.material;
        if (font && material && font->Resource())
        {
            const auto string = core.string;
            const auto boundsSpace = bounds.Space();
            const auto position = ToRenderPoint(bounds.Position(), cameraTopLeft, boundsSpace);
            const auto baseSize = bounds.BaseSize();
            const auto rotation = bounds.Rotation();
            const auto color = renderCore.color;
            const auto bold = core.bold;
            const auto italics = core.italics;
            const auto wrapWidth = core.wrapWidth;

            const auto resource = const_cast<Asset::Resource::Font*>(font->Resource());

            const auto scalers = bounds.Scalers();
            const auto size = bounds.Size();
            const auto standardBox = Spatial::ToAxisAlignedBox2D(0, 0, size.width, size.height);
            const auto viewSlice = ViewSliceClamp(Owner().Find<ViewSlice>(id), standardBox);
            const auto slice = Spatial::ScaleOf(viewSlice, standardBox);

            const RenderText render
            {
                string,
                resource,
                slice,
                material,
                position,
                rotation,
                scalers,
                color,
                bold,
                italics,
                wrapWidth,
                ToRenderSpace(boundsSpace),
                mainSurface.Resource()
            };
            graphicsManager->Stage(render);
        }
    }

    void TextCurator::AttemptChangeBaseSize(const Matrix::ReferenceTuple& tuple)
    {
        const auto core = MutablePointer().Of(std::get<1>(tuple));
        const auto font = MutablePointer().Of(core->font);
        const auto string = core->string;
        const auto wrapWidth = core->wrapWidth;
        const auto bold = core->bold;
        const auto italics = core->italics;
        const auto bounds = MutablePointer().Of(std::get<2>(tuple));
        bounds->BaseSize(font
            ? graphicsManager->TextBaseSize(string, *font->Resource(), bold, italics, wrapWidth)
            : Spatial::Size2D{});
    }
}