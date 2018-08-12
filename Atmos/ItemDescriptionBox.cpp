
#include "ItemDescriptionBox.h"

namespace Agui
{
    void ItemDescriptionBox::Setup()
    {
        image.Set(*Image::Factory(this, "itemImage", RelativePosition(HorizontalAlignment::MID, VerticalAlignment::MID), 1));
    }

    void ItemDescriptionBox::Refresh_impl()
    {
        if(item)
            image->SetSprite(::Atmos::CreateAguiSpriteComponent(item->portrait, "portrait").get());
    }

    void ItemDescriptionBox::GetNameString(std::string &string) const
    {
        string = (!item) ? "" : item->GetName();
    }

    void ItemDescriptionBox::GetDescriptionString(std::string &string) const
    {
        string = (!item) ? "" : item->GetDescription();
    }

    ItemDescriptionBox::ItemDescriptionBox(Object *parent, const std::string &name, const RelativePosition &pos, float z) : DescriptionBox(parent, name, pos, z), item(nullptr)
    {
        Setup();
    }

    ItemDescriptionBox::ItemDescriptionBox(Object *parent, const WidgetPrototype &arg) : DescriptionBox(parent, arg), item(nullptr)
    {
        Setup();
    }

    ItemDescriptionBox::ItemDescriptionBox(Object *parent, const ItemDescriptionBox &arg) : DescriptionBox(parent, arg), item(nullptr)
    {
        Setup();
    }

    ItemDescriptionBox* ItemDescriptionBox::Factory(Object *parent, const std::string &name, const RelativePosition &pos, float z)
    {
        return CreateBase<ItemDescriptionBox>(parent, name, pos, z);
    }

    ItemDescriptionBox* ItemDescriptionBox::Factory(Object *parent, const WidgetPrototype &arg)
    {
        return CreateBase<ItemDescriptionBox>(parent, arg);
    }

    ItemDescriptionBox* ItemDescriptionBox::Factory(Object *parent, const ItemDescriptionBox &arg)
    {
        return CreateBase<ItemDescriptionBox>(parent, arg);
    }

    void ItemDescriptionBox::Set()
    {
        item = nullptr;
        Refresh();
    }

    void ItemDescriptionBox::Set(const Atmos::Item &set)
    {
        this->item = &set;
        Refresh();
    }

    void ItemDescriptionBox::Set(const Atmos::ItemStack &set)
    {
        this->item = &*set.GetWrapped();
        Refresh();
    }

    WidgetSetup<ItemDescriptionBox>::WidgetSetup() : spriteFitter(false)
    {}
}