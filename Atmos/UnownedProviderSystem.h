#pragma once

#include <memory>

#include "ObjectSystem.h"

#include "Event.h"

#include <Inscription/Memory.h>

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
        UnownedProviderSystem(const ::Inscription::Table<UnownedProviderSystem>& table);
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

    template<class T>
    UnownedProviderSystem<T>::UnownedProviderSystem(const ::Inscription::Table<UnownedProviderSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem), INSCRIPTION_TABLE_GET_MEM(value)
    {}
}

namespace Inscription
{
    template<class T>
    class Inscripter<::Atmos::UnownedProviderSystem<T>> : public InscripterBase<::Atmos::UnownedProviderSystem<T>>
    {
    public:
        INSCRIPTION_INSCRIPTER_BASE_TYPEDEFS(::Atmos::UnownedProviderSystem<T>);

        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
    };

    template<class T>
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::UnownedProviderSystem<T>)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);
        INSCRIPTION_TABLE_ADD_UNOWNING_POINTER(value);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }
}