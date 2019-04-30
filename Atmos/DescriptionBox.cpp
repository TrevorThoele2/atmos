
#include "DescriptionBox.h"

#include "FontDefines.h"
#include <AGUI/TextComponent.h>

namespace Agui
{
    void DescriptionBox::Setup()
    {
        CreateComponent<TextComponent>(name, "name", 1.0f, RelativePosition(HorizontalAlignment::LEFT, VerticalAlignment::TOP), Size(), Text("", Agui::Text::Format({ Agui::Text::CENTER_HORIZONTAL, Agui::Text::WORD_BREAK }), *fontSlender, Color(255, 0, 0, 0)));
        name->SetAutoCalcTextHeight();

        CreateComponent<TextComponent>(description, "description", 1.0f, RelativePosition(Dimension(), Dimension(0, static_cast<float>(fontSlender->GetHeight())), HorizontalAlignment::LEFT, VerticalAlignment::TOP), Size(), Text("", Agui::Text::Format({ Agui::Text::CENTER_HORIZONTAL, Agui::Text::WORD_BREAK }), *fontSlender, Color(255, 0, 0, 0)));
        description->SetAutoCalcTextHeight();
    }

    DescriptionBox::DescriptionBox(Object *parent, const std::string &name, const RelativePosition &pos, float z) : SpriteWidgetSync(parent, name, pos, z), name(nullptr), description(nullptr)
    {
        Setup();
    }

    DescriptionBox::DescriptionBox(Object *parent, const WidgetPrototype &arg) : SpriteWidgetSync(parent, arg), name(nullptr), description(nullptr)
    {
        Setup();
    }

    DescriptionBox::DescriptionBox(Object *parent, const DescriptionBox &arg) : SpriteWidgetSync(parent, arg), name(nullptr), description(nullptr)
    {
        Setup();
    }

    void DescriptionBox::Refresh()
    {
        std::string string;

        GetNameString(string);
        name->SetString(string);

        GetDescriptionString(string);
        description->SetString(string);

        Refresh_impl();
    }

    void DescriptionBox::Init()
    {
        auto fitter = CreateInternalArranger<FitterGroup>(true, *this);
        fitter->Add(*name, Fitter::Dimension(), Fitter::Dimension(false), false);
        fitter->Add(*description, Fitter::Dimension(), Fitter::Dimension(false, -10), false);
    }

    TextComponent& DescriptionBox::GetName()
    {
        return *name;
    }

    TextComponent& DescriptionBox::GetDescription()
    {
        return *description;
    }
}