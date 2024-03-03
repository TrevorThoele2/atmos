
#include "AguiSpriteFactory.h"

#include <AGUI/ImageResource.h>

#include "MaterialAssetSystem.h"

#include <Inscription/Scribe.h>
#include <Inscription/String.h>

namespace Atmos
{
    AguiSpriteFactory::AguiSpriteFactory(
        const Agui::FileName& resourceName,
        ComponentT::Index index,
        const Agui::Color& color,
        ComponentT::Z z,
        Agui::Size::ValueT widthScaler,
        Agui::Size::ValueT heightScaler) :
        
        resourceName(resourceName), index(index), color(color), z(z), widthScaler(widthScaler), heightScaler(heightScaler)
    {}

    AguiSpriteFactory::AguiSpriteFactory(const ComponentT& arg) :
        resourceName(arg.GetResource()->GetFileName()), index(arg.GetIndex()), color(arg.color),
        z(arg.GetZ()), widthScaler(arg.GetSize().GetXScaler()), heightScaler(arg.GetSize().GetYScaler())
    {}

    AguiSpriteFactory::AguiSpriteFactory(TypedObjectReference<Sprite> arg) :
        resourceName(arg->material->xVisual->fileName.c_str()), index(arg->index),
        color(Agui::Color(arg->color.Get().alpha, arg->color.Get().red, arg->color.Get().green, arg->color.Get().blue)),
        z(arg->position.z), widthScaler(arg->size.widthScaler), heightScaler(arg->size.heightScaler)
    {}

    void AguiSpriteFactory::Set(const ComponentT& arg)
    {
        resourceName = arg.GetResource()->GetFileName();
        index = arg.GetIndex();
        color = arg.color;
        z = arg.GetZ();
        widthScaler = arg.GetSize().GetXScaler();
        heightScaler = arg.GetSize().GetYScaler();
    }

    void AguiSpriteFactory::Set(const TypedObjectReference<Sprite>& arg)
    {
        resourceName = arg->material->xVisual->fileName.c_str();
        index = arg->index;
        color = Agui::Color(arg->color.Get().alpha, arg->color.Get().red, arg->color.Get().green, arg->color.Get().blue);
        z = arg->position.z;
        widthScaler = arg->size.widthScaler;
        heightScaler = arg->size.heightScaler;
    }

    void AguiSpriteFactory::Set(
        const Agui::FileName& resourceName,
        ComponentT::Index index,
        const Agui::Color& color,
        ComponentT::Z z,
        Agui::Size::ValueT widthScaler,
        Agui::Size::ValueT heightScaler)
    {
        this->resourceName = resourceName;
        this->index = index;
        this->color = color;
        this->z = z;
        this->widthScaler = widthScaler;
        this->heightScaler = heightScaler;
    }

    Agui::SpriteComponent* AguiSpriteFactory::CreateComponent(const ComponentT::NameT& name, const Agui::RelativePosition& relativePosition) const
    {
        auto ret = Agui::SpriteComponent::Factory(name, z, relativePosition, Agui::Size(), Agui::Sprite(resourceName, index, color));
        ret->ScaleWidth(widthScaler);
        ret->ScaleHeight(heightScaler);
        return ret;
    }

    TypedObjectReference<Sprite> AguiSpriteFactory::CreateAtmosSprite(ObjectManager& objectManager)
    {
        auto ret = objectManager.CreateObject<Sprite>();
        ret->material = objectManager.FindSystem<MaterialAssetSystem>()->FindAsset(resourceName);
        ret->index = index;
        ret->color = Atmos::Color(color.alpha, color.red, color.green, color.blue);
        ret->size.widthScaler = widthScaler;
        ret->size.heightScaler = heightScaler;
        return ret;
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AguiSpriteFactory)
    {
        ::Inscription::TrackingChangerStack tracking(scribe, false);
        if (scribe.IsOutput())
        {
            scribe.Save(::Inscription::RemoveConst(resourceName.GetValue()));
            scribe.Save(index);
            scribe.Save(color.alpha);
            scribe.Save(color.red);
            scribe.Save(color.green);
            scribe.Save(color.blue);
            scribe.Save(z);
            scribe.WriteNumeric(widthScaler);
            scribe.WriteNumeric(heightScaler);
        }
        else
        {
            std::string str;
            scribe.Load(str);
            resourceName = str;

            scribe.Load(index);
            scribe.Load(color.alpha);
            scribe.Load(color.red);
            scribe.Load(color.green);
            scribe.Load(color.blue);
            scribe.Load(z);
            scribe.ReadNumeric(widthScaler);
            scribe.ReadNumeric(heightScaler);
        }
    }
}