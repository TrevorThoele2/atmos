#pragma once

#include <unordered_set>
#include "RegistryObjectReference.h"

#include "Serialization.h"

namespace Atmos
{
    class CharacterClass;
    class CharacterClassGroup
    {
    private:
        typedef std::unordered_set<RegistryObjectReference<CharacterClass>> Set;
    public:
        typedef Set::size_type size_type;
        typedef Set::const_iterator const_iterator;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        bool usesAll;
        Set set;
    public:
        CharacterClassGroup(bool usesAll = false);
        CharacterClassGroup(const CharacterClassGroup &arg) = default;
        CharacterClassGroup(CharacterClassGroup &&arg);
        CharacterClassGroup& operator=(const CharacterClassGroup &arg) = default;
        CharacterClassGroup& operator=(CharacterClassGroup &&arg);

        bool operator==(const CharacterClassGroup &arg) const;
        bool operator!=(const CharacterClassGroup &arg) const;

        void Add(const RegistryObjectReference<CharacterClass> &add);
        void Remove(const RegistryObjectReference<CharacterClass> &remove);
        void Clear();

        // When this uses all, then this will always return true for In
        void UsesAll(bool set);
        // When this uses all, then this will always return true for In
        bool UsesAll() const;
        bool In(const CharacterClass &find) const;

        size_type size() const;
        bool empty() const;
        const_iterator begin() const;
        const_iterator end() const;
    };
}