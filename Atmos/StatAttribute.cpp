
#include "StatAttribute.h"

#include "CharacterClass.h"
#include "FixedPoint.h"

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>
#include <Inscription\ContainerSize.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(StatAttributeTable::Modifier)
    {
        scribe(name);
        scribe(value);
        scribe(op);
    }

    StatAttributeTable::Modifier::Modifier() : value(0), op(OperatorSelector::ADD)
    {}

    StatAttributeTable::Modifier::Modifier(const Name &name, Value value, OperatorSelector op) : name(name), value(value), op(op)
    {}

    bool StatAttributeTable::Modifier::operator==(const Modifier &arg) const
    {
        return name == arg.name && value == arg.value && op == arg.op;
    }

    bool StatAttributeTable::Modifier::operator!=(const Modifier &arg) const
    {
        return !(*this == arg);
    }

    StatAttributeTable::Entry::Entry(Value base) : base(base), value(base)
    {}

    bool StatAttributeTable::Entry::operator==(const Entry &arg) const
    {
        return base == arg.base && value == arg.value;
    }

    bool StatAttributeTable::Entry::operator!=(const Entry &arg) const
    {
        return !(*this == arg);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(StatAttributeTable)
    {
        for (auto& loop : map)
        {
            scribe(loop.second.base);
            scribe(loop.second.value);
        }

        scribe(modifiers);

        CalculateEntries();
    }

    void StatAttributeTable::AddEntry(const Name &name, Value value)
    {
        map.emplace(name, Entry(value));
    }

    void StatAttributeTable::CalculateEntries()
    {
        for (auto& modLoop : modifiers)
        {
            auto &focusedEntry = map.find(modLoop.name)->second;
            switch (modLoop.op)
            {
            case OperatorSelector::ADD:
                focusedEntry.value += modLoop.value;
                break;
            case OperatorSelector::SUBTRACT:
                focusedEntry.value -= modLoop.value;
                break;
            case OperatorSelector::MULTIPLY:
                focusedEntry.value *= modLoop.value;
                break;
            case OperatorSelector::DIVIDE:
                focusedEntry.value /= modLoop.value;
                break;
            }
        }
    }

    StatAttributeTable::StatAttributeTable()
    {
        DataStandard<StatAttributeTable>::Setup(*this);
    }

    StatAttributeTable::StatAttributeTable(const RegistryObjectReference<CharacterClass> &charClass)
    {
        DataStandard<StatAttributeTable>::Setup(*this);
        SetCharacterClass(charClass);
    }

    bool StatAttributeTable::operator==(const StatAttributeTable &arg) const
    {
        return map == arg.map && modifiers == arg.modifiers;
    }

    bool StatAttributeTable::operator!=(const StatAttributeTable &arg) const
    {
        return !(*this == arg);
    }

    void StatAttributeTable::SetCharacterClass(const RegistryObjectReference<CharacterClass> &charClass)
    {
        for (auto& loop : charClass->stats)
            map.find(loop.first)->second.base = loop.second.base;
    }

    void StatAttributeTable::AddModifier(const Modifier &mod)
    {
        modifiers.emplace(mod);
        CalculateEntries();
    }

    void StatAttributeTable::RemoveModifier(const Modifier &mod)
    {
        modifiers.erase(mod);
        CalculateEntries();
    }

    Optional<StatAttributeTable::Value> StatAttributeTable::GetValue(const Name &name) const
    {
        typedef Optional<Value> Ret;

        auto found = map.find(name);
        if (found == map.end())
            return Ret();

        return Ret(found->second.value);
    }

    DataStandard<StatAttributeTable>& DataStandard<StatAttributeTable>::Instance()
    {
        static DataStandard instance;
        return instance;
    }

    void DataStandard<StatAttributeTable>::Setup(Object &object)
    {
        for (auto &loop : entries)
            object.AddEntry(loop.first, 0);
    }

    DataStandard<StatAttributeTable>::Entry* DataStandard<StatAttributeTable>::Add(const Name &name, const Name &niceName, const Name &opposing)
    {
        return entries.Add(name, Entry(name, niceName, opposing));
    }

    DataStandard<StatAttributeTable>::Entry* DataStandard<StatAttributeTable>::Add(Entry &&add)
    {
        return entries.Add(add.name, std::move(add));
    }

    void DataStandard<StatAttributeTable>::Remove(const Name &name)
    {
        entries.Remove(name);
    }

    DataStandard<StatAttributeTable>::const_iterator DataStandard<StatAttributeTable>::Remove(const_iterator itr)
    {
        return entries.Remove(itr);
    }

    DataStandard<StatAttributeTable>::Entry* DataStandard<StatAttributeTable>::Find(const Name &name)
    {
        return entries.Find(name);
    }

    std::set<Name> DataStandard<StatAttributeTable>::GetNames()
    {
        std::set<Name> ret;
        for (auto &loop : entries)
            ret.emplace(loop.first);
        return ret;
    }

    Optional<Name> DataStandard<StatAttributeTable>::GetNiceName(const Name &stat)
    {
        auto found = entries.Find(stat);
        if (!found)
            return Optional<Name>();

        return Optional<Name>(found->niceName);
    }

    Optional<Name> DataStandard<StatAttributeTable>::GetOpposing(const Name &stat)
    {
        auto found = entries.Find(stat);
        if (!found)
            return Optional<Name>();

        return Optional<Name>(found->opposing);
    }

    DataStandard<StatAttributeTable>::const_iterator DataStandard<StatAttributeTable>::begin() const
    {
        return entries.begin();
    }

    DataStandard<StatAttributeTable>::const_iterator DataStandard<StatAttributeTable>::end() const
    {
        return entries.end();
    }

    DataStandard<StatAttributeTable>::Entry::Entry(const Name &name, const Name &niceName, const Name &opposing) : name(name), niceName(niceName), opposing(opposing)
    {}

    bool DataStandard<StatAttributeTable>::Entry::operator==(const Entry &arg) const
    {
        return name == arg.name;
    }

    bool DataStandard<StatAttributeTable>::Entry::operator!=(const Entry &arg) const
    {
        return !(*this == arg);
    }

    DataStandard<StatAttributeTable>::EntryMap<DataStandard<StatAttributeTable>::Entry> DataStandard<StatAttributeTable>::entries;

    void DataStandard<StatAttributeTable>::SetupObj(Object &object, const Name &name, Entry &entry)
    {
        object.AddEntry(name, 0);
        object.AddEntry(entry.opposing, 0);
    }

    void DataStandard<StatAttributeTable>::SerializeImpl(::inscription::Scribe &scribe)
    {
        ::inscription::TrackingChangerStack tracking(scribe, false);

        if (scribe.IsOutput())
        {
            ::inscription::ContainerSize size(entries.size());
            scribe.Save(size);

            for (auto &loop : entries)
            {
                scribe.Save(::inscription::RemoveConst(loop.first));
                scribe.Save(loop.second.niceName);
                scribe.Save(loop.second.opposing);
            }
        }
        else // INPUT
        {
            ::inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                Name name;
                scribe.Load(name);

                Name niceName;
                scribe.Load(niceName);

                Name opposing;
                scribe.Load(opposing);

                entries.Add(name, Entry(name, niceName, opposing));
            }
        }
    }
}