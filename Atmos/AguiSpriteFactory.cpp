
#include "AguiSpriteFactory.h"
#include <AGUI\ImageResource.h>

#include <Inscription\Scribe.h>
#include <Inscription\String.h>

namespace Atmos
{
    void AguiSpriteFactory::Serialize(::Inscription::Scribe &scribe)
    {
        ::Inscription::TrackingChangerStack tracking(scribe, false);
        if (scribe.IsOutput())
        {
            scribe.Save(::Inscription::RemoveConst(imageName.GetValue()));
            scribe.Save(index);
            scribe.Save(color.alpha, color.red, color.green, color.blue);
            scribe.Save(z);
            scribe.WriteNumeric(xScaler);
            scribe.WriteNumeric(yScaler);
        }
        else
        {
            std::string str;
            scribe.Load(str);
            imageName = str;

            scribe.Load(index);
            scribe.Load(color.alpha, color.red, color.green, color.blue);
            scribe.Load(z);
            scribe.ReadNumeric(xScaler);
            scribe.ReadNumeric(yScaler);
        }
    }

    AguiSpriteFactory::AguiSpriteFactory(const Agui::FileName &imageName, ComponentT::Index index, const Agui::Color &color, ComponentT::Z z, Agui::Size::ValueT xScaler, Agui::Size::ValueT yScaler) : imageName(imageName), index(index), color(color), z(z), xScaler(xScaler), yScaler(yScaler)
    {}

    AguiSpriteFactory::AguiSpriteFactory(const ComponentT &arg) : imageName(arg.GetResource()->GetFileName()), index(arg.GetIndex()), color(arg.color), z(arg.GetZ()), xScaler(arg.GetSize().GetXScaler()), yScaler(arg.GetSize().GetYScaler())
    {}

    AguiSpriteFactory::AguiSpriteFactory(const Sprite &arg) : imageName(arg.GetMaterial()->GetXVisual()->GetFileName().c_str()), index(arg.GetIndex()), color(Agui::Color(arg.color.alpha, arg.color.red, arg.color.green, arg.color.blue)), z(arg.GetPosition().GetZ()), xScaler(arg.GetSize().GetXScaler()), yScaler(arg.GetSize().GetYScaler())
    {}

    void AguiSpriteFactory::Set(const ComponentT &arg)
    {
        imageName = arg.GetResource()->GetFileName();
        index = arg.GetIndex();
        color = arg.color;
        z = arg.GetZ();
        xScaler = arg.GetSize().GetXScaler();
        yScaler = arg.GetSize().GetYScaler();
    }

    void AguiSpriteFactory::Set(const Sprite &arg)
    {
        imageName = arg.GetMaterial()->GetXVisual()->GetFileName().c_str();
        index = arg.GetIndex();
        color = Agui::Color(arg.color.alpha, arg.color.red, arg.color.green, arg.color.blue);
        z = arg.GetPosition().GetZ();
        xScaler = arg.GetXScaler();
        yScaler = arg.GetYScaler();
    }

    void AguiSpriteFactory::Set(const Agui::FileName &imageName, ComponentT::Index index, const Agui::Color &color, ComponentT::Z z, Agui::Size::ValueT xScaler, Agui::Size::ValueT yScaler)
    {
        this->imageName = imageName;
        this->index = index;
        this->color = color;
        this->z = z;
        this->xScaler = xScaler;
        this->yScaler = yScaler;
    }

    Agui::SpriteComponent* AguiSpriteFactory::CreateComponent(const ComponentT::NameT &name, const Agui::RelativePosition &relativePosition) const
    {
        auto ret = Agui::SpriteComponent::Factory(name, z, relativePosition, Agui::Size(), Agui::Sprite(imageName, index, color));
        ret->ScaleWidth(xScaler);
        ret->ScaleHeight(yScaler);
        return ret;
    }

    Sprite AguiSpriteFactory::CreateAtmosSprite()
    {
        Sprite ret(imageName.c_str(), index, Atmos::Color(color.alpha, color.red, color.green, color.blue));
        ret.SetXScaler(xScaler);
        ret.SetYScaler(yScaler);
        return ret;
    }
}