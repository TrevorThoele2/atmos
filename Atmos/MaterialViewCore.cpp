#include "MaterialViewCore.h"

#include "TileSize.h"

namespace Atmos::Render
{
    void MaterialViewCore::Material(Arca::RelicIndex<Asset::MaterialAsset> to)
    {
        material = to;
    }

    Arca::RelicIndex<Asset::MaterialAsset> MaterialViewCore::Material() const
    {
        return material;
    }

    void MaterialViewCore::MaterialIndex(Index to)
    {
        materialIndex = to;
    }

    auto MaterialViewCore::MaterialIndex() const -> Index
    {
        return materialIndex;
    }

    AxisAlignedBox2D MaterialViewCore::MaterialSlice() const
    {
        return materialSlice;
    }

    void MaterialViewCore::Color(Render::Color to)
    {
        color = to;
    }

    Render::Color MaterialViewCore::Color() const
    {
        return color;
    }

    void MaterialViewCore::PatchShader(Arca::RelicIndex<Asset::ShaderAsset> to)
    {
        patchShader = to;
    }

    Arca::RelicIndex<Asset::ShaderAsset> MaterialViewCore::PatchShader() const
    {
        return patchShader;
    }

    void MaterialViewCore::CalculateMaterialSlice()
    {
        if (!material || !material->Image())
        {
            materialSlice.Top(0);
            materialSlice.Bottom(0);
            materialSlice.Left(0);
            materialSlice.Right(0);
            return;
        }

        const auto columns = material->Columns();
        const auto rows = material->Rows();

        auto column = materialIndex % columns;
        if (column == 0)
            column = columns;
        --column;

        auto row = static_cast<int>(std::ceil(static_cast<float>(materialIndex) / static_cast<float>(columns)));
        --row;

        const auto indexWidth = static_cast<float>(material->Image()->Width() / columns);
        const auto indexHeight = static_cast<float>(material->Image()->Height() / rows);

        materialSlice.Top(row * indexHeight);
        materialSlice.Bottom(row * indexHeight + indexHeight);
        materialSlice.Left(column * indexWidth);
        materialSlice.Right(column * indexWidth + indexWidth);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Render::MaterialViewCore, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        
    }
}