#pragma once

#include <unordered_map>

#include "Name.h"
#include "Percentage.h"

#include "ObjectReference.h"

#include "Serialization.h"

namespace Atmos
{
    namespace Ent
    {
        class nCombatComponent;
    }

    class Proficiencies
    {
    public:
        typedef unsigned int Experience;
        typedef FixedPoint64 Level;
        typedef Percentage Rating;

        typedef TypedObjectReference<Ent::nCombatComponent> CombatComponentReference;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        const Experience levelThreshold = 100;
        const Level maxLevel = Level(180, 0);

        struct Entry
        {
            Experience experience;
            Level level;
            Level profLevelPerCharLevel;
            Entry();
            Entry(Experience experience, Level level, Level profLevelPerCharLevel);
            Entry(const Entry &arg) = default;
            Entry& operator=(const Entry &arg) = default;
            bool operator==(const Entry &arg) const;
            bool operator!=(const Entry &arg) const;
        };
    private:
        std::unordered_map<Name, Entry> entries;
    public:
        Proficiencies() = default;
        Proficiencies(const Proficiencies &arg);
        Proficiencies(Proficiencies &&arg);
        Proficiencies& operator=(const Proficiencies &arg);
        Proficiencies& operator=(Proficiencies &&arg);

        bool operator==(const Proficiencies &arg) const;
        bool operator!=(const Proficiencies &arg) const;

        // Returns the experience given (can be 0)
        Experience AttemptIncrement(const Name &name, CombatComponentReference source);
        Experience GetExperience(const Name &name) const;
        Level GetLevel(const Name &name) const;
    };
}