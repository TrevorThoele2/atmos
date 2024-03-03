
#pragma once

#include <set>

#include "Attribute.h"

#include "Name.h"
#include "OperatorSelector.h"
#include "BoundedNumber.h"
#include "Optional.h"
#include "RegistryObjectReference.h"

#include "DataStandard.h"

#include "Serialization.h"

namespace Atmos
{
    class CharacterClass;
    class StatAttributeTable
    {
    public:
        typedef Attribute Value;

        class Modifier
        {
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            Name name;
            Value value;
            OperatorSelector op;

            Modifier();
            Modifier(const Name &name, Value value, OperatorSelector op);
            bool operator==(const Modifier &arg) const;
            bool operator!=(const Modifier &arg) const;
        };
    private:
        class Entry
        {
        public:
            constexpr static Value maxPossible = 10000;
            constexpr static Value minPossible = -10000;

            // The value as it is without the modifiers
            Value base;
            StaticBoundedNumber<Value, minPossible, maxPossible> value;
            Entry(Value base);
            bool operator==(const Entry &arg) const;
            bool operator!=(const Entry &arg) const;
        };

        struct ModifierSort
        {
            bool operator()(const Modifier &left, const Modifier &right) const
            {
                return static_cast<std::underlying_type<OperatorSelector>::type>(left.op) < static_cast<std::underlying_type<OperatorSelector>::type>(right.op);
            }
        };
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        friend DataStandard<StatAttributeTable>;
    private:
        std::unordered_map<Name, Entry> map;
        std::set<Modifier, ModifierSort> modifiers;

        void AddEntry(const Name &name, Value value);
        void CalculateEntries();
    public:
        StatAttributeTable();
        StatAttributeTable(const RegistryObjectReference<CharacterClass> &charClass);

        bool operator==(const StatAttributeTable &arg) const;
        bool operator!=(const StatAttributeTable &arg) const;

        void SetCharacterClass(const RegistryObjectReference<CharacterClass> &charClass);

        void AddModifier(const Modifier &mod);
        void RemoveModifier(const Modifier &mod);

        Optional<Value> GetValue(const Name &name) const;
    };

    template<>
    class DataStandard<StatAttributeTable> : public DataStandardInherit<StatAttributeTable>
    {
    public:
        struct Entry
        {
            const Name name;
            Name niceName;
            Name opposing;
            Entry(const Name &name, const Name &niceName = "", const Name &opposing = "");
            bool operator==(const Entry &arg) const;
            bool operator!=(const Entry &arg) const;
        };
    private:
        typedef EntryMap<Entry> Entries;
    public:
        typedef Entries::const_iterator const_iterator;
    private:
        static Entries entries;
        static void SetupObj(Object &object, const Name &name, Entry &entry);

        static void SerializeImpl(::inscription::Scribe &scribe);

        friend DataStandardInherit<StatAttributeTable>;
    public:
        static DataStandard& Instance();
        static void Setup(Object &object);

        static Entry* Add(const Name &name, const Name &niceName = "", const Name &opposing = "");
        static Entry* Add(Entry &&add);
        static void Remove(const Name &name);
        static const_iterator Remove(const_iterator itr);
        static Entry* Find(const Name &name);

        static std::set<Name> GetNames();
        static Optional<Name> GetNiceName(const Name &stat);
        static Optional<Name> GetOpposing(const Name &stat);
        
        const_iterator begin() const;
        const_iterator end() const;
    };
}