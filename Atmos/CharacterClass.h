#pragma once

#include "RegistryObject.h"

#include "Attribute.h"
#include "Proficiency.h"
#include "ItemStash.h"
#include "Optional.h"

#include "Serialization.h"

namespace Atmos
{
    class CharacterClass : public RegistryObject
    {
    public:
        class AttributeEntry
        {
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            typedef Attribute ValueT;
            ValueT base;
            ValueT growth;
            AttributeEntry();
            AttributeEntry(ValueT base, ValueT growth);
            bool operator==(const AttributeEntry &arg) const;
            bool operator!=(const AttributeEntry &arg) const;
        };
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        std::unordered_map<Name, AttributeEntry> resources;
        std::unordered_map<Name, AttributeEntry> stats;
        std::unordered_map<Name, Proficiencies::Rating> proficiencyRatings;
        ItemStashSize permanentMaxItemCount;
        ItemStashSize temporaryMaxItemCount;

        CharacterClass();
        CharacterClass(const CharacterClass &arg) = default;
        CharacterClass& operator=(const CharacterClass &arg) = default;
        CharacterClass(CharacterClass &&arg);
        CharacterClass& operator=(CharacterClass &&arg);
        
        bool operator==(const CharacterClass &arg) const;
        bool operator!=(const CharacterClass &arg) const;

        AttributeEntry* FindResource(const Name &name);
        AttributeEntry* FindStat(const Name &name);

        Proficiencies::Rating* AddProficiencyRating(const Name &name, Proficiencies::Rating rating);
        void RemoveProficiencyRating(const Name &name);
        Proficiencies::Rating* FindProficiencyRating(const Name &name);
        const Proficiencies::Rating* FindProficiencyRating(const Name &name) const;
    };

    template<>
    class Registry<CharacterClass> : public RegistryBase<CharacterClass, Registry<CharacterClass>>
    {
    private:
        Registry() = default;
        friend RegistryBase<CharacterClass, Registry<CharacterClass>>;
    };

    typedef Registry<CharacterClass> CharacterClassRegistry;
}