#pragma once

#include <AGUI\SpriteComponent.h>
#include "Sprite.h"

#include "Serialization.h"

namespace Atmos
{
    class AguiSpriteFactory
    {
    public:
        typedef agui::SpriteComponent ComponentT;
    private:
        void Serialize(inscription::Scribe &scribe);
        INSCRIPTION_ACCESS;
    public:
        agui::FileName imageName;
        ComponentT::Index index;
        agui::Color color;
        ComponentT::Z z;
        agui::Size::ValueT xScaler;
        agui::Size::ValueT yScaler;

        AguiSpriteFactory(const agui::FileName &imageName = "", ComponentT::Index index = 1, const agui::Color &color = agui::Color(), ComponentT::Z z = 1.0f, agui::Size::ValueT xScaler = 1.0f, agui::Size::ValueT yScaler = 1.0f);
        AguiSpriteFactory(const AguiSpriteFactory &arg) = default;
        AguiSpriteFactory& operator=(const AguiSpriteFactory &arg) = default;

        AguiSpriteFactory(const ComponentT &arg);
        AguiSpriteFactory(const Sprite &arg);
        void Set(const agui::FileName &imageName, ComponentT::Index index = 1, const agui::Color &color = agui::Color(), ComponentT::Z z = 1.0f, agui::Size::ValueT xScaler = 1.0f, agui::Size::ValueT yScaler = 1.0f);
        void Set(const ComponentT &arg);
        void Set(const Sprite &arg);

        agui::SpriteComponent* CreateComponent(const ComponentT::NameT &name, const agui::RelativePosition &relativePosition = agui::RelativePosition()) const;
        Sprite CreateAtmosSprite();
    };
}