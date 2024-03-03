
#include "Proficiency.h"

#include "CombatComponent.h"
#include "Random.h"

#include "Error.h"

#include <Inscription/Scribe.h>
#include <Inscription/ContainerSize.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Proficiencies)
    {
        if (scribe.IsOutput())
        {
            inscription::ContainerSize size(entries.size());
            scribe.Save(size);

            for (auto &loop : entries)
            {
                scribe.Save(::inscription::RemoveConst(loop.first));
                scribe.Save(loop.second.experience);
                scribe.Save(loop.second.level);
                scribe.Save(loop.second.profLevelPerCharLevel);
            }
        }
        else
        {
            inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                Name name;
                scribe.Load(name);

                Experience experience;
                scribe.Load(experience);

                Level level;
                scribe.Load(level);

                Level profLevelPerCharLevel;
                scribe.Load(profLevelPerCharLevel);

                entries.emplace(name, Entry(experience, level, profLevelPerCharLevel));
            }
        }
    }

    Proficiencies::Entry::Entry() : experience(0)
    {}

    Proficiencies::Entry::Entry(Experience experience, Level level, Level profLevelPerCharLevel) : experience(experience), level(level), profLevelPerCharLevel(profLevelPerCharLevel)
    {}

    bool Proficiencies::Entry::operator==(const Entry &arg) const
    {
        return experience == arg.experience && level == arg.level && profLevelPerCharLevel == arg.profLevelPerCharLevel;
    }

    bool Proficiencies::Entry::operator!=(const Entry &arg) const
    {
        return !(*this == arg);
    }

    Proficiencies::Proficiencies(const Proficiencies &arg) : entries(arg.entries)
    {}

    Proficiencies::Proficiencies(Proficiencies &&arg) : entries(std::move(arg.entries))
    {}

    Proficiencies& Proficiencies::operator=(const Proficiencies &arg)
    {
        entries = arg.entries;
        return *this;
    }

    Proficiencies& Proficiencies::operator=(Proficiencies &&arg)
    {
        entries = std::move(arg.entries);
        return *this;
    }

    bool Proficiencies::operator==(const Proficiencies &arg) const
    {
        return entries == arg.entries;
    }

    bool Proficiencies::operator!=(const Proficiencies &arg) const
    {
        return !(*this == arg);
    }

    Proficiencies::Experience Proficiencies::AttemptIncrement(const Name &name, const Ent::CombatComponent &source)
    {
        auto focused = entries.find(name);
        if (focused == entries.end())
        {
            focused = entries.emplace(name, Entry()).first;
            // Max possible level with the proficiency rating
            Rating::WrappedT adjustedMaxLevel(Rating::WrappedT(static_cast<double>(source.charClass->FindProficiencyRating(name)->ConvertToDecimal())) * maxLevel);
            // Figure out how many proficiency levels per character level
            focused->second.profLevelPerCharLevel = adjustedMaxLevel / Rating::WrappedT::Split(Ent::CombatComponent::Level::GetUpperBoundStatic(), 0);
        }

        // Filter out 50% of the attempts as no experience
        if (!Random::GenerateBool())
            return 0;

        // Check if proficiency level is maxed out
        Level curMaxLevel(focused->second.profLevelPerCharLevel * Level(source.level.Get(), 0));
        if (curMaxLevel <= focused->second.level)
            return 0;

        // Experience away from next level
        const Experience experienceFromNextLevel = focused->second.experience % levelThreshold;

        // Calculate the amount of experience to add to the proficiency
        // If you're 3 levels away from max, add in the possibility of getting a +5, 6 levels away +6, 9 levels away +7, etc.
        // Cap at +15
        const Experience baseExpPossibility = 4;
        const FixedPoint64 levelStep(3, 0);
        const Experience maxPossibleExpCap = 15;
        Experience maxExpPossibility = baseExpPossibility + static_cast<Experience>((curMaxLevel - focused->second.level) / levelStep);
        if (maxExpPossibility > maxPossibleExpCap)
            maxExpPossibility = maxPossibleExpCap;

        // Add experience
        const Experience generatedExperience = Random::Generate(unsigned int(1), maxExpPossibility);
        focused->second.experience += generatedExperience;

        // Check if gained a new level
        if (experienceFromNextLevel <= generatedExperience)
            ++focused->second.level;

        return generatedExperience;
    }

    Proficiencies::Experience Proficiencies::GetExperience(const Name &name) const
    {
        auto found = entries.find(name);
        if (found == entries.end())
            ErrorHandler::Log("Tried to find a proficiency with a tag that doesn't exist.",
                ErrorHandler::Severity::ERROR_LOW,
                ErrorHandler::NameValueVector{ NameValuePair("Proficiency Name", name) });

        return found->second.experience;
    }

    Proficiencies::Level Proficiencies::GetLevel(const Name &name) const
    {
        auto found = entries.find(name);
        if (found == entries.end())
            ErrorHandler::Log("Tried to find a proficiency with a tag that doesn't exist.",
                ErrorHandler::Severity::ERROR_LOW,
                ErrorHandler::NameValueVector{ NameValuePair("Proficiency Name", name) });

        return found->second.level;
    }
}