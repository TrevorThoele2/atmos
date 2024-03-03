
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

    InventoryGui::InventoryGui(agui::Root &root, const agui::RelativePosition &pos, const std::string &labelString, DescriptionBox &description, ContextMenu *contextMenu)
    {
        Init(root, pos, labelString, description, contextMenu);
    }

    void InventoryGui::Init(agui::Root &root, const agui::RelativePosition &pos, const std::string &labelString, DescriptionBox &description, ContextMenu *contextMenu)
    {
        // Background
        background = agui::Image::Factory(&root, "background", pos, 0);
        background->ScaleTo(256, 768);
        background->GetSprite()->color.Edit(255, 100, 100, 255);

        // Label
        if (labelString != "")
        {
            label = agui::Label::Factory(background, "label", agui::RelativePosition(agui::Dimension(), agui::Dimension(0, 2), agui::HorizontalAlignment::MID, agui::VerticalAlignment::TOP), 0);
            label->GetText()->SetString(labelString);
            label->GetText()->color.Edit(255, 255, 255, 255);
            label->GetText()->SetAutoCalcTextSize();
        }

        InitBase(*background, description, contextMenu);
        GetMenu()->SetPosition(agui::RelativePosition(agui::Dimension(0, 24), agui::Dimension(0, 20)));
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

    agui::Image* InventoryGui::GetBackground()
    {
        return background;
    }

    agui::Label* InventoryGui::GetLabel()
    {
        return label;
    }
}