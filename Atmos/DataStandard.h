#pragma once

#include <set>
#include <map>
#include <unordered_map>
#include <Chroma\Function.h>
#include <Chroma\Iterate.h>
#include "Name.h"

#include "Serialization.h"
#include <Inscription/TrackingTraits.h>

namespace Atmos
{
    template<class T>
    class DataStandardBase
    {
    public:
        typedef T Object;
        typedef size_t PieceIndex;
    };

    template<class T>
    class DataStandardInherit : public DataStandardBase<T>
    {
    public:
        constexpr static bool hasStandard = true;

        static void Serialize(::Inscription::Scribe &scribe);
    protected:
        // Sets up something with a standard set of names
        class NameSet
        {
        public:
            typedef T Object;
            typedef std::set<Name> Names;
            typedef Names::const_iterator const_iterator;
            typedef Names::size_type size_type;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            Names names;

            NameSet(const NameSet &arg) = delete;
            NameSet& operator=(const NameSet &arg) = delete;
        public:
            NameSet() = default;
            void Add(const Name &name);
            void Remove(const Name &name);
            const Name* Find(const Name &name) const;
            bool IsHere(const Name &name) const;

            const_iterator begin() const;
            const_iterator end() const;
            size_type size() const;
        };

        // Sets up something with a standard set of entries
        template<class Entry>
        class EntryMap
        {
        public:
            typedef T Object;
            typedef Entry EntryT;
        private:
            typedef std::map<Name, Entry> Entries;
        public:
            typedef typename Entries::iterator iterator;
            typedef typename Entries::const_iterator const_iterator;
            typedef typename Entries::size_type size_type;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            Entries entries;

            EntryMap(const EntryMap &arg) = delete;
            EntryMap& operator=(const EntryMap &arg) = delete;
        public:
            EntryMap();
            EntryT* Add(const Name &name, const EntryT &entry);
            EntryT* Add(const Name &name, EntryT &&entry);
            template<class... Args>
            EntryT* Add(const Name &name, Args && ... args);
            void Remove(const Name &name);
            const_iterator Remove(const_iterator itr);
            EntryT* Find(const Name &name);
            const EntryT* Find(const Name &name) const;
            bool IsHere(const Name &name) const;
            void Setup(Object &object);

            iterator begin();
            iterator end();
            const_iterator begin() const;
            const_iterator end() const;
            size_type size() const;
        };
    };

    template<class T>
    void DataStandardInherit<T>::Serialize(::Inscription::Scribe &scribe)
    {
        DataStandard<T>::SerializeImpl(scribe);
    }

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(DataStandardInherit<T>::NameSet)
    {
        scribe(names);
    }

    template<class T>
    void DataStandardInherit<T>::NameSet::Add(const Name &name)
    {
        names.emplace(name);
    }

    template<class T>
    void DataStandardInherit<T>::NameSet::Remove(const Name &name)
    {
        names.erase(name);
    }

    template<class T>
    const Name* DataStandardInherit<T>::NameSet::Find(const Name &name) const
    {
        auto found = names.find(name);
        if (found == names.end())
            return nullptr;

        return &*found;
    }

    template<class T>
    bool DataStandardInherit<T>::NameSet::IsHere(const Name &name) const
    {
        return names.find(name) != names.end();
    }

    template<class T>
    typename DataStandardInherit<T>::NameSet::const_iterator DataStandardInherit<T>::NameSet::begin() const
    {
        return names.begin();
    }

    template<class T>
    typename DataStandardInherit<T>::NameSet::const_iterator DataStandardInherit<T>::NameSet::end() const
    {
        return names.end();
    }

    template<class T>
    typename DataStandardInherit<T>::NameSet::size_type DataStandardInherit<T>::NameSet::size() const
    {
        return names.size();
    }

    template<class T>
    template<class Entry>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(DataStandardInherit<T>::EntryMap<Entry>)
    {
        scribe(entries);
    }

    template<class T>
    template<class Entry>
    DataStandardInherit<T>::EntryMap<Entry>::EntryMap()
    {}

    template<class T>
    template<class Entry>
    typename DataStandardInherit<T>::EntryMap<Entry>::EntryT* DataStandardInherit<T>::EntryMap<Entry>::Add(const Name &name, const EntryT &entry)
    {
        return &entries.emplace(name, entry).first->second;
    }

    template<class T>
    template<class Entry>
    typename DataStandardInherit<T>::EntryMap<Entry>::EntryT* DataStandardInherit<T>::EntryMap<Entry>::Add(const Name &name, EntryT &&entry)
    {
        return &entries.emplace(name, std::move(entry)).first->second;
    }

    template<class T>
    template<class Entry>
    template<class... Args>
    typename DataStandardInherit<T>::EntryMap<Entry>::EntryT* DataStandardInherit<T>::EntryMap<Entry>::Add(const Name &name, Args && ... args)
    {
        return &entries.emplace(name, EntryT(std::forward<Args>(args)...)).first->second;
    }

    template<class T>
    template<class Entry>
    void DataStandardInherit<T>::EntryMap<Entry>::Remove(const Name &name)
    {
        entries.erase(name);
    }

    template<class T>
    template<class Entry>
    typename DataStandardInherit<T>::EntryMap<Entry>::const_iterator DataStandardInherit<T>::EntryMap<Entry>::Remove(const_iterator itr)
    {
        return entries.erase(itr);
    }

    template<class T>
    template<class Entry>
    typename DataStandardInherit<T>::EntryMap<Entry>::EntryT* DataStandardInherit<T>::EntryMap<Entry>::Find(const Name &name)
    {
        auto found = entries.find(name);
        if (found == entries.end())
            return nullptr;

        return &found->second;
    }

    template<class T>
    template<class Entry>
    typename const DataStandardInherit<T>::EntryMap<Entry>::EntryT* DataStandardInherit<T>::EntryMap<Entry>::Find(const Name &name) const
    {
        auto found = entries.find(name);
        if (found == entries.end())
            return nullptr;

        return &found->second;
    }

    template<class T>
    template<class Entry>
    bool DataStandardInherit<T>::EntryMap<Entry>::IsHere(const Name &name) const
    {
        return entries.find(name) != entries.end();
    }

    template<class T>
    template<class Entry>
    void DataStandardInherit<T>::EntryMap<Entry>::Setup(Object &object)
    {
        for (auto& loop : entries)
            function.Execute(object, loop);
    }

    template<class T>
    template<class Entry>
    typename DataStandardInherit<T>::EntryMap<Entry>::iterator DataStandardInherit<T>::EntryMap<Entry>::begin()
    {
        return entries.begin();
    }

    template<class T>
    template<class Entry>
    typename DataStandardInherit<T>::EntryMap<Entry>::iterator DataStandardInherit<T>::EntryMap<Entry>::end()
    {
        return entries.end();
    }

    template<class T>
    template<class Entry>
    typename DataStandardInherit<T>::EntryMap<Entry>::const_iterator DataStandardInherit<T>::EntryMap<Entry>::begin() const
    {
        return entries.begin();
    }

    template<class T>
    template<class Entry>
    typename DataStandardInherit<T>::EntryMap<Entry>::const_iterator DataStandardInherit<T>::EntryMap<Entry>::end() const
    {
        return entries.end();
    }

    template<class T>
    template<class Entry>
    typename DataStandardInherit<T>::EntryMap<Entry>::size_type DataStandardInherit<T>::EntryMap<Entry>::size() const
    {
        return entries.size();
    }

    template<class T>
    class DataStandard : public DataStandardBase<T>
    {
    public:
        constexpr static bool hasStandard = false;
    };
}

namespace Inscription
{
    template<class T>
    struct TrackPointer<::Atmos::DataStandard<T>>
    {
        static constexpr TrackEnum value = TRACK_NEVER;
    };
}