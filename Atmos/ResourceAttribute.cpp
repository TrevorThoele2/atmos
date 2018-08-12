
#include "ResourceAttribute.h"

#include "CharacterClass.h"
#include "FixedPoint.h"

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ResourceAttributeTable::Modifier)
    {
        scribe(name);
        scribe(value);
        scribe(op);
        scribe(type);
    }

    ResourceAttributeTable::Modifier::Modifier() : value(0), op(OperatorSelector::ADD)
    {}

    ResourceAttributeTable::Modifier::Modifier(const Name &name, ValueT value, OperatorSelector op, Type type) : name(name), value(value), op(op), type(type)
    {}

    bool ResourceAttributeTable::Modifier::operator==(const Modifier &arg) const
    {
        return name == arg.name && value == arg.value && op == arg.op && type == arg.type;
    }

    bool ResourceAttributeTable::Modifier::operator!=(const Modifier &arg) const
    {
        return !(*this == arg);
    }

    ResourceAttributeTable::Entry::Entry(ValueT base) : base(base), value(base, base, 0)
    {}

    bool ResourceAttributeTable::Entry::operator==(const Entry &arg) const
    {
        return base == arg.base && value == arg.value;
    }

    bool ResourceAttributeTable::Entry::operator!=(const Entry &arg) const
    {
        return !(*this == arg);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ResourceAttributeTable)
    {
        for (auto& loop : map)
        {
            scribe(loop.second.base);
            scribe(loop.second.value);
        }

        scribe(modifiers);

        CalculateEntries();
    }

    void ResourceAttributeTable::AddEntry(const Name &name, ValueT value)
    {
        map.emplace(name, Entry(value));
    }

    void ResourceAttributeTable::CalculateEntries()
    {
        for (auto &loop : modifiers)
        {
            auto& focusedEntry = map.find(loop.name)->second;
            if (loop.type == Modifier::Type::VALUE)
            {
                if (loop.op == OperatorSelector::ADD)
                {
                    // Check for going past max possible
                    if (loop.value > Entry::maxPossible - focusedEntry.value.Get())
                        focusedEntry.value = Entry::maxPossible;
                    else
                        focusedEntry.value += loop.value;
                }
                else if (loop.op == OperatorSelector::SUBTRACT)
                {
                    // Check for going past max possible
                    if (focusedEntry.value - loop.value > Entry::maxPossible)
                        focusedEntry.value = Entry::maxPossible;
                    else
                        focusedEntry.value -= loop.value;
                }
                else if (loop.op == OperatorSelector::MULTIPLY)
                {
                    FixedPoint64 entryValue(FixedPoint64::Split(focusedEntry.value.Get(), 0));
                    FixedPoint64 modValue(FixedPoint64::Split(loop.value, 0));
                    FixedPoint64 maxPossible(FixedPoint64::Split(Entry::maxPossible, 0));
                    // Check for going past max possible
                    if (modValue > maxPossible / entryValue)
                        focusedEntry.value = Entry::maxPossible;
                    else
                        focusedEntry.value *= loop.value;
                }
                else // DIVISION
                {
                    // Check for division by 0
                    if (loop.value == 0)
                        ;
                    // Check for going past 0
                    else if (loop.value < 0)
                        focusedEntry.value = 0;
                    else
                    {
                        FixedPoint64 entryValue(FixedPoint64::Split(focusedEntry.value.Get(), 0));
                        FixedPoint64 modValue(FixedPoint64::Split(loop.value, 0));
                        entryValue /= modValue;
                        entryValue.Floor();
                        focusedEntry.value = static_cast<ValueT>(entryValue);
                    }
                }
            }
            else
            {
                if (loop.op == OperatorSelector::ADD)
                {
                    // Check for going past max possible
                    if (loop.value > Entry::maxPossible - focusedEntry.value.GetUpperBound())
                        focusedEntry.value.SetUpperBound(Entry::maxPossible);
                    else
                        focusedEntry.value.SetUpperBound(focusedEntry.value.GetUpperBound() + loop.value);
                }
                else if (loop.op == OperatorSelector::SUBTRACT)
                {
                    // Check for going past max possible
                    if (focusedEntry.value.GetUpperBound() - loop.value > Entry::maxPossible)
                        focusedEntry.value.SetUpperBound(Entry::maxPossible);
                    else
                        focusedEntry.value.SetUpperBound(focusedEntry.value.GetUpperBound() - loop.value);
                }
                else if (loop.op == OperatorSelector::MULTIPLY)
                {
                    // Check for going past max possible
                    if (loop.value > Entry::maxPossible / focusedEntry.value.GetUpperBound())
                        focusedEntry.value.SetUpperBound(Entry::maxPossible);
                    else
                        focusedEntry.value.SetUpperBound(focusedEntry.value.GetUpperBound() * loop.value);
                }
                else // DIVISION
                {
                    // Check for division by 0
                    if (loop.value == 0)
                        ;
                    // Check for going past 0
                    else if (loop.value < 0)
                        focusedEntry.value.SetUpperBound(0);
                    else
                    {
                        FixedPoint64 entryValue(FixedPoint64::Split(focusedEntry.value.GetUpperBound(), 0));
                        FixedPoint64 modValue(FixedPoint64::Split(loop.value, 0));
                        entryValue /= modValue;
                        entryValue.Floor();
                        focusedEntry.value.SetUpperBound(static_cast<ValueT>(entryValue));
                    }
                }
            }
        }
    }

    ResourceAttributeTable::Entry& ResourceAttributeTable::GetHealthImpl()
    {
        return map.find(GlobalContext<ResourceAttributeTable>::health)->second;
    }

    const ResourceAttributeTable::Entry& ResourceAttributeTable::GetHealthImpl() const
    {
        return map.find(GlobalContext<ResourceAttributeTable>::health)->second;
    }

    ResourceAttributeTable::Entry& ResourceAttributeTable::GetManaImpl()
    {
        return map.find(GlobalContext<ResourceAttributeTable>::mana)->second;
    }

    const ResourceAttributeTable::Entry& ResourceAttributeTable::GetManaImpl() const
    {
        return map.find(GlobalContext<ResourceAttributeTable>::mana)->second;
    }

    ResourceAttributeTable::ResourceAttributeTable()
    {
        DataStandard<ResourceAttributeTable>::Setup(*this);
    }

    ResourceAttributeTable::ResourceAttributeTable(const RegistryObjectReference<CharacterClass> &charClass)
    {
        DataStandard<ResourceAttributeTable>::Setup(*this);
        SetCharacterClass(charClass);
    }

    bool ResourceAttributeTable::operator==(const ResourceAttributeTable &arg) const
    {
        return map == arg.map && modifiers == arg.modifiers;
    }

    bool ResourceAttributeTable::operator!=(const ResourceAttributeTable &arg) const
    {
        return !(*this == arg);
    }

    void ResourceAttributeTable::SetCharacterClass(const RegistryObjectReference<CharacterClass> &charClass)
    {
        for (auto& loop : charClass->resources)
            map.find(loop.first)->second.base = loop.second.base;
    }

    void ResourceAttributeTable::AddModifier(const Modifier &mod)
    {
        modifiers.emplace(mod);
        CalculateEntries();
    }

    void ResourceAttributeTable::RemoveModifier(const Modifier &mod)
    {
        modifiers.erase(mod);
        CalculateEntries();
    }

    Optional<ResourceAttributeTable::ValueT> ResourceAttributeTable::GetValue(const Name &name) const
    {
        typedef Optional<ValueT> Ret;

        auto found = map.find(name);
        if (found == map.end())
            return Ret();

        return Ret(found->second.value.Get());
    }

    Optional<ResourceAttributeTable::ValueT> ResourceAttributeTable::GetMax(const Name &name) const
    {
        typedef Optional<ValueT> Ret;

        auto found = map.find(name);
        if (found == map.end())
            return Ret();

        return Ret(found->second.value.GetUpperBound());
    }

    void ResourceAttributeTable::SetHealth(ValueT set)
    {
        GetHealthImpl().value.Set(set);
    }

    void ResourceAttributeTable::SetMana(ValueT set)
    {
        GetManaImpl().value.Set(set);
    }

    void ResourceAttributeTable::SetMaxHealth(ValueT set)
    {
        GetHealthImpl().value.SetUpperBound(set);
    }

    void ResourceAttributeTable::SetMaxMana(ValueT set)
    {
        GetManaImpl().value.SetUpperBound(set);
    }

    ResourceAttributeTable::ValueT ResourceAttributeTable::GetHealth() const
    {
        return GetHealthImpl().value.Get();
    }

    ResourceAttributeTable::ValueT ResourceAttributeTable::GetMana() const
    {
        return GetManaImpl().value.Get();
    }

    ResourceAttributeTable::ValueT ResourceAttributeTable::GetMaxHealth() const
    {
        return GetHealthImpl().value.GetUpperBound();
    }

    ResourceAttributeTable::ValueT ResourceAttributeTable::GetMaxMana() const
    {
        return GetManaImpl().value.GetUpperBound();
    }

    DataStandard<ResourceAttributeTable>::Entry::Entry()
    {}

    DataStandard<ResourceAttributeTable>::Entry::Entry(const Name &niceName) : niceName(niceName)
    {}

    void DataStandard<ResourceAttributeTable>::Setup(Object &object)
    {
        for (auto &loop : entries)
            object.AddEntry(loop.first, 0);
    }

    std::set<Name> DataStandard<ResourceAttributeTable>::GetNames()
    {
        std::set<Name> ret;
        for (auto &loop : entries)
            ret.emplace(loop.first);
        return ret;
    }

    Optional<Name> DataStandard<ResourceAttributeTable>::GetNiceName(const Name &name)
    {
        typedef Optional<Name> Ret;
        auto found = entries.Find(name);
        if (!found)
            return Ret();

        return Ret(found->niceName);
    }

    DataStandard<ResourceAttributeTable>::EntryMap<DataStandard<ResourceAttributeTable>::Entry> DataStandard<ResourceAttributeTable>::entries;

    void DataStandard<ResourceAttributeTable>::SetupObj(Object &object, const Name &name)
    {
        object.AddEntry(name, 0);
    }

    void DataStandard<ResourceAttributeTable>::SerializeImpl(::Inscription::Scribe &scribe)
    {
        ::Inscription::TrackingChangerStack tracking(scribe, false);
        if (scribe.IsOutput())
        {
            ::Inscription::ContainerSize size(entries.size());
            scribe.Save(size);

            for (auto &loop : entries)
            {
                scribe.Save(::Inscription::RemoveConst(loop.first));
                scribe.Save(loop.second.niceName);
            }
        }
        else // INPUT
        {
            ::Inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                Name name;
                scribe.Load(name);

                Name niceName;
                scribe.Load(niceName);

                entries.Add(name, Entry(niceName));
            }
        }
    }

    void GlobalContext<ResourceAttributeTable>::SerializeImpl(::Inscription::Scribe &scribe)
    {
        scribe(health);
        scribe(mana);
    }

    Name GlobalContext<ResourceAttributeTable>::health;
    Name GlobalContext<ResourceAttributeTable>::mana;
    Name GlobalContext<ResourceAttributeTable>::acumen;

    GlobalContext<ResourceAttributeTable>& GlobalContext<ResourceAttributeTable>::Instance()
    {
        static GlobalContext instance;
        return instance;
    }
}