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
    private:
        void Serialize(::Inscription::Scribe &scribe);
        INSCRIPTION_ACCESS;
    public:
        Agui::FileName imageName;
        ComponentT::Index index;
        Agui::Color color;
        ComponentT::Z z;
        Agui::Size::ValueT xScaler;
        Agui::Size::ValueT yScaler;

        AguiSpriteFactory(const Agui::FileName &imageName = "", ComponentT::Index index = 1, const Agui::Color &color = Agui::Color(), ComponentT::Z z = 1.0f, Agui::Size::ValueT xScaler = 1.0f, Agui::Size::ValueT yScaler = 1.0f);
        AguiSpriteFactory(const AguiSpriteFactory &arg) = default;
        AguiSpriteFactory& operator=(const AguiSpriteFactory &arg) = default;

        AguiSpriteFactory(const ComponentT &arg);
        AguiSpriteFactory(const Sprite &arg);
        void Set(const Agui::FileName &imageName, ComponentT::Index index = 1, const Agui::Color &color = Agui::Color(), ComponentT::Z z = 1.0f, Agui::Size::ValueT xScaler = 1.0f, Agui::Size::ValueT yScaler = 1.0f);
        void Set(const ComponentT &arg);
        void Set(const Sprite &arg);

        Agui::SpriteComponent* CreateComponent(const ComponentT::NameT &name, const Agui::RelativePosition &relativePosition = Agui::RelativePosition()) const;
        Sprite CreateAtmosSprite();
    };
}