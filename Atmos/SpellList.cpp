
/*
#include "SpellList.h"

#include "Spell.h"
#include "Character.h"

#include <Inscription\Scribe.h>
#include <Inscription\Const.h>

namespace Atmos
{
    ::Inscription::Inscripter<SpellList::Entry>& SpellList::Entry::GetInscripter()
    {
        INSCRIPTION_SIMPLE_INSCRIPTER_TABLE_BEGIN(Entry)
            INSCRIPTION_TABLE_ADD(spell)
        INSCRIPTION_TABLE_END
    }

    SpellList::Entry::Entry(const ::Inscription::Table<Entry> &table) : owner(nullptr), INSCRIPTION_TABLE_GET_MEM(spell)
    {}

    SpellList::Entry::Entry(SpellList &owner) : owner(&owner), spell(nullptr)
    {}

    SpellList::Entry::Entry(SpellList &owner, const Spell &spell) : owner(&owner), spell(&spell)
    {}

    SpellList::Entry::Entry(SpellList &owner, const Entry &arg) : owner(&owner), spell(arg.spell)
    {}

    SpellList::Entry& SpellList::Entry::operator=(Entry &&arg)
    {
        owner = arg.owner;
        spell = arg.spell;
        return *this;
    }

    SpellList::Entry& SpellList::Entry::Copy(SpellList &owner, const Entry &arg)
    {
        this->owner = &owner;
        spell = arg.spell;
        return *this;
    }

    SpellList::Entry::Entry(Entry &&arg) : owner(arg.owner), spell(arg.spell)
    {}

    SpellList::Entry::operator const Spell&() const
    {
        return *spell;
    }

    void SpellList::Entry::Select() const
    {
        spell->Select(owner->owner);
    }

    const std::string& SpellList::Entry::GetName() const
    {
        return spell->GetName();
    }

    ::Inscription::Inscripter<SpellList>& SpellList::GetInscripter()
    {
        INSCRIPTION_INSCRIPTER_BEGIN(SpellList)
            INSCRIPTION_SERIALIZATION
            {
                INSCRIPTION_TABLE_LINK_EX("size", ::Inscription::ContainerSize())
                INSCRIPTION_TABLE_SERIALIZE
                if (scribe.IsOutput())
                {
                    for (auto &loop : obj.map)
                        scribe.Save(::Inscription::RemoveConst(loop.second));
                }
                else
                {
                    auto &size = table.Get<::Inscription::ContainerSize>("size");

                    obj.map.clear();
                    while (size-- > 0)
                    {
                        ::Inscription::StackConstructor<Entry> entry(scribe);
                        obj.map.emplace(entry->GetName(), std::move(*entry.Get()));
                    }
                }
            }

            INSCRIPTION_CONSTRUCTION
            {
                INSCRIPTION_TABLE_LINK_EX("size", ::Inscription::ContainerSize())
                INSCRIPTION_TABLE_CONSTRUCT
            }
        INSCRIPTION_INSCRIPTER_END
        INSCRIPTION_TABLE_BEGIN
            INSCRIPTION_TABLE_ADD_EXTERNAL_EX(::Inscription::ContainerSize, "size")
        INSCRIPTION_TABLE_END
    }

    SpellList::SpellList(Character &owner, const ::Inscription::Table<SpellList> &table) : owner(owner)
    {
        for (auto &loop : map)
            loop.second.owner = this;
    }

    
}
*/