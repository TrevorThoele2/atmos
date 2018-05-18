
#pragma once

#include <unordered_map>

#include "Name.h"
#include "FixedPoint.h"
#include "Optional.h"
#include "GlobalContext.h"

#include "Serialization.h"

namespace Atmos
{
    class Element
    {
    public:
        typedef FixedPoint64 Multiplier;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        Name name;
    public:
        Element() = default;
        Element(const Name &name);
        Element& operator=(const Name &name);
        Element(const Element &arg) = default;
        Element& operator=(const Element &arg) = default;

        bool operator==(const Element &arg) const;
        bool operator!=(const Element &arg) const;

        explicit operator const Name&() const;

        void Set(const Name &set);
        const Name& Get() const;

        Multiplier GetAdvantageAgainst(const Element &against) const;
        Multiplier GetSameMultiplier(const Element &main) const;
    };
}

namespace std
{
    template<>
    struct hash<::Atmos::Element>
    {
        typedef ::Atmos::Element argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type &arg) const
        {
            const result_type first(std::hash<::Atmos::Name>()(static_cast<::Atmos::Name>(arg)));
            return first;
        }
    };
}

namespace Atmos
{
    template<>
    class GlobalContext<Element> : public GlobalContextBase<Element, GlobalContext<Element>>
    {
    private:
        typedef GlobalContextBase<Element, GlobalContext<Element>> BaseT;
        friend GlobalContextBase<Element, GlobalContext<Element>>;
    public:
        struct Entry
        {
            const Name name;
            Name niceName;
            Element opposing;
            Name statName;
            Entry(const Name &name, const Name &niceName = Name(), const Element &opposing = Element(), const Name &statName = Name());
            bool operator==(const Entry &arg) const;
            bool operator!=(const Entry &arg) const;
        };
    public:
        typedef std::unordered_map<Element, Entry> Entries;
        typedef Entries::const_iterator const_iterator;
    private:
        static std::unordered_map<Element, Entry> entries;

        static void SerializeImpl(::inscription::Scribe &scribe);
    public:
        typedef Element::Multiplier Multiplier;

        static Name neutral;
        static GlobalContext& Instance();

        static Entry* Add(const Name &name, const Name &niceName = Name(), const Element &opposing = Element(), const Name &statName = Name());
        static Entry* Add(Entry &&add);
        static void Remove(const Name &name);
        static const_iterator Remove(const_iterator itr);
        static Entry* Find(const Name &name);
        static Entries& GetEntries();

        static Multiplier GetAdvantage(const Element &main, const Element &against);
        static Optional<Name> GetStatName(const Element &check);

        const_iterator begin() const;
        const_iterator end() const;
    };
}