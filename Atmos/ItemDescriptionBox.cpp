
#include "ItemDescriptionBox.h"

namespace Agui
{
    void ItemDescriptionBox::Setup()
    {
        image.Set(*Image::Factory(this, "itemImage", RelativePosition(HorizontalAlignment::MID, VerticalAlignment::MID), 1));
    }

    void ItemDescriptionBox::Refresh_impl()
    {
        if (focusedItem)
            image->SetSprite(::Atmos::CreateAguiSpriteComponent(focusedItem->portrait, "portrait").get());
    }

    void ItemDescriptionBox::GetNameString(std::string &string) const
    {
        string = (!focusedItem) ? "" : focusedItem->name.Get();
    }

    void ItemDescriptionBox::GetDescriptionString(std::string &string) const
    {
        string = (!focusedItem) ? "" : focusedItem->description;
    }

    ItemDescriptionBox::ItemDescriptionBox(Object *parent, const std::string &name, const RelativePosition &pos, float z) :
        DescriptionBox(parent, name, pos, z), focusedItem()
    {
        Setup();
    }

    ItemDescriptionBox::ItemDescriptionBox(Object *parent, const WidgetPrototype &arg) :
        DescriptionBox(parent, arg), focusedItem()
    {
        Setup();
    }

    ItemDescriptionBox::ItemDescriptionBox(Object *parent, const ItemDescriptionBox &arg) :
        DescriptionBox(parent, arg), focusedItem()
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
        focusedItem.Reset();
        Refresh();
    }

    void ItemDescriptionBox::Set(ItemReference set)
    {
        focusedItem = set;
        Refresh();
    }

    void ItemDescriptionBox::Set(const Atmos::ItemStack &set)
    {
        focusedItem = set.itemSource;
        Refresh();
    }

    WidgetSetup<ItemDescriptionBox>::WidgetSetup() : spriteFitter(false)
    {}
}