#include "TextCurator.h"

#include "StagedRasters.h"
#include "ColorChanged.h"
#include "RenderAlgorithms.h"
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
        std::vector<Raster::Prepared<Raster::Text>> rasters;
        rasters.reserve(matrices.size());
        for (auto& matrix : matrices)
        {
            const auto raster = Raster(matrix.second.ID(), *matrix.second, cameraTopLeft, mainSurface);
            if (raster)
                rasters.push_back(*raster);
        }

        const auto stagedRasters = MutablePointer().Of<Raster::Staged>();
        stagedRasters->texts.insert(stagedRasters->texts.end(), rasters.begin(), rasters.end());
    }

    std::optional<Raster::Prepared<Raster::Text>> TextCurator::Raster(
        Arca::RelicID id,
        const Matrix::ReferenceTuple& tuple,
        Spatial::Point2D cameraTopLeft,
        const MainSurface& mainSurface)
    {
        const auto& renderCore = *std::get<0>(tuple);
        const auto& core = *std::get<1>(tuple);
        const auto& bounds = *std::get<2>(tuple);
        const auto font = core.font;
        const auto material = renderCore.material;
        if (font && material && font->Resource())
        {
            const auto boundsSpace = bounds.Space();
            const auto position = ToRenderPoint(bounds.Position(), cameraTopLeft, boundsSpace);

            return std::tuple
            {
                Raster::Text
                {
                    .material = material,
                    .string = core.string,
                    .font = font,
                    .viewSlice = ViewSliceBox(Owner().Find<ViewSlice>(id)),
                    .color = renderCore.color,
                    .bold = core.bold,
                    .italics = core.italics,
                    .wrapWidth = core.wrapWidth,
                    .position = ToPoint2D(position),
                    .rotation = bounds.Rotation(),
                    .scalers = bounds.Scalers()
                },
                mainSurface.Resource(),
                Raster::Order
                {
                    .space = Ordering(boundsSpace),
                    .z = position.z
                }
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
