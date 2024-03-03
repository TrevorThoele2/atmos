
/*
#pragma once

#include <string>
#include <unordered_map>

#include "Spell.h"

namespace Atmos
{
    class Spellbook;
    class SpellList
    {
    public:
        class Entry
        {
        private:
            static inscription::Inscripter<Entry>& GetInscripter();
        public:
            Entry(const inscription::Table<Entry> &table);
            INSCRIPTION_ACCESS;
        private:
            SpellList *owner;
            const Spell *spell;
            Entry(SpellList &owner);
            Entry(SpellList &owner, const Spell &spell);
            Entry(SpellList &owner, const Entry &arg);
            Entry& operator=(Entry &&arg);
            Entry& Copy(SpellList &owner, const Entry &arg);
        public:
            Entry(Entry &&arg);
            operator const Spell&() const;

            void Select() const;
            const std::string& GetName() const;
            friend SpellList;
        };
    private:
        
    public:
        typedef Map::iterator iterator;
        typedef Map::const_iterator const_iterator;
        typedef Map::size_type Size;
    private:
        static inscription::Inscripter<SpellList>& GetInscripter();
    public:
        SpellList(Character &owner, const inscription::Table<SpellList> &table);
        INSCRIPTION_ACCESS;
    private:
        Character &owner;
        Map map;
        Spellbook *gui;
        
    };
}
*/