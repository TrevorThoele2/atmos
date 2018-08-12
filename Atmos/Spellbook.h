
#pragma once

#include "CombatComponent.h"
#include "ObjectMenu.h"
#include "SpellDescriptionBox.h"
#include "SpellContextMenu.h"

#include <AGUI\Root.h>
#include <AGUI\Image.h>

namespace Atmos
{
    class Spellbook : public ObjectMenu<const Spell, Ent::CombatComponent, Agui::SpellDescriptionBox, Agui::SpellContextMenu>
    {
    private:
        Agui::Image *background;
        Spellbook(const Spellbook &arg) = delete;
    public:
        Spellbook();
        Spellbook(Agui::Root &root, const Agui::RelativePosition &pos, DescriptionBox &description, ContextMenu *contextMenu);
        void Init(Agui::Root &root, const Agui::RelativePosition &pos, DescriptionBox &description, ContextMenu *contextMenu);
        Agui::Image* GetBackground();
    };

    static std::unique_ptr<Spellbook> spellbook;
}