#pragma once

#include <vector>

#include "ObjectID.h"
#include "Object.h"
#include "ObjectReference.h"
#include "ObjectStaticAssert.h"

#include "ObjectBatchSource.h"
#include "ObjectBatchException.h"

#include "Event.h"
#include "ScopedEventConnectionList.h"

namespace Atmos
{
    typedef size_t ObjectBatchSizeT;

    template<class T>
    class ObjectBatch
    {
    public:
        typedef TypedObjectReference<T> Reference;
    private:
        typedef ObjectBatchSource<T> SourceT;
    public:
        typedef typename ObjectBatchSizeT SizeT;
        typedef typename SourceT::iterator iterator;
        typedef typename SourceT::const_iterator const_iterator;
    public:
        Event<Reference> onCreated;

        Event<Reference> onBeforeDestroyed;
    public:
        ObjectBatch();
        ObjectBatch(SourceT& source);
        ObjectBatch(const ObjectBatch& arg);
        ObjectBatch(ObjectBatch&& arg);
        ~ObjectBatch();

        ObjectBatch& operator=(const ObjectBatch& arg);
        ObjectBatch& operator=(ObjectBatch&& arg);

        iterator RemoveObject(iterator itr);
        iterator RemoveObject(const_iterator itr);

        Reference FindObject(ObjectID id);

        SizeT Size() const;
        bool IsEmpty() const;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
    private:
        SourceT* source;

        void IncrementSource();
        void DecrementSource();
    private:
        bool isInvalid;

        void Invalidate();
        void ThrowIfInvalid() const;
    private:
        ScopedEventConnectionList sourceConnections;

        void SubscribeToSource();
        void UnsubscribeFromSource();
        void OnCreated(Reference reference);
        void OnBeforeDestroyed(Reference reference);
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);
    };

    template<class T>
    ObjectBatch<T>::ObjectBatch() : source(nullptr), isInvalid(false)
    {}

    template<class T>
    ObjectBatch<T>::ObjectBatch(SourceT& source) : source(&source), isInvalid(false)
    {
        IncrementSource();
        SubscribeToSource();
    }

    template<class T>
    ObjectBatch<T>::ObjectBatch(const ObjectBatch& arg) : source(arg.source), isInvalid(false)
    {
        IncrementSource();
        SubscribeToSource();
    }

    template<class T>
    ObjectBatch<T>::ObjectBatch(ObjectBatch&& arg) : source(std::move(arg.source)), isInvalid(false)
    {
        arg.source = nullptr;
        SubscribeToSource();
    }

    template<class T>
    ObjectBatch<T>::~ObjectBatch()
    {
        if (isInvalid)
            return;

        DecrementSource();
        UnsubscribeFromSource();
    }

    template<class T>
    ObjectBatch<T>& ObjectBatch<T>::operator=(const ObjectBatch& arg)
    {
        UnsubscribeFromSource();
        source = arg.source;
        source->IncrementReference();
        SubscribeToSource();
        return *this;
    }

    template<class T>
    ObjectBatch<T>& ObjectBatch<T>::operator=(ObjectBatch&& arg)
    {
        UnsubscribeFromSource();
        source = std::move(arg.source);
        arg.source = nullptr;
        SubscribeToSource();
        return *this;
    }

    template<class T>
    typename ObjectBatch<T>::iterator ObjectBatch<T>::RemoveObject(iterator itr)
    {
        ThrowIfInvalid();

        return source->RemoveObject(itr);
    }

    template<class T>
    typename ObjectBatch<T>::iterator ObjectBatch<T>::RemoveObject(const_iterator itr)
    {
        ThrowIfInvalid();

        return source->RemoveObject(itr);
    }

    template<class T>
    typename ObjectBatch<T>::Reference ObjectBatch<T>::FindObject(ObjectID id)
    {
        ThrowIfInvalid();

        return source->FindObject(id);
    }

    template<class T>
    typename ObjectBatch<T>::SizeT ObjectBatch<T>::Size() const
    {
        ThrowIfInvalid();

        return source->Size();
    }

    template<class T>
    bool ObjectBatch<T>::IsEmpty() const
    {
        ThrowIfInvalid();

        return source->IsEmpty();
    }

    template<class T>
    typename ObjectBatch<T>::iterator ObjectBatch<T>::begin()
    {
        ThrowIfInvalid();

        return source->begin();
    }

    template<class T>
    typename ObjectBatch<T>::const_iterator ObjectBatch<T>::begin() const
    {
        ThrowIfInvalid();

        return source->begin();
    }

    template<class T>
    typename ObjectBatch<T>::iterator ObjectBatch<T>::end()
    {
        ThrowIfInvalid();

        return source->end();
    }

    template<class T>
    typename ObjectBatch<T>::const_iterator ObjectBatch<T>::end() const
    {
        ThrowIfInvalid();

        return source->end();
    }

    template<class T>
    void ObjectBatch<T>::IncrementSource()
    {
        if (!source)
            return;

        source->IncrementReference();
    }

    template<class T>
    void ObjectBatch<T>::DecrementSource()
    {
        if (!source)
            return;

        source->DecrementReference();
    }

    template<class T>
    void ObjectBatch<T>::Invalidate()
    {
        isInvalid = true;
    }

    template<class T>
    void ObjectBatch<T>::ThrowIfInvalid() const
    {
        if (isInvalid)
            throw ObjectBatchInvalidatedException();
    }

    template<class T>
    void ObjectBatch<T>::SubscribeToSource()
    {
        if (!source)
            return;

        sourceConnections.Add(source->onInvalidated.Subscribe(&ObjectBatch::Invalidate, *this));
        sourceConnections.Add(source->onCreated.Subscribe(&ObjectBatch::OnCreated, *this));
        sourceConnections.Add(source->onBeforeDestroyed.Subscribe(&ObjectBatch::OnBeforeDestroyed, *this));
    }

    template<class T>
    void ObjectBatch<T>::UnsubscribeFromSource()
    {
        sourceConnections.Clear();
    }

    template<class T>
    void ObjectBatch<T>::OnCreated(Reference reference)
    {
        onCreated(reference);
    }

    template<class T>
    void ObjectBatch<T>::OnBeforeDestroyed(Reference reference)
    {
        onBeforeDestroyed(reference);
    }

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ObjectBatch<T>)
    {
        scribe.UnowningPointer(source);
        if(scribe.IsInput())
            IncrementSource();
    }
}