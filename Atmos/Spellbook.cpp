
#include "Spellbook.h"

#include "Spell.h"
#include "FontDefines.h"

namespace Atmos
{
    Spellbook::Spellbook() : background(nullptr)
    {}

    Spellbook::Spellbook(Agui::Root &root, const Agui::RelativePosition &pos, DescriptionBox &description, ContextMenu *contextMenu) : background(nullptr)
    {
        Init(root, pos, description, contextMenu);
    }

    void Spellbook::Init(Agui::Root &root, const Agui::RelativePosition &pos, DescriptionBox &description, ContextMenu *contextMenu)
    {
        // Background
        background = Agui::Image::Factory(&root, "background", pos, 0);
        background->ScaleTo(256, 768);
        background->GetSprite()->color.Edit(255, 255, 100, 100);

        InitBase(*background, description, contextMenu);

        GetMenu()->SetPosition(Agui::RelativePosition(Agui::Dimension(0, 24), Agui::Dimension(0, 20)));
        GetMenu()->GetLayout()->ChangeDimensions(230, 738);
        GetMenu()->GetSprite()->color.Edit(255, 255, 255, 255);
    }

    Agui::Image* Spellbook::GetBackground()
    {
        return background;
    }
}