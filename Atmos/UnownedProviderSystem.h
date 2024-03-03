#pragma once

#include <memory>

#include "ObjectSystem.h"

#include "Event.h"

namespace Atmos
{
    template<class T>
    class UnownedProviderSystem : public ObjectSystem
    {
    public:
        typedef T Value;
        typedef Value* ValuePtr;
    public:
        Event<T*> onChanged;
    public:
        virtual ~UnownedProviderSystem() = 0;

        Value& operator*() const;
        Value* operator->() const;

        void Set(ValuePtr set);
        Value* Get() const;
    protected:
        UnownedProviderSystem(ObjectManager& manager);
    private:
        ValuePtr value;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    UnownedProviderSystem<T>::~UnownedProviderSystem()
    {}

    template<class T>
    typename UnownedProviderSystem<T>::Value& UnownedProviderSystem<T>::operator*() const
    {
        return *Get();
    }

    template<class T>
    typename UnownedProviderSystem<T>::Value* UnownedProviderSystem<T>::operator->() const
    {
        return Get();
    }

    template<class T>
    void UnownedProviderSystem<T>::Set(ValuePtr set)
    {
        value = set;
        onChanged.Execute(value);
    }

    template<class T>
    typename UnownedProviderSystem<T>::Value* UnownedProviderSystem<T>::Get() const
    {
        return value;
    }

    template<class T>
    UnownedProviderSystem<T>::UnownedProviderSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}
}

namespace Inscription
{
    template<class T>
    class Scribe<::Atmos::UnownedProviderSystem<T>, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::UnownedProviderSystem<T>, BinaryArchive>
    {
    private:
        using BaseT = typename ObjectSystemScribe<::Atmos::UnownedProviderSystem<T>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive)
        {
            archive(object.value);
        }
    };
}