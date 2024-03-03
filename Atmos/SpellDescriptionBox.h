
#pragma once

#include "DescriptionBox.h"

#include "ObjectReference.h"
#include "Spell.h"

namespace Agui
{
    class SpellDescriptionBox : public DescriptionBox
    {
    private:
        typedef ::Atmos::TypedObjectReference<::Atmos::nSpell> SpellReference;
        ::Atmos::TypedObjectReference<::Atmos::nSpell> focusedSpell;

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
        void Set(SpellReference set);
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
        ComponentAttribute<SpriteComponent> sprite;
        bool spriteFitter;

        WidgetSetup();
    };

    typedef WidgetSetup<SpellDescriptionBox> SpellDescriptionBoxSetup;
}