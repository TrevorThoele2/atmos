
#pragma once

#include "CombatComponent.h"
#include "ObjectMenu.h"
#include "SpellDescriptionBox.h"
#include "SpellContextMenu.h"

#include <AGUI\Root.h>
#include <AGUI\Image.h>

namespace Atmos
{
    class Spellbook : public ObjectMenu<const Spell, Ent::CombatComponent, agui::SpellDescriptionBox, agui::SpellContextMenu>
    {
    private:
        agui::Image *background;
        Spellbook(const Spellbook &arg) = delete;
    public:
        Spellbook();
        Spellbook(agui::Root &root, const agui::RelativePosition &pos, DescriptionBox &description, ContextMenu *contextMenu);
        void Init(agui::Root &root, const agui::RelativePosition &pos, DescriptionBox &description, ContextMenu *contextMenu);
        agui::Image* GetBackground();
    };

    static std::unique_ptr<Spellbook> spellbook;
}