
#pragma once

#include "DescriptionBox.h"

namespace Atmos { class Spell; }
namespace agui
{
    class SpellDescriptionBox : public DescriptionBox
    {
    private:
        const Atmos::Spell *selected;

        SpellDescriptionBox(const SpellDescriptionBox &arg) = delete;

        void Refresh_impl() override {}
        void GetNameString(::std::string &string) const;
        void GetDescriptionString(::std::string &string) const;
    protected:
        SpellDescriptionBox(Object *parent, const std::string &name, const RelativePosition &pos, float z);
        SpellDescriptionBox(Object *parent, const WidgetPrototype &arg);
        SpellDescriptionBox(Object *parent, const SpellDescriptionBox &arg);
    public:
        static SpellDescriptionBox* Factory(Object *parent, const std::string &name, const RelativePosition &pos, float z);
        static SpellDescriptionBox* Factory(Object *parent, const WidgetPrototype &arg);
        static SpellDescriptionBox* Factory(Object *parent, const SpellDescriptionBox &arg);
        void Set();
        void Set(const Atmos::Spell &set);
        WIDGET_COMMON_ACCESS(SpellDescriptionBox);
    };

    template<>
    class WidgetSetup<SpellDescriptionBox> : public WidgetSetupBase<SpellDescriptionBox>
    {
    private:
        void Setup(SpellDescriptionBox &obj) const override
        {
            SetupOptional(obj, &SpellDescriptionBox::CreateSprite<const SpriteComponent&>, sprite);
            SetupOptional(obj, &SpellDescriptionBox::CreateSpriteFitter<>, spriteFitter);
            obj.Init();
        }
    public:
        Attribute<SpriteComponent> sprite;
        bool spriteFitter;

        WidgetSetup();
    };

    typedef WidgetSetup<SpellDescriptionBox> SpellDescriptionBoxSetup;
}