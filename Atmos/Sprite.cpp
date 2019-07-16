#include "Sprite.h"

#include "GridSize.h"

#include "GraphicsSystem.h"

namespace Atmos
{
    Sprite::Sprite(ObjectManager& manager) :
        Fragment(manager), index(0)
    {
        SubscribeToProperties();
    }

    Sprite::Sprite(const Sprite& arg) :
        Fragment(arg), material(arg.material), patchShader(arg.patchShader),
        index(arg.index), color(arg.color)
    {
        SubscribeToProperties();
    }

    Sprite::Sprite(const ::Inscription::BinaryTableData<Sprite>& data) :
        Fragment(std::get<0>(data.bases)), material(data.material), patchShader(data.patchShader),
        color(data.color), index(data.index)
    {
        SubscribeToProperties();
    }

    ObjectTypeDescription Sprite::TypeDescription() const
    {
        return ObjectTraits<Sprite>::TypeDescription();
    }

    void Sprite::DrawImpl()
    {
        auto graphics = Manager()->FindSystem<GraphicsSystem>();
        graphics->Get()->RenderSprite(*this);
    }

    void Sprite::SubscribeToProperties()
    {
        onPositionChanged.Subscribe(&Sprite::OnPositionChanged, *this);
        material.onValueChanged.Subscribe(&Sprite::OnMaterialChanged, *this);
        index.onValueChanged.Subscribe(&Sprite::OnIndexChanged, *this);
    }

    void Sprite::CalculatePrimaryAssetSlice()
    {
        if (!material || !material->xVisual)
        {
            _primaryAssetSlice.top = 0;
            _primaryAssetSlice.bottom = 0;
            _primaryAssetSlice.left = 0;
            _primaryAssetSlice.right = 0;

            InformBaseSizeChanged(0.0f, 0.0f, GRID_SIZE<float>);
            return;
        }

        auto columns(material->columns);
        auto rows(material->rows);

        auto column = index % columns;
        if (column == 0)
            column = columns;
        --column;

        auto row = static_cast<int>(std::ceil(static_cast<float>(index) / static_cast<float>(columns)));
        --row;

        auto indexWidth = static_cast<float>(material->width / columns);
        auto indexHeight = static_cast<float>(material->height / rows);

        _primaryAssetSlice.top = row * indexHeight;
        _primaryAssetSlice.bottom = row * indexHeight + indexHeight;
        _primaryAssetSlice.left = column * indexWidth;
        _primaryAssetSlice.right = column * indexWidth + indexWidth;

        // Calculate size
        auto width = static_cast<float>(material->width / material->columns);
        auto height = static_cast<float>(material->height / material->rows);
        auto depth = GRID_SIZE<float>;
        InformBaseSizeChanged(width, height, depth);
    }

    void Sprite::OnPositionChanged(Position3D newValue)
    {
        CalculatePrimaryAssetSlice();
    }

    void Sprite::OnMaterialChanged(MaterialReference newValue)
    {
        CalculatePrimaryAssetSlice();
    }

    void Sprite::OnIndexChanged(Index newValue)
    {
        CalculatePrimaryAssetSlice();
    }

    const ObjectTypeName ObjectTraits<Sprite>::typeName = "Sprite";
}

namespace Inscription
{
    Scribe<::Atmos::Sprite, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::material, &DataT::material),
            DataEntry::Auto(&ObjectT::patchShader, &DataT::patchShader),
            DataEntry::Auto(&ObjectT::index, &DataT::index),
            DataEntry::Auto(&ObjectT::color, &DataT::color) });
    }
}