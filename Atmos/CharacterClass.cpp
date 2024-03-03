
#include "CharacterClass.h"

#include "ResourceAttribute.h"
#include "StatAttribute.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(CharacterClass::AttributeEntry)
    {
        scribe(base);
        scribe(growth);
    }

    CharacterClass::AttributeEntry::AttributeEntry() : base(0), growth(0)
    {}

    CharacterClass::AttributeEntry::AttributeEntry(ValueT base, ValueT growth) : base(base), growth(growth)
    {}

    bool CharacterClass::AttributeEntry::operator==(const AttributeEntry &arg) const
    {
        return base == arg.base && growth == arg.growth;
    }

    bool CharacterClass::AttributeEntry::operator!=(const AttributeEntry &arg) const
    {
        return !(*this == arg);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(CharacterClass)
    {
        ::Inscription::BaseSerialize<RegistryObject>(scribe, *this);
        for (auto& loop : resources)
            scribe(loop.second);
        for (auto& loop : stats)
            scribe(loop.second);
        scribe(proficiencyRatings);
        scribe(permanentMaxItemCount);
        scribe(temporaryMaxItemCount);
    }

    CharacterClass::CharacterClass()
    {
        // Resources
        {
            auto &names = DataStandard<ResourceAttributeTable>::GetNames();
            for (auto &loop : names)
                resources.emplace(loop, AttributeEntry());
        }

        // Stats
        {
            auto &names = DataStandard<StatAttributeTable>::GetNames();
            for (auto &loop : names)
                stats.emplace(loop, AttributeEntry());
        }
    }

    CharacterClass::CharacterClass(CharacterClass &&arg) : RegistryObject(std::move(arg)), resources(std::move(arg.resources)), stats(std::move(arg.stats)), proficiencyRatings(std::move(arg.proficiencyRatings)), permanentMaxItemCount(std::move(arg.permanentMaxItemCount)), temporaryMaxItemCount(std::move(arg.temporaryMaxItemCount))
    {}

    CharacterClass& CharacterClass::operator=(CharacterClass &&arg)
    {
        RegistryObject::operator=(std::move(arg));
        resources = std::move(arg.resources);
        stats = std::move(arg.stats);
        proficiencyRatings = std::move(arg.proficiencyRatings);
        permanentMaxItemCount = std::move(arg.permanentMaxItemCount);
        temporaryMaxItemCount = std::move(arg.temporaryMaxItemCount);
        return *this;
    }

    bool CharacterClass::operator==(const CharacterClass &arg) const
    {
        return RegistryObject::operator==(arg);
    }

    bool CharacterClass::operator!=(const CharacterClass &arg) const
    {
        return !(*this == arg);
    }

    CharacterClass::AttributeEntry* CharacterClass::FindResource(const Name &name)
    {
        auto found = resources.find(name);
        if (found == resources.end())
            return nullptr;

        return &found->second;
    }

    CharacterClass::AttributeEntry* CharacterClass::FindStat(const Name &name)
    {
        auto found = stats.find(name);
        if (found == stats.end())
            return nullptr;

        return &found->second;
    }

    Proficiencies::Rating* CharacterClass::AddProficiencyRating(const Name &name, Proficiencies::Rating rating)
    {
        return &proficiencyRatings.emplace(name, rating).first->second;
    }

    void CharacterClass::RemoveProficiencyRating(const Name &name)
    {
        proficiencyRatings.erase(name);
    }

    Proficiencies::Rating* CharacterClass::FindProficiencyRating(const Name &name)
    {
        auto found = proficiencyRatings.find(name);
        if (found == proficiencyRatings.end())
            return nullptr;

        return &found->second;
    }

    const Proficiencies::Rating* CharacterClass::FindProficiencyRating(const Name &name) const
    {
        auto found = proficiencyRatings.find(name);
        if (found == proficiencyRatings.end())
            return nullptr;

        return &found->second;
    }
}