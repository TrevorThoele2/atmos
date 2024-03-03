
#include "WidgetSetups.h"

#include "FontDefines.h"

#include "Cursor.h"
#include "ItemDescriptionBox.h"
#include "SpellDescriptionBox.h"
#include "ItemContextMenu.h"
#include "SpellContextMenu.h"

#include <AGUI\BasicWidget.h>
#include <AGUI\PushButton.h>
#include <AGUI\Checkbox.h>
#include <AGUI\Fillbar.h>
#include <AGUI\Image.h>
#include <AGUI\Label.h>
#include <AGUI\Menu.h>
#include <AGUI\MenuButton.h>
#include <AGUI\Scrollbar.h>
#include <AGUI\Slider.h>
#include <AGUI\Textbox.h>
#include <AGUI\Thumb.h>

namespace agui
{
    void SetupWidgets()
    {
        // CUSTOM
        ItemDescriptionBoxSetup::Main().sprite = SpriteComponent::Factory("sprite", 0.0f, RelativePosition(), Size(), agui::Sprite("Pixel.png", 1, Color()));
        ItemDescriptionBoxSetup::Main().spriteFitter = true;

        SpellDescriptionBoxSetup::Main().sprite = SpriteComponent::Factory("sprite", 0.0f, RelativePosition(), Size(), agui::Sprite("Pixel.png", 1, Color()));
        SpellDescriptionBoxSetup::Main().spriteFitter = true;

        CursorSetup<unsigned char>::Main().sprite = SpriteComponent::Factory("sprite", 0.0f, agui::RelativePosition(), agui::Size(), agui::Sprite("cursor.png", 1, agui::Color()));
        CursorSetup<unsigned short>::Main().sprite = SpriteComponent::Factory("sprite", 0.0f, agui::RelativePosition(), agui::Size(), agui::Sprite("cursor.png", 1, agui::Color()));

        // AGUI
        BasicWidgetSetup::Main().sprite = SpriteComponent::Factory("sprite", 0.0f, RelativePosition(), Size(), agui::Sprite("Pixel.png", 1, Color()));
        BasicWidgetSetup::Main().spriteFitter = true;

        CheckboxSetup::Main().sprite = SpriteComponent::Factory("sprite", 0.0f, RelativePosition(), Size(), agui::Sprite("check24.png", 1, Color()));
        CheckboxSetup::Main().spriteFitter = true;

        FillbarSetup::Main().frame = SpriteComponent::Factory("frame", 0.0f, RelativePosition(), Size(), Sprite("FillbarBackground.png", 1, Color()));
        FillbarSetup::Main().frameFitter = true;
        FillbarSetup::Main().filler = SpriteComponent::Factory("filler", 0.0f, RelativePosition(), Size(), Sprite("FillbarInside.png", 1, Color()));

        ImageSetup::Main().sprite = SpriteComponent::Factory("sprite", 0.0f, RelativePosition(), Size(), agui::Sprite("Pixel.png", 1, Color()));
        ImageSetup::Main().spriteFitter = true;

        LabelSetup::Main().text = TextComponent::Factory("text", 0.0f, RelativePosition(), Size(), agui::Text("", agui::Text::Format(), *fontSlender, Color()));
        LabelSetup::Main().textFitter = true;

        MenuSetup::Main().sprite = SpriteComponent::Factory("sprite", 0.0f, RelativePosition(), Size(), agui::Sprite("Pixel.png", 1, Color()));
        MenuSetup::Main().spriteFitter = true;

        PushButtonSetup::Main().sprite = SpriteComponent::Factory("sprite", 0.0f, RelativePosition(), Size(), agui::Sprite("button.png", 1, Color()));
        PushButtonSetup::Main().spriteFitter = true;
        PushButtonSetup::Main().text = TextComponent::Factory("text", 1.0f, RelativePosition(), Size(), agui::Text("", agui::Text::Format({ agui::Text::CENTER_HORIZONTAL, agui::Text::CENTER_VERTICAL }), *fontButton, Color()));
        PushButtonSetup::Main().textFitter = true;

        MenuButtonSetup::Main().menu.prototype.name = "menu";
        MenuButtonSetup::Main().menu.widgetSetup = MenuSetup::Main();
        MenuButtonSetup::Main().button.prototype.name = "button";
        MenuButtonSetup::Main().button.widgetSetup = PushButtonSetup::Main();

        ThumbSetup::Main().sprite = SpriteComponent::Factory("sprite", 0.0f, RelativePosition(), Size(), agui::Sprite("SliderMovable.png", 1, Color()));
        ThumbSetup::Main().spriteFitter = true;

        ScrollbarSetup::Main().sprite = SpriteComponent::Factory("sprite", 0.0f, RelativePosition(), Size(), agui::Sprite("SliderBackgroundVertical.png", 1, Color()));
        ScrollbarSetup::Main().spriteFitter = true;
        ScrollbarSetup::Main().orient = Orientation::VERTICAL;
        ScrollbarSetup::Main().thumb.prototype.name = "thumb";
        ScrollbarSetup::Main().thumb.prototype.margins.Set(1, 1, 1, 1);
        ScrollbarSetup::Main().thumb.widgetSetup = ThumbSetup::Main();
        ScrollbarSetup::Main().increment.prototype.name = "increment";
        ScrollbarSetup::Main().increment.prototype.pos.SetY(Dimension(0, 20));
        ScrollbarSetup::Main().increment.prototype.pos.SetAlignments(HorizontalAlignment::MID, VerticalAlignment::BOT);
        ScrollbarSetup::Main().increment.widgetSetup = PushButtonSetup::Main();
        ScrollbarSetup::Main().increment.widgetSetup.sprite->SetResource("Graphics\\GUI\\increment.png");
        ScrollbarSetup::Main().decrement.prototype.name = "decrement";
        ScrollbarSetup::Main().decrement.prototype.pos.SetY(Dimension(0, -20));
        ScrollbarSetup::Main().decrement.prototype.pos.SetHorizontalAlignment(HorizontalAlignment::MID);
        ScrollbarSetup::Main().decrement.widgetSetup = PushButtonSetup::Main();
        ScrollbarSetup::Main().decrement.widgetSetup.sprite->SetResource("Graphics\\GUI\\decrement.png");

        SliderSetup::Main().sprite = SpriteComponent::Factory("sprite", 0.0f, RelativePosition(), Size(), agui::Sprite("SliderBackground.png", 1, Color()));
        SliderSetup::Main().spriteFitter = true;
        SliderSetup::Main().thumb.prototype.name = "thumb";
        SliderSetup::Main().thumb.widgetSetup = ThumbSetup::Main();

        TextboxSetup::Main().sprite = SpriteComponent::Factory("sprite", 0.0f, RelativePosition(), Size(), agui::Sprite("Pixel.png", 1, Color()));
        TextboxSetup::Main().spriteFitter = true;

        ItemContextMenuSetup::Main().menu.widgetSetup = MenuSetup::Main();
        ItemContextMenuSetup::Main().cursor.prototype.name = "cursor";
        ItemContextMenuSetup::Main().cursor.prototype.pos.SetX(Dimension(0, -20));
        ItemContextMenuSetup::Main().cursor.widgetSetup = CursorSetup<unsigned char>::Main();

        SpellContextMenuSetup::Main().menu.widgetSetup = MenuSetup::Main();
        SpellContextMenuSetup::Main().cursor.prototype.name = "cursor";
        SpellContextMenuSetup::Main().cursor.prototype.pos.SetX(Dimension(0, -20));
        SpellContextMenuSetup::Main().cursor.widgetSetup = CursorSetup<unsigned char>::Main();
    }
}