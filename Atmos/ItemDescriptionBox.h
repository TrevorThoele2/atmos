
#pragma once

#include "DescriptionBox.h"
#include "ItemStack.h"

#include <AGUI\Image.h>

namespace agui
{
    class ItemDescriptionBox : public DescriptionBox
    {
    private:
        WidgetComponent<Image> image;
        const Atmos::Item *item;

        ItemDescriptionBox(const ItemDescriptionBox &arg) = delete;
        void Setup();

        void Refresh_impl() override;
        void GetNameString(std::string &string) const override;
        void GetDescriptionString(std::string &string) const override;
    protected:
        ItemDescriptionBox(Object *parent, const std::string &name, const RelativePosition &pos, float z);
        ItemDescriptionBox(Object *parent, const WidgetPrototype &arg);
        ItemDescriptionBox(Object *parent, const ItemDescriptionBox &arg);
    public:
        static ItemDescriptionBox* Factory(Object *parent, const std::string &name, const RelativePosition &pos, float z);
        static ItemDescriptionBox* Factory(Object *parent, const WidgetPrototype &arg);
        static ItemDescriptionBox* Factory(Object *parent, const ItemDescriptionBox &arg);
        void Set();
        void Set(const Atmos::Item &set);
        void Set(const Atmos::ItemStack &set);
        WIDGET_COMMON_ACCESS(ItemDescriptionBox);
    };

    template<>
    class WidgetSetup<ItemDescriptionBox> : public WidgetSetupBase<ItemDescriptionBox>
    {
    private:
        void Setup(ItemDescriptionBox &obj) const override
        {
            SetupOptional(obj, &ItemDescriptionBox::CreateSprite<const SpriteComponent&>, sprite);
            SetupOptional(obj, &ItemDescriptionBox::CreateSpriteFitter<>, spriteFitter);
            obj.Init();
        }
    public:
        Attribute<SpriteComponent> sprite;
        bool spriteFitter;

        WidgetSetup();
    };

    typedef WidgetSetup<ItemDescriptionBox> ItemDescriptionBoxSetup;
}