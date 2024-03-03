#pragma once

#include <memory>

#include "ObjectSystem.h"

#include "Event.h"

#include <Inscription/MemoryScribe.h>

namespace Atmos
{
    template<class T>
    class UniqueProviderSystem : public ObjectSystem
    {
    public:
        using Value = T;
        using ValuePtr = std::unique_ptr<Value>;
    public:
        Event<T*> onChanged;
    public:
        virtual ~UniqueProviderSystem() = 0;

        Value& operator*() const;
        Value* operator->() const;

        void Set(ValuePtr&& set);
        template<class U, class... Args>
        void Create(Args&& ... args);
        Value* Get() const;
    protected:
        UniqueProviderSystem(ObjectManager& manager);
        UniqueProviderSystem(ObjectManager& manager, ValuePtr&& value);
    private:
        ValuePtr value;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    UniqueProviderSystem<T>::~UniqueProviderSystem()
    {}

    template<class T>
    typename UniqueProviderSystem<T>::Value& UniqueProviderSystem<T>::operator*() const
    {
        return *Get();
    }

    template<class T>
    typename UniqueProviderSystem<T>::Value* UniqueProviderSystem<T>::operator->() const
    {
        return Get();
    }

    template<class T>
    void UniqueProviderSystem<T>::Set(ValuePtr&& set)
    {
        value = std::move(set);
        onChanged.Execute(value.get());
    }

    template<class T>
    template<class U, class... Args>
    void UniqueProviderSystem<T>::Create(Args&& ... args)
    {
        static_assert(std::is_base_of_v<T, U>, "When creating, the created type must be derived from the provided type.");

        Set(ValuePtr(new U(std::forward<Args>(args)...)));
    }

    template<class T>
    typename UniqueProviderSystem<T>::Value* UniqueProviderSystem<T>::Get() const
    {
        return value.get();
    }

    template<class T>
    UniqueProviderSystem<T>::UniqueProviderSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    template<class T>
    UniqueProviderSystem<T>::UniqueProviderSystem(ObjectManager& manager, ValuePtr&& value) :
        ObjectSystem(manager), value(std::move(value))
    {}
}

namespace Inscription
{
    template<class T>
    class Scribe<::Atmos::UniqueProviderSystem<T>, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::UniqueProviderSystem<T>, BinaryArchive>
    {
    private:
        using BaseT = typename ObjectSystemScribe<::Atmos::UniqueProviderSystem<T>, BinaryArchive>;
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