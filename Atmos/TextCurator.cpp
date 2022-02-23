#include "TextCurator.h"

#include "RenderText.h"
#include "StagedRenders.h"
#include "ColorChanged.h"
#include "TextBaseSize.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Render
{
    TextCurator::TextCurator(Init init, TextManager& manager) :
        ObjectCurator(init), manager(&manager)
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
    
    void TextCurator::WorkImpl(
        Spatial::AxisAlignedBox3D cameraBox,
        Spatial::Point2D cameraTopLeft,
        const MainSurface& mainSurface)
    {
        std::vector<RenderText> renders;
        renders.reserve(matrices.size());
        for (auto& matrix : matrices)
        {
            const auto render = RenderOf(matrix.second.ID(), *matrix.second, cameraTopLeft, mainSurface);
            if (render)
                renders.push_back(*render);
        }

        const auto stagedRenders = MutablePointer().Of<StagedRenders>();
        stagedRenders->texts.insert(stagedRenders->texts.end(), renders.begin(), renders.end());
    }

    std::optional<RenderText> TextCurator::RenderOf(
        Arca::RelicID id,
        const Matrix::ReferenceTuple& tuple,
        Spatial::Point2D cameraTopLeft,
        const MainSurface& mainSurface)
    {
        const auto& renderCore = *std::get<0>(tuple);
        const auto& core = *std::get<1>(tuple);
        const auto& bounds = *std::get<2>(tuple);
        const auto font = core.font.Get();
        const auto material = renderCore.material;
        if (font && material && font->Resource())
        {
            const auto boundsSpace = bounds.Space();
            
            return RenderText
            {
                .string = core.string,
                .fontResource = const_cast<Asset::Resource::Font*>(font->Resource()),
                .viewSlice = ViewSliceBox(Owner().Find<ViewSlice>(id)),
                .material = material,
                .position = ToRenderPoint(bounds.Position(), cameraTopLeft, boundsSpace),
                .rotation = bounds.Rotation(),
                .scalers = bounds.Scalers(),
                .color = renderCore.color,
                .bold = core.bold,
                .italics = core.italics,
                .wrapWidth = core.wrapWidth,
                .space = ToRenderSpace(boundsSpace),
                .surface = mainSurface.Resource()
            };
        }
        else
            return {};
    }

    void TextCurator::AttemptChangeBaseSize(const Matrix::ReferenceTuple& tuple)
    {
        const auto core = MutablePointer().Of(std::get<1>(tuple));
        const auto string = core->string;
        const auto wrapWidth = core->wrapWidth;
        const auto bold = core->bold;
        const auto italics = core->italics;
        const auto bounds = MutablePointer().Of(std::get<2>(tuple));
        bounds->BaseSize(core->font
            ? Owner().Do(TextBaseSize(string, core->font.ID(), bold, italics, wrapWidth))
            : Spatial::Size2D{});
    }
}
