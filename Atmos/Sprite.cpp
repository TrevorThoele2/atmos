
#include "Sprite.h"

#include "GridSize.h"

#include "GraphicsSystem.h"

#include "AguiSpriteFactory.h"
#include <AGUI/ImageResource.h>

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

    Sprite::Sprite(const ::Inscription::Table<Sprite>& table) :
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
        position.onValueChanged.Subscribe(&Sprite::OnPositionChanged, *this);
        material.onValueChanged.Subscribe(&Sprite::OnMaterialChanged, *this);
        index.onValueChanged.Subscribe(&Sprite::OnIndexChanged, *this);
    }

    void Sprite::CalculatePrimaryAssetSlice()
    {
        if (!material || !material->xVisual)
        {
            _primaryAssetSlice.SetTop(0);
            _primaryAssetSlice.SetBottom(0);
            _primaryAssetSlice.SetLeft(0);
            _primaryAssetSlice.SetRight(0);

            size.width.Set(0.0f);
            size.height.Set(0.0f);
            size.depth.Set(GRID_SIZE<float>);
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

        _primaryAssetSlice.SetTop(row * indexHeight);
        _primaryAssetSlice.SetBottom(row * indexHeight + indexHeight);
        _primaryAssetSlice.SetLeft(column * indexWidth);
        _primaryAssetSlice.SetRight(column * indexWidth + indexWidth);

        // Calculate size
        size.width.Set(static_cast<float>(material->width / material->columns));
        size.height.Set(static_cast<float>(material->height / material->rows));
        size.depth.Set(GRID_SIZE<float>);
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

    std::unique_ptr<Agui::SpriteComponent> CreateAguiSpriteComponent(
        const TypedObjectReference<Sprite> sprite,
        const Agui::FileName& imageName,
        const Agui::RelativePosition& relPosition)
    {
        if (!sprite->material.Get() || !sprite->material->xVisual.Get())
            return std::unique_ptr<Agui::SpriteComponent>();

        AguiSpriteFactory factory(sprite);
        return std::unique_ptr<Agui::SpriteComponent>(factory.CreateComponent(imageName, relPosition));
    }
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