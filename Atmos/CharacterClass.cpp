
#include "CharacterClass.h"

namespace Atmos
{
    nCharacterClass::AttributeEntry::AttributeEntry() : base(0), growth(0)
    {}

    nCharacterClass::AttributeEntry::AttributeEntry(ValueT base, ValueT growth) : base(base), growth(growth)
    {}

    bool nCharacterClass::AttributeEntry::operator==(const AttributeEntry &arg) const
    {
        return base == arg.base && growth == arg.growth;
    }

    bool nCharacterClass::AttributeEntry::operator!=(const AttributeEntry &arg) const
    {
        return !(*this == arg);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(nCharacterClass::AttributeEntry)
    {
        scribe(base);
        scribe(growth);
    }

    nCharacterClass::nCharacterClass(ObjectManager& manager, const Name& name) :
        RegistryObject(manager, name), permanentMaxItemCount(0), temporaryMaxItemCount(0)
    {}

    nCharacterClass::nCharacterClass(const ::Inscription::Table<nCharacterClass>& table) : INSCRIPTION_TABLE_GET_BASE(RegistryObject)
    {}

    nCharacterClass::AttributeEntry* nCharacterClass::FindResource(const Name &name)
    {
        auto found = resources.find(name);
        if (found == resources.end())
            return nullptr;

        return &found->second;
    }

    nCharacterClass::AttributeEntry* nCharacterClass::FindStat(const Name &name)
    {
        auto found = stats.find(name);
        if (found == stats.end())
            return nullptr;

        return &found->second;
    }

    Proficiencies::Rating* nCharacterClass::AddProficiencyRating(const Name &name, Proficiencies::Rating rating)
    {
        return &proficiencyRatings.emplace(name, rating).first->second;
    }

    void nCharacterClass::RemoveProficiencyRating(const Name &name)
    {
        proficiencyRatings.erase(name);
    }

    Proficiencies::Rating* nCharacterClass::FindProficiencyRating(const Name &name)
    {
        auto found = proficiencyRatings.find(name);
        if (found == proficiencyRatings.end())
            return nullptr;

        return &found->second;
    }

    const Proficiencies::Rating* nCharacterClass::FindProficiencyRating(const Name &name) const
    {
        auto found = proficiencyRatings.find(name);
        if (found == proficiencyRatings.end())
            return nullptr;

        return &found->second;
    }

    ObjectTypeDescription nCharacterClass::TypeDescription() const
    {
        return ObjectTraits<nCharacterClass>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<nCharacterClass>::typeName = "CharacterClass";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::nCharacterClass)
    {

    }
}