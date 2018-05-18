#pragma once

#include <unordered_map>

#include "RegistryObject.h"
#include "RegistryObjectReference.h"

#include "Name.h"
#include "IDSequencer.h"

#include "Serialization.h"
#include <Inscription/Scribe.h>

namespace Atmos
{
    template<class T, class Mixin>
    class RegistryBase
    {
    public:
        typedef T ObjectT;
        typedef RegistryObjectReference<ObjectT> Reference;
        typedef typename ObjectT::ID ID;

        static const ID nullID = ObjectT::nullID;
        static const ID startID = nullID + 1;
    private:
        typedef std::list<ObjectT> List;
    public:
        typedef typename List::size_type SizeT;
        typedef typename List::const_iterator const_iterator;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        static List list;

        typedef std::unordered_map<Name, typename List::iterator> NameMap;
        static NameMap nameMap;

        static IDSequencer<ID> idSequencer;
        typedef std::unordered_map<ID, typename List::iterator> IDMap;
        static IDMap idMap;

        RegistryBase(const RegistryBase &arg) = delete;
        RegistryBase& operator=(const RegistryBase &arg) = delete;
    protected:
        RegistryBase() = default;
    public:
        static Mixin& Instance();
        // Will return the object already here if there's one for this name
        static Reference Register(const Name &name);
        // Will return the object already here if there's one for this name
        static Reference Register(ID id, const Name &name);
        // Will return true if there was an object that was removed
        static bool Remove(const Name &name);
        // Will return true if there was an object that was removed
        static bool Remove(ID id);
        static const_iterator Remove(const_iterator itr);
        static void Clear();
        // Will return nullptr if the object was not found
        static Reference Find(const Name &name);
        // Will return nullptr if the object was not found
        static Reference Find(ID id);

        static void ChangeName(const Name &from, const Name &to);
        static void ChangeName(const Reference &from, const Name &to);

        static SizeT Size();
        static bool Empty();
        const_iterator begin();
        const_iterator end();
    };

    template<class T, class Mixin>
    void RegistryBase<T, Mixin>::Serialize(::inscription::Scribe &scribe)
    {
        ::inscription::TrackingChangerStack tracking(scribe, false);
        if (scribe.IsOutput())
        {
            inscription::ContainerSize size(Size());
            scribe.Save(size);

            for (auto &loop : list)
            {
                // Save the name
                scribe.Save(::inscription::RemoveConst(loop));
                // Save the ID
                scribe.Save(loop.GetID());
                // Save the object
                scribe.Save(loop);
            }
        }
        else
        {
            list.clear();
            nameMap.clear();
            idMap.clear();

            inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                // Load the name
                Name name;
                scribe.Load(name);

                // Load the ID
                ID id;
                scribe.Load(id);

                // Load the object
                ObjectT object;
                scribe.Load(object);

                object.name = name;
                object.id = id;

                // Emplace the object
                list.push_back(std::move(object));
                auto emplaced = --list.end();
                nameMap.emplace(name, emplaced);
                idMap.emplace(id, emplaced);
            }
        }
    }

    template<class T, class Mixin>
    typename RegistryBase<T, Mixin>::List RegistryBase<T, Mixin>::list;
    template<class T, class Mixin>
    typename RegistryBase<T, Mixin>::NameMap RegistryBase<T, Mixin>::nameMap;
    template<class T, class Mixin>
    IDSequencer<typename RegistryBase<T, Mixin>::ID> RegistryBase<T, Mixin>::idSequencer(startID);
    template<class T, class Mixin>
    typename RegistryBase<T, Mixin>::IDMap RegistryBase<T, Mixin>::idMap;

    template<class T, class Mixin>
    Mixin& RegistryBase<T, Mixin>::Instance()
    {
        static Mixin instance;
        return instance;
    }

    template<class T, class Mixin>
    typename RegistryBase<T, Mixin>::Reference RegistryBase<T, Mixin>::Register(const Name &name)
    {
        auto found = nameMap.find(name);
        if (found != nameMap.end())
            return Reference(&*found->second);

        list.push_back(ObjectT());
        auto registered = --list.end();

        registered->id = idSequencer.ReserveNext();
        registered->name = name;
        registered->niceName = name;

        nameMap.emplace(name, registered).first;
        idMap.emplace(registered->id, registered); 

        return Reference(*registered);
    }

    template<class T, class Mixin>
    typename RegistryBase<T, Mixin>::Reference RegistryBase<T, Mixin>::Register(ID id, const Name &name)
    {
        auto found = nameMap.find(name);
        if (found != nameMap.end())
            return Reference(&*found->second);

        list.push_back(ObjectT());
        auto registered = --list.end();

        // Figure out the ID to use
        if (id <= nullID)
            registered->id = idSequencer.ReserveNext();
        else
        {
            idSequencer.Reserve(id);
            registered->id = id;
        }

        registered->name = name;
        registered->niceName = name;

        nameMap.emplace(name, registered).first;
        idMap.emplace(registered->id, registered);

        return Reference(*registered);
    }

    template<class T, class Mixin>
    bool RegistryBase<T, Mixin>::Remove(const Name &name)
    {
        auto found = nameMap.find(id);
        if (found == nameMap.end())
            return false;

        return Remove(found->second) != list.end();
    }

    template<class T, class Mixin>
    bool RegistryBase<T, Mixin>::Remove(ID id)
    {
        auto found = idMap.find(id);
        if (found == idMap.end())
            return false;

        return Remove(found->second) != list.end();
    }

    template<class T, class Mixin>
    typename RegistryBase<T, Mixin>::const_iterator RegistryBase<T, Mixin>::Remove(const_iterator itr)
    {
        if (itr == list.end())
            return list.end();

        nameMap.erase(itr->GetName());
        idMap.erase(itr->GetID());
        return list.erase(itr);
    }

    template<class T, class Mixin>
    void RegistryBase<T, Mixin>::Clear()
    {
        list.clear();
        nameMap.clear();
        idMap.clear();
    }

    template<class T, class Mixin>
    typename RegistryBase<T, Mixin>::Reference RegistryBase<T, Mixin>::Find(const Name &name)
    {
        auto found = nameMap.find(name);
        if (found == nameMap.end())
            return nullptr;

        return Reference(&*found->second);
    }

    template<class T, class Mixin>
    typename RegistryBase<T, Mixin>::Reference RegistryBase<T, Mixin>::Find(ID id)
    {
        auto found = idMap.find(id);
        if (found == idMap.end())
            return nullptr;

        return Reference(&*found->second);
    }

    template<class T, class Mixin>
    void RegistryBase<T, Mixin>::ChangeName(const Name &from, const Name &to)
    {
        ChangeName(Find(from), to);
    }

    template<class T, class Mixin>
    void RegistryBase<T, Mixin>::ChangeName(const Reference &from, const Name &to)
    {
        if (!from)
            return;

        if (nameMap.find(to) != nameMap.end())
            return;

        auto itr = nameMap.find(from->GetName());
        auto secondItr = itr->second;
        ATMOS_ASSERT_MESSAGE(itr != nameMap.end(), "This iterator should always exist.");
        nameMap.erase(from->GetName());
        nameMap.emplace(to, secondItr);
        secondItr->name = to;
    }

    template<class T, class Mixin>
    typename RegistryBase<T, Mixin>::SizeT RegistryBase<T, Mixin>::Size()
    {
        return list.size();
    }

    template<class T, class Mixin>
    bool RegistryBase<T, Mixin>::Empty()
    {
        return list.empty();
    }

    template<class T, class Mixin>
    typename RegistryBase<T, Mixin>::const_iterator RegistryBase<T, Mixin>::begin()
    {
        return list.begin();
    }

    template<class T, class Mixin>
    typename RegistryBase<T, Mixin>::const_iterator RegistryBase<T, Mixin>::end()
    {
        return list.end();
    }

    template<class T>
    class Registry;
}

namespace inscription
{
    template<class T>
    struct TrackPointer<::Atmos::Registry<T>>
    {
        static constexpr TrackEnum value = TRACK_NEVER;
    };
}