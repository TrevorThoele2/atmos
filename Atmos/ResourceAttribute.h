
#pragma once

#include <set>

#include "Attribute.h"

#include "Name.h"
#include "OperatorSelector.h"
#include "BoundedNumber.h"
#include "Optional.h"
#include "RegistryObjectReference.h"

#include "DataStandard.h"
#include "GlobalContext.h"

#include "Serialization.h"

namespace Atmos
{
    class CharacterClass;
    class ResourceAttributeTable
    {
    public:
        typedef Attribute ValueT;

        class Modifier
        {
        public:
            enum class Type : bool
            {
                VALUE,
                MAX
            };
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            Name name;
            ValueT value;
            OperatorSelector op;
            Type type;

            Modifier();
            Modifier(const Name &name, ValueT value, OperatorSelector op, Type type);
            bool operator==(const Modifier &arg) const;
            bool operator!=(const Modifier &arg) const;
        };
    private:
        class Entry
        {
        public:
            constexpr static ValueT maxPossible = 10000;

            // The value as it is without the modifiers
            ValueT base;
            DynamicBoundedNumber<ValueT> value;
            Entry(ValueT base);
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
        friend DataStandard<ResourceAttributeTable>;
    private:
        std::unordered_map<Name, Entry> map;
        std::set<Modifier, ModifierSort> modifiers;

        void AddEntry(const Name &name, ValueT value);
        void CalculateEntries();

        Entry& GetHealthImpl();
        const Entry& GetHealthImpl() const;
        Entry& GetManaImpl();
        const Entry& GetManaImpl() const;
    public:
        ResourceAttributeTable();
        ResourceAttributeTable(const RegistryObjectReference<CharacterClass> &charClass);

        bool operator==(const ResourceAttributeTable &arg) const;
        bool operator!=(const ResourceAttributeTable &arg) const;

        void SetCharacterClass(const RegistryObjectReference<CharacterClass> &charClass);

        void AddModifier(const Modifier &mod);
        void RemoveModifier(const Modifier &mod);

        Optional<ValueT> GetValue(const Name &name) const;
        Optional<ValueT> GetMax(const Name &name) const;

        void SetHealth(ValueT set);
        void SetMana(ValueT set);
        void SetMaxHealth(ValueT set);
        void SetMaxMana(ValueT set);
        ValueT GetHealth() const;
        ValueT GetMana() const;
        ValueT GetMaxHealth() const;
        ValueT GetMaxMana() const;
    };

    template<>
    class DataStandard<ResourceAttributeTable> : public DataStandardInherit<ResourceAttributeTable>
    {
    public:
        struct Entry
        {
            Name niceName;
            Entry();
            Entry(const Name &niceName);
        };
    private:
        static EntryMap<Entry> entries;
        static void SetupObj(Object &object, const Name &name);

        static void SerializeImpl(::inscription::Scribe &scribe);

        friend DataStandardInherit<ResourceAttributeTable>;
    public:
        static void Setup(Object &object);
        static std::set<Name> GetNames();
        static Optional<Name> GetNiceName(const Name &name);
    };

    template<>
    class GlobalContext<ResourceAttributeTable> : public GlobalContextBase<ResourceAttributeTable, GlobalContext<ResourceAttributeTable>>
    {
    private:
        typedef GlobalContextBase<ResourceAttributeTable, GlobalContext<ResourceAttributeTable>> BaseT;
        friend GlobalContextBase<ResourceAttributeTable, GlobalContext<ResourceAttributeTable>>;
    private:
        static void SerializeImpl(::inscription::Scribe &scribe);
    public:
        static Name health;
        static Name mana;
        static Name acumen;
        static GlobalContext& Instance();
    };
}