
#include "InventoryGui.h"

namespace Atmos
{
    void InventoryGui::OnGotoContextMenu()
    {
        GetContextMenu()->SetMoveFrom(*GetList());
        GetContextMenu()->SetMoveTo(*moveTo);
    }

    InventoryGui::InventoryGui() : background(nullptr), label(nullptr), moveTo(nullptr)
    {}

    InventoryGui::InventoryGui(Agui::Root &root, const Agui::RelativePosition &pos, const std::string &labelString, DescriptionBox &description, ContextMenu *contextMenu)
    {
        Init(root, pos, labelString, description, contextMenu);
    }

    void InventoryGui::Init(Agui::Root &root, const Agui::RelativePosition &pos, const std::string &labelString, DescriptionBox &description, ContextMenu *contextMenu)
    {
        // Background
        background = Agui::Image::Factory(&root, "background", pos, 0);
        background->ScaleTo(256, 768);
        background->GetSprite()->color.Edit(255, 100, 100, 255);

        // Label
        if (labelString != "")
        {
            label = Agui::Label::Factory(background, "label", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 2), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP), 0);
            label->GetText()->SetString(labelString);
            label->GetText()->color.Edit(255, 255, 255, 255);
            label->GetText()->SetAutoCalcTextSize();
        }

        InitBase(*background, description, contextMenu);
        GetMenu()->SetPosition(Agui::RelativePosition(Agui::Dimension(0, 24), Agui::Dimension(0, 20)));
        GetMenu()->GetLayout()->ChangeDimensions(230, 738);
        GetMenu()->GetSprite()->color.Edit(255, 255, 255, 255);
    }

    void InventoryGui::SetMoveTo()
    {
        moveTo = nullptr;
    }

    void InventoryGui::SetMoveTo(Ent::InventoryComponent &set)
    {
        moveTo = &set;
    }

    Agui::Image* InventoryGui::GetBackground()
    {
        return background;
    }

    Agui::Label* InventoryGui::GetLabel()
    {
        return label;
    }
}