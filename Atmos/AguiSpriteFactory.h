#pragma once

#include <AGUI\SpriteComponent.h>
#include "Sprite.h"

#include "Serialization.h"

namespace Atmos
{
    class AguiSpriteFactory
    {
    public:
        typedef Agui::SpriteComponent ComponentT;
    public:
        Agui::FileName resourceName;
        ComponentT::Index index;
        Agui::Color color;
        ComponentT::Z z;
        Agui::Size::ValueT widthScaler;
        Agui::Size::ValueT heightScaler;
    public:
        AguiSpriteFactory(
            const Agui::FileName& xVisualName = "",
            ComponentT::Index index = 1,
            const Agui::Color& color = Agui::Color(),
            ComponentT::Z z = 1.0f,
            Agui::Size::ValueT widthScaler = 1.0f,
            Agui::Size::ValueT heightScaler = 1.0f);
        AguiSpriteFactory(const AguiSpriteFactory& arg) = default;
        AguiSpriteFactory& operator=(const AguiSpriteFactory& arg) = default;

        AguiSpriteFactory(const ComponentT& arg);
        AguiSpriteFactory(TypedObjectReference<nSprite> arg);
        void Set(const Agui::FileName& xVisualName,
            ComponentT::Index index = 1,
            const Agui::Color& color = Agui::Color(),
            ComponentT::Z z = 1.0f,
            Agui::Size::ValueT widthScaler = 1.0f,
            Agui::Size::ValueT heightScaler = 1.0f);
        void Set(const ComponentT& arg);
        void Set(const TypedObjectReference<nSprite>& arg);

        Agui::SpriteComponent* CreateComponent(const ComponentT::NameT& name, const Agui::RelativePosition& relativePosition = Agui::RelativePosition()) const;
        TypedObjectReference<nSprite> CreateAtmosSprite(ObjectManager& objectManager);
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}