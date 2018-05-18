
#include "CharacterClassGroup.h"

#include "CharacterClass.h"
#include <Inscription/ContainerSize.h>
#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(CharacterClassGroup)
    {
        inscription::TrackingChangerStack changer(scribe, false);
        if (scribe.IsOutput())
        {
            scribe.Save(usesAll);
            if (!usesAll)
            {
                inscription::ContainerSize size(set.size());
                scribe.Save(size);

                for (auto &loop : set)
                    scribe.Save(::inscription::RemoveConst(loop));
            }
        }
        else
        {
            scribe.Load(usesAll);
            if (!usesAll)
            {
                inscription::ContainerSize size;
                scribe.Load(size);

                while (size-- > 0)
                {
                    RegistryObjectReference<CharacterClass> charClass;
                    scribe.Load(charClass);
                    set.emplace(charClass);
                }
            }
        }
    }

    CharacterClassGroup::CharacterClassGroup(bool usesAll) : usesAll(usesAll)
    {}

    CharacterClassGroup::CharacterClassGroup(CharacterClassGroup &&arg) : usesAll(arg.usesAll), set(std::move(arg.set))
    {}

    CharacterClassGroup& CharacterClassGroup::operator=(CharacterClassGroup &&arg)
    {
        set = std::move(arg.set);
        return *this;
    }

    bool CharacterClassGroup::operator==(const CharacterClassGroup &arg) const
    {
        return set == arg.set;
    }

    bool CharacterClassGroup::operator!=(const CharacterClassGroup &arg) const
    {
        return !(*this == arg);
    }

    void CharacterClassGroup::Add(const RegistryObjectReference<CharacterClass> &add)
    {
        set.emplace(add);
    }

    void CharacterClassGroup::Remove(const RegistryObjectReference<CharacterClass> &remove)
    {
        set.erase(remove);
    }

    void CharacterClassGroup::Clear()
    {
        set.clear();
    }

    void CharacterClassGroup::UsesAll(bool set)
    {
        if (set)
            Clear();

        usesAll = set;
    }

    bool CharacterClassGroup::UsesAll() const
    {
        return usesAll;
    }

    bool CharacterClassGroup::In(const CharacterClass &find) const
    {
        if (usesAll)
            return true;

        auto found = set.find(&const_cast<CharacterClass&>(find));
        if (found != set.end())
            return true;

        return false;
    }

    CharacterClassGroup::size_type CharacterClassGroup::size() const
    {
        return set.size();
    }

    bool CharacterClassGroup::empty() const
    {
        return set.empty();
    }

    CharacterClassGroup::const_iterator CharacterClassGroup::begin() const
    {
        return set.begin();
    }

    CharacterClassGroup::const_iterator CharacterClassGroup::end() const
    {
        return set.end();
    }
}