#pragma once

#include <unordered_map>

#include "ObjectReference.h"
#include "ObjectMapIterator.h"
#include "ObjectStaticAssert.h"

namespace Atmos
{
    template<class T>
    class ObjectMap
    {
    public:
        typedef TypedObjectReference<T> Reference;
        typedef size_t SizeT;
    private:
        typedef std::unordered_map<ObjectID, Reference> ReferenceMap;

        typedef typename ReferenceMap::iterator BasicIterator;
        typedef typename ReferenceMap::const_iterator ConstBasicIterator;
    public:
        typedef ObjectMapIteratorBase<Reference, BasicIterator> iterator;
        typedef ObjectMapIteratorBase<const Reference, ConstBasicIterator> const_iterator;
    public:
        ObjectMap();

        void Add(Reference add);
        void Insert(iterator from, iterator to);

        bool Remove(ObjectID remove);
        bool Remove(Reference remove);
        bool Remove(iterator remove);
        bool Remove(const_iterator remove);
        void Clear();

        iterator Find(ObjectID check);
        const_iterator Find(ObjectID check) const;

        bool Has(ObjectID check) const;
        bool Has(Reference check) const;

        SizeT Size() const;
        bool IsEmpty() const;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
    private:
        ReferenceMap map;
    private:
        bool RemoveByID(ObjectID remove);
        bool RemoveByIterator(BasicIterator remove);
    private:
        iterator Wrap(BasicIterator itr);
        const_iterator Wrap(ConstBasicIterator itr) const;
        BasicIterator Unwrap(iterator itr);
        ConstBasicIterator Unwrap(const_iterator itr) const;
    private:
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);
    };

    template<class T>
    ObjectMap<T>::ObjectMap()
    {}

    template<class T>
    void ObjectMap<T>::Add(Reference add)
    {
        map.emplace(add->ID(), add);
    }

    template<class T>
    void ObjectMap<T>::Insert(iterator from, iterator to)
    {
        map.insert(Unwrap(from), Unwrap(to));
    }

    template<class T>
    bool ObjectMap<T>::Remove(ObjectID remove)
    {
        return RemoveByID(remove);
    }

    template<class T>
    bool ObjectMap<T>::Remove(Reference remove)
    {
        return RemoveByID(remove->ID());
    }

    template<class T>
    bool ObjectMap<T>::Remove(iterator remove)
    {
        return RemoveByIterator(Unwrap(remove));
    }

    template<class T>
    bool ObjectMap<T>::Remove(const_iterator remove)
    {
        RemoveByIterator(remove);
    }

    template<class T>
    void ObjectMap<T>::Clear()
    {
        map.clear();
    }

    template<class T>
    typename ObjectMap<T>::iterator ObjectMap<T>::Find(ObjectID check)
    {
        return iterator(Wrap(map.find(check)));
    }

    template<class T>
    typename ObjectMap<T>::const_iterator ObjectMap<T>::Find(ObjectID check) const
    {
        return const_iterator(Wrap(map.find(check)));
    }

    template<class T>
    bool ObjectMap<T>::Has(ObjectID check) const
    {
        return Unwrap(Find(check)) != map.end();
    }

    template<class T>
    bool ObjectMap<T>::Has(Reference check) const
    {
        return Has(check->ID());
    }

    template<class T>
    typename ObjectMap<T>::SizeT ObjectMap<T>::Size() const
    {
        return map.size();
    }

    template<class T>
    bool ObjectMap<T>::IsEmpty() const
    {
        return map.empty();
    }

    template<class T>
    typename ObjectMap<T>::iterator ObjectMap<T>::begin()
    {
        return Wrap(map.begin());
    }

    template<class T>
    typename ObjectMap<T>::const_iterator ObjectMap<T>::begin() const
    {
        return Wrap(map.begin());
    }

    template<class T>
    typename ObjectMap<T>::iterator ObjectMap<T>::end()
    {
        return Wrap(map.end());
    }

    template<class T>
    typename ObjectMap<T>::const_iterator ObjectMap<T>::end() const
    {
        return Wrap(map.end());
    }

    template<class T>
    bool ObjectMap<T>::RemoveByID(ObjectID remove)
    {
        return map.erase(remove) != 0;
    }

    template<class T>
    bool ObjectMap<T>::RemoveByIterator(BasicIterator remove)
    {
        return RemoveByID(remove.first);
    }

    template<class T>
    typename ObjectMap<T>::iterator ObjectMap<T>::Wrap(BasicIterator itr)
    {
        return iterator(itr);
    }

    template<class T>
    typename ObjectMap<T>::const_iterator ObjectMap<T>::Wrap(ConstBasicIterator itr) const
    {
        return const_iterator(itr);
    }

    template<class T>
    typename ObjectMap<T>::BasicIterator ObjectMap<T>::Unwrap(iterator itr)
    {
        return itr.wrapped;
    }

    template<class T>
    typename ObjectMap<T>::ConstBasicIterator ObjectMap<T>::Unwrap(const_iterator itr) const
    {
        return itr.wrapped;
    }
}