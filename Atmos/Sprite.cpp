
#include "Sprite.h"

#include "Environment.h"
#include "CurrentField.h"
#include "GridSize.h"

#include "AguiSpriteFactory.h"
#include <AGUI\ImageResource.h>

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>
#include <Inscription\Table.h>

namespace Atmos
{
    nSprite::nSprite() :
        index(0), primaryAssetSlice([this]() { return _primaryAssetSlice; })
    {
        SubscribeToProperties();
    }

    nSprite::nSprite(const nSprite& arg) :
        material(arg.material), patchShader(arg.patchShader), index(arg.index), color(arg.color),
        primaryAssetSlice([this]() { return _primaryAssetSlice; })
    {
        SubscribeToProperties();
    }

    nSprite::nSprite(const ::Inscription::Table<nSprite>& table) :
        INSCRIPTION_TABLE_GET_BASE(nRenderFragment),
        INSCRIPTION_TABLE_GET_MEM(material),
        INSCRIPTION_TABLE_GET_MEM(patchShader),
        INSCRIPTION_TABLE_GET_MEM(color),
        INSCRIPTION_TABLE_GET_MEM(index),
        primaryAssetSlice([this]() { return _primaryAssetSlice; })
    {
        SubscribeToProperties();
    }

    ObjectTypeDescription nSprite::TypeDescription() const
    {
        return ObjectTraits<nSprite>::TypeDescription();
    }

    void nSprite::DrawImpl()
    {
        Environment::GetGraphics()->RenderSprite(*this);
    }

    void nSprite::SubscribeToProperties()
    {
        position.onValueChanged.Subscribe(&nSprite::OnPositionChanged, *this);
        material.onValueChanged.Subscribe(&nSprite::OnMaterialChanged, *this);
        index.onValueChanged.Subscribe(&nSprite::OnIndexChanged, *this);
    }

    void nSprite::CalculatePrimaryAssetSlice()
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

    void nSprite::OnPositionChanged(Position3D newValue)
    {
        CalculatePrimaryAssetSlice();
    }

    void nSprite::OnMaterialChanged(MaterialReference newValue)
    {
        CalculatePrimaryAssetSlice();
    }

    void nSprite::OnIndexChanged(Index newValue)
    {
        CalculatePrimaryAssetSlice();
    }

    const ObjectTypeName ObjectTraits<nSprite>::typeName = "Sprite";

    namespace Modulator
    {
        LinkGeneratorGroup<nSprite>::Generator<nSprite::Index> LinkGeneratorGroup<nSprite>::index(
            0,
            [](TypedObjectReference<nSprite> sprite) { return Value(std::int64_t(sprite->index.Get())); },
            [](TypedObjectReference<nSprite> sprite, nSprite::Index newValue) { sprite->index = newValue; });

        LinkGeneratorGroup<nSprite>::Generator<Color::ValueT> LinkGeneratorGroup<nSprite>::colorA(
            1,
            [](TypedObjectReference<nSprite> sprite) { return Value(std::int64_t(sprite->color.Get().alpha)); },
            [](TypedObjectReference<nSprite> sprite, Color::ValueT newValue) { sprite->color.Get().alpha = newValue; });

        LinkGeneratorGroup<nSprite>::Generator<Color::ValueT> LinkGeneratorGroup<nSprite>::colorR(
            2,
            [](TypedObjectReference<nSprite> sprite) { return Value(std::int64_t(sprite->color.Get().red)); },
            [](TypedObjectReference<nSprite> sprite, Color::ValueT newValue) { sprite->color.Get().red = newValue; });

        LinkGeneratorGroup<nSprite>::Generator<Color::ValueT> LinkGeneratorGroup<nSprite>::colorG(
            3,
            [](TypedObjectReference<nSprite> sprite) { return Value(std::int64_t(sprite->color.Get().green)); },
            [](TypedObjectReference<nSprite> sprite, Color::ValueT newValue) { sprite->color.Get().green = newValue; });

        LinkGeneratorGroup<nSprite>::Generator<Color::ValueT> LinkGeneratorGroup<nSprite>::colorB(
            4,
            [](TypedObjectReference<nSprite> sprite) { return Value(std::int64_t(sprite->color.Get().blue)); },
            [](TypedObjectReference<nSprite> sprite, Color::ValueT newValue) { sprite->color.Get().blue = newValue; });

        LinkGeneratorGroup<nSprite>::LinkGeneratorGroup()
        {
            RegisterGenerator(index);
            RegisterGenerator(colorA);
            RegisterGenerator(colorR);
            RegisterGenerator(colorG);
            RegisterGenerator(colorB);
        }
    }

    std::unique_ptr<Agui::SpriteComponent> CreateAguiSpriteComponent(
        const TypedObjectReference<nSprite> sprite,
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
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::nSprite)
    {
        INSCRIPTION_TABLE_ADD(material);
        INSCRIPTION_TABLE_ADD(patchShader);
        INSCRIPTION_TABLE_ADD(index);
        INSCRIPTION_TABLE_ADD(color);
    }
}