#pragma once

#include "RegistryObject.h"

#include "Attribute.h"
#include "Proficiency.h"
#include "ItemStash.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class nCharacterClass : public RegistryObject
    {
    public:
        class AttributeEntry
        {
        public:
            typedef Attribute ValueT;
            ValueT base;
            ValueT growth;
            AttributeEntry();
            AttributeEntry(ValueT base, ValueT growth);
            bool operator==(const AttributeEntry &arg) const;
            bool operator!=(const AttributeEntry &arg) const;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        };
    public:
        std::unordered_map<Name, AttributeEntry> resources;
        std::unordered_map<Name, AttributeEntry> stats;
        std::unordered_map<Name, Proficiencies::Rating> proficiencyRatings;
        ItemStashSize permanentMaxItemCount;
        ItemStashSize temporaryMaxItemCount;
    public:
        nCharacterClass(const Name& name);
        nCharacterClass(const nCharacterClass& arg) = default;
        nCharacterClass(const ::Inscription::Table<nCharacterClass>& table);

        AttributeEntry* FindResource(const Name &name);
        AttributeEntry* FindStat(const Name &name);

        Proficiencies::Rating* AddProficiencyRating(const Name &name, Proficiencies::Rating rating);
        void RemoveProficiencyRating(const Name &name);
        Proficiencies::Rating* FindProficiencyRating(const Name &name);
        const Proficiencies::Rating* FindProficiencyRating(const Name &name) const;

        ObjectTypeDescription TypeDescription() const override;
    };

    template<>
    struct ObjectTraits<nCharacterClass> : ObjectTraitsBase<nCharacterClass>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<RegistryObject> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::nCharacterClass)
    {
    public:
        static void AddMembers(TableT& table);
    };
}