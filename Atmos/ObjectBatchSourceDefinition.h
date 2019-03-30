#pragma once

#include "ObjectBatchSource.h"

namespace Atmos
{
    template<class T>
    ObjectBatchSource<T>::ObjectBatchSource(ObjectManager& manager) : manager(&manager)
    {
        SetupAssociatedDescriptionGroup();
        Synchronize();
    }

    template<class T>
    ObjectBatchSource<T>::ObjectBatchSource(const ::Inscription::Table<ObjectBatchSource>& table) : INSCRIPTION_TABLE_GET_MEM(manager)
    {
        SetupAssociatedDescriptionGroup();
        Synchronize();
    }

    template<class T>
    ObjectBatchSource<T>::~ObjectBatchSource()
    {
        onInvalidated();
    }

    template<class T>
    typename ObjectBatchSource<T>::iterator ObjectBatchSource<T>::RemoveObject(iterator itr)
    {
        if (itr == end())
            return end();

        auto next = std::next(itr);
        manager->DestroyObject(*itr);
        return next;
    }

    template<class T>
    typename ObjectBatchSource<T>::iterator ObjectBatchSource<T>::RemoveObject(const_iterator itr)
    {
        if (itr == end())
            return end();

        auto next = std::next(itr);
        manager->DestroyObject(*itr);
        return next;
    }

    template<class T>
    typename ObjectBatchSource<T>::Reference ObjectBatchSource<T>::FindObject(ObjectID id) const
    {
        for (auto& loop : list)
            if (loop->ID() == id && IsManaging(*loop))
                return loop;

        return nullptr;
    }

    template<class T>
    typename ObjectBatchSource<T>::SizeT ObjectBatchSource<T>::Size() const
    {
        return list.size();
    }

    template<class T>
    bool ObjectBatchSource<T>::IsEmpty() const
    {
        return Size() == 0;
    }

    template<class T>
    typename ObjectBatchSource<T>::iterator ObjectBatchSource<T>::begin()
    {
        return list.begin();
    }

    template<class T>
    typename ObjectBatchSource<T>::const_iterator ObjectBatchSource<T>::begin() const
    {
        return list.begin();
    }

    template<class T>
    typename ObjectBatchSource<T>::iterator ObjectBatchSource<T>::end()
    {
        return list.end();
    }

    template<class T>
    typename ObjectBatchSource<T>::const_iterator ObjectBatchSource<T>::end() const
    {
        return list.end();
    }

    template<class T>
    void ObjectBatchSource<T>::NotifyCreated(Object& object)
    {
        if (!IsManaging(object))
            return;

        auto found = FindObject(object.ID());
        if (found)
            return;

        auto reference = Reference(Downcast(&object));
        list.push_back(reference);
        onCreated(reference);
    }

    template<class T>
    void ObjectBatchSource<T>::NotifyDestroyed(Object& object)
    {
        if (!IsManaging(object))
            return;

        for (auto loop = list.begin(); loop != list.end(); ++loop)
        {
            if ((*loop)->ID() != object.ID())
                continue;

            onBeforeDestroyed(*loop);
            list.erase(loop);
            return;
        }
    }

    template<class T>
    void ObjectBatchSource<T>::IncrementReference()
    {
        ++referenceCount;
    }

    template<class T>
    void ObjectBatchSource<T>::DecrementReference()
    {
        --referenceCount;
        if (referenceCount == 0)
            manager->DestroyBatchSource<T>();
    }

    template<class T>
    void ObjectBatchSource<T>::SetupAssociatedDescriptionGroup()
    {
        associatedDescriptionGroup = manager->DescriptionGroupFor(TypeNameFor<T>());
    }

    template<class T>
    void ObjectBatchSource<T>::Synchronize()
    {
        for (auto& loop : manager->objects)
        {
            if (!IsManaging(*loop))
                continue;

            list.push_back(Downcast(loop.get()));
        }
    }

    template<class T>
    bool ObjectBatchSource<T>::IsManaging(const ObjectTypeName& checkAgainst) const
    {
        return associatedDescriptionGroup.IsIn(checkAgainst);
    }

    template<class T>
    bool ObjectBatchSource<T>::IsManaging(ObjectID checkAgainst) const
    {
        for (auto& loop : list)
            if (loop->ID() == checkAgainst)
                return true;

        return false;
    }

    template<class T>
    bool ObjectBatchSource<T>::IsManaging(const Object& checkAgainst) const
    {
        return IsManaging(checkAgainst.TypeDescription().typeName);
    }

    template<class T>
    T* ObjectBatchSource<T>::Downcast(Object* object)
    {
        return static_cast<T*>(object);
    }

    template<class T>
    T& ObjectBatchSource<T>::Downcast(Object& object)
    {
        return static_cast<T&>(object);
    }
}