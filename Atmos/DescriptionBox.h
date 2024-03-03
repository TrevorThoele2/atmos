
#pragma once

#include <string>

#include <AGUI\SpriteWidget.h>

namespace Agui
{
    class TextComponent;
    class DescriptionBox : public SpriteWidgetSync
    {
    private:
        TextComponent *name, *description;

        void Setup();
        virtual void Refresh_impl() = 0;
        virtual void GetNameString(std::string &string) const = 0;
        virtual void GetDescriptionString(std::string &string) const = 0;
    protected:
        DescriptionBox(Object *parent, const std::string &name, const RelativePosition &pos, float z);
        DescriptionBox(Object *parent, const WidgetPrototype &arg);
        DescriptionBox(Object *parent, const DescriptionBox &arg);
        void Refresh();
        void Init();
    public:
        TextComponent& GetName();
        TextComponent& GetDescription();
    };
}