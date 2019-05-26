
#include "Sprite.h"

#include "GridSize.h"

#include "GraphicsSystem.h"

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>
#include <Inscription\Table.h>

namespace Atmos
{
    Sprite::Sprite(ObjectManager& manager) :
        RenderFragment(manager),
        index(0), primaryAssetSlice([this]() { return _primaryAssetSlice; })
    {
        SubscribeToProperties();
    }

    Sprite::Sprite(const Sprite& arg) :
        RenderFragment(arg),
        material(arg.material), patchShader(arg.patchShader), index(arg.index), color(arg.color),
        primaryAssetSlice([this]() { return _primaryAssetSlice; })
    {
        SubscribeToProperties();
    }

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(Sprite) :
        INSCRIPTION_TABLE_GET_BASE(RenderFragment),
        INSCRIPTION_TABLE_GET_MEM(material),
        INSCRIPTION_TABLE_GET_MEM(patchShader),
        INSCRIPTION_TABLE_GET_MEM(color),
        INSCRIPTION_TABLE_GET_MEM(index),
        primaryAssetSlice([this]() { return _primaryAssetSlice; })
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
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Sprite)
    {
        INSCRIPTION_TABLE_ADD(material);
        INSCRIPTION_TABLE_ADD(patchShader);
        INSCRIPTION_TABLE_ADD(index);
        INSCRIPTION_TABLE_ADD(color);
    }
}