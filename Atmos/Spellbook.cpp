
#include "Spellbook.h"

#include "Spell.h"
#include "FontDefines.h"

namespace Atmos
{
    Spellbook::Spellbook() : background(nullptr)
    {}

    Spellbook::Spellbook(agui::Root &root, const agui::RelativePosition &pos, DescriptionBox &description, ContextMenu *contextMenu) : background(nullptr)
    {
        Init(root, pos, description, contextMenu);
    }

    void Spellbook::Init(agui::Root &root, const agui::RelativePosition &pos, DescriptionBox &description, ContextMenu *contextMenu)
    {
        // Background
        background = agui::Image::Factory(&root, "background", pos, 0);
        background->ScaleTo(256, 768);
        background->GetSprite()->color.Edit(255, 255, 100, 100);

        InitBase(*background, description, contextMenu);

        GetMenu()->SetPosition(agui::RelativePosition(agui::Dimension(0, 24), agui::Dimension(0, 20)));
        GetMenu()->GetLayout()->ChangeDimensions(230, 738);
        GetMenu()->GetSprite()->color.Edit(255, 255, 255, 255);
    }

    agui::Image* Spellbook::GetBackground()
    {
        return background;
    }
}