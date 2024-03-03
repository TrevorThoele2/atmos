
#include "Element.h"

#include "CombatComponent.h"

#include "Error.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Element)
    {
        scribe(name);
    }

    Element::Element(const Name &name) : name(name)
    {}

    Element& Element::operator=(const Name &name)
    {
        this->name = name;
        return *this;
    }

    bool Element::operator==(const Element &arg) const
    {
        return name == arg.name;
    }

    bool Element::operator!=(const Element &arg) const
    {
        return !(*this == arg);
    }

    Element::operator const Name&() const
    {
        return name;
    }

    void Element::Set(const Name &set)
    {
        name = set;
    }

    const Name& Element::Get() const
    {
        return name;
    }

    Element::Multiplier Element::GetAdvantageAgainst(const Element &against) const
    {
        return GlobalContext<Element>::GetAdvantage(*this, against);
    }

    Element::Multiplier Element::GetSameMultiplier(const Element &main) const
    {
        // For when the damage is the same as the element of the character
        const FixedPoint64 DAMAGE_MULTIPLIER_SAME = FixedPoint64::Split(1, FixedPoint64::Split::AdjustF(1, FixedPoint64::GetDefaultRadixPoint()));
        // For when the damage is opposing the character's
        const FixedPoint64 DAMAGE_MULTIPLIER_OPPOSING = FixedPoint64::Split(0, FixedPoint64::Split::AdjustF(9, FixedPoint64::GetDefaultRadixPoint()));

        if (*this == main)
            return DAMAGE_MULTIPLIER_SAME;
        else
            return DAMAGE_MULTIPLIER_OPPOSING;
    }

    GlobalContext<Element>::Entry::Entry(const Name &name, const Name &niceName, const Element &opposing, const Name &statName) : name(name), niceName(niceName), opposing(opposing), statName(statName)
    {}

    bool GlobalContext<Element>::Entry::operator==(const Entry &arg) const
    {
        return name == arg.name;
    }

    bool GlobalContext<Element>::Entry::operator!=(const Entry &arg) const
    {
        return !(*this == arg);
    }

    std::unordered_map<Element, GlobalContext<Element>::Entry> GlobalContext<Element>::entries;

    void GlobalContext<Element>::SerializeImpl(::inscription::Scribe &scribe)
    {
        if (scribe.IsOutput())
        {
            ::inscription::ContainerSize size(entries.size());
            scribe.Save(size);

            for (auto &loop : entries)
            {
                scribe.Save(::inscription::RemoveConst(loop.first));
                scribe.Save(loop.second.niceName);
                scribe.Save(loop.second.opposing);
                scribe.Save(loop.second.statName);
            }
        }
        else // INPUT
        {
            ::inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                Element element;
                scribe.Load(element);

                Name niceName;
                scribe.Load(niceName);

                Element opposing;
                scribe.Load(opposing);

                Name statName;
                scribe.Load(statName);
                entries.emplace(element, Entry(element.Get(), niceName, std::move(opposing), std::move(statName)));
            }
        }
    }

    Name GlobalContext<Element>::neutral;

    GlobalContext<Element>& GlobalContext<Element>::Instance()
    {
        static GlobalContext instance;
        return instance;
    }

    GlobalContext<Element>::Entry* GlobalContext<Element>::Add(const Name &name, const Name &niceName, const Element &opposing, const Name &statName)
    {
        return &entries.emplace(name, Entry(name, niceName, opposing, statName)).first->second;
    }

    GlobalContext<Element>::Entry* GlobalContext<Element>::Add(Entry &&add)
    {
        return &entries.emplace(add.name, std::move(add)).first->second;
    }

    void GlobalContext<Element>::Remove(const Name &name)
    {
        entries.erase(name);
    }

    GlobalContext<Element>::const_iterator GlobalContext<Element>::Remove(const_iterator itr)
    {
        return entries.erase(itr);
    }

    GlobalContext<Element>::Entry* GlobalContext<Element>::Find(const Name &name)
    {
        auto found = entries.find(name);
        if (found == entries.end())
            return nullptr;

        return &found->second;
    }

    GlobalContext<Element>::Entries& GlobalContext<Element>::GetEntries()
    {
        return entries;
    }

    GlobalContext<Element>::Multiplier GlobalContext<Element>::GetAdvantage(const Element &main, const Element &against)
    {
        const FixedPoint64 DAMAGE_MULTIPLIER_ADVANTAGE = FixedPoint64::Split(2, 0);
        const FixedPoint64 DAMAGE_MULTIPLIER_NORMAL = FixedPoint64::Split(1, 0);
        const FixedPoint64 DAMAGE_MULTIPLIER_DISADVANTAGE = FixedPoint64::Split(0, FixedPoint64::Split::AdjustF(5, FixedPoint64::GetDefaultRadixPoint()));

        auto found = entries.find(main);
        auto againstFound = entries.find(against);

        // Error checking
        {
            const String str("An element that isn't inside of element's data context is attempting to be found.");
            if (found == entries.end())
            {
                ErrorHandler::Log(str,
                    ErrorHandler::Severity::ERROR_LOW,
                    ErrorHandler::NameValueVector{ NameValuePair("Element Name", main.Get()) });
                return DAMAGE_MULTIPLIER_NORMAL;
            }

            if (againstFound == entries.end())
            {
                ErrorHandler::Log(str,
                    ErrorHandler::Severity::ERROR_LOW,
                    ErrorHandler::NameValueVector{ NameValuePair("Element Name", against.Get()) });
                return DAMAGE_MULTIPLIER_NORMAL;
            }
        }

        if (found->second.opposing == against)
            return DAMAGE_MULTIPLIER_ADVANTAGE;
        else if (againstFound->second.opposing == main)
            return DAMAGE_MULTIPLIER_DISADVANTAGE;

        return DAMAGE_MULTIPLIER_NORMAL;
    }

    Optional<Name> GlobalContext<Element>::GetStatName(const Element &check)
    {
        typedef Optional<Name> Ret;

        auto found = entries.find(check);
        // Error checking
        if (found == entries.end())
        {
            String str("An element that isn't inside of element's data context is attempting to be found.");
            ErrorHandler::Log(str,
                ErrorHandler::Severity::ERROR_LOW,
                ErrorHandler::NameValueVector{ NameValuePair("Element Name", check.Get()) });
            return Ret();
        }

        return Ret(found->second.statName);
    }

    GlobalContext<Element>::const_iterator GlobalContext<Element>::begin() const
    {
        return entries.begin();
    }

    GlobalContext<Element>::const_iterator GlobalContext<Element>::end() const
    {
        return entries.end();
    }
}