#pragma once

#include <memory>

#include "ObjectSystem.h"

#include "Event.h"

#include <Inscription/Memory.h>

namespace Atmos
{
    template<class T>
    class UniqueProviderSystem : public ObjectSystem
    {
    public:
        typedef T Value;
        typedef std::unique_ptr<Value> ValuePtr;
    public:
        Event<T*> onChanged;
    public:
        virtual ~UniqueProviderSystem() = 0;

        Value& operator*() const;
        Value* operator->() const;

        void Set(ValuePtr&& set);
        Value* Get() const;
    protected:
        UniqueProviderSystem(ObjectManager& manager);
        UniqueProviderSystem(const ::Inscription::Table<UniqueProviderSystem>& table);
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
    typename UniqueProviderSystem<T>::Value* UniqueProviderSystem<T>::Get() const
    {
        return value.get();
    }

    template<class T>
    UniqueProviderSystem<T>::UniqueProviderSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    template<class T>
    UniqueProviderSystem<T>::UniqueProviderSystem(const ::Inscription::Table<UniqueProviderSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem), INSCRIPTION_TABLE_GET_MEM(value)
    {}
}

namespace Inscription
{
    template<class T>
    class Inscripter<::Atmos::UniqueProviderSystem<T>> : public InscripterBase<::Atmos::UniqueProviderSystem<T>>
    {
    public:
        INSCRIPTION_INSCRIPTER_BASE_TYPEDEFS(::Atmos::UniqueProviderSystem<T>);

        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
    };

    template<class T>
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::UniqueProviderSystem<T>)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);
        INSCRIPTION_TABLE_ADD(value);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }
}