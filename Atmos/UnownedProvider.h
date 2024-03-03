#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

namespace Atmos
{
    template<class T>
    class UnownedProvider : public Arca::ClosedTypedRelicAutomation<UnownedProvider<T>>
    {
    public:
        using Value = T;
    public:
        UnownedProvider() = default;

        Value& operator*() const;
        Value* operator->() const;

        void Change(Value* ptr);
        [[nodiscard]] Value* Get() const;
    protected:
        Value* value = nullptr;
    };

    template<class T>
    auto UnownedProvider<T>::operator*() const -> Value&
    {
        return *Get();
    }

    template<class T>
    auto UnownedProvider<T>::operator->() const -> Value*
    {
        return Get();
    }

    template<class T>
    void UnownedProvider<T>::Change(Value* ptr)
    {
        value = ptr;
    }

    template<class T>
    auto UnownedProvider<T>::Get() const -> Value*
    {
        return value;
    }
}

namespace Arca
{
    template<class T>
    struct Traits<Atmos::UnownedProvider<T>>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static const Locality locality = Locality::Global;
    };

    template<class T>
    const TypeName Traits<Atmos::UnownedProvider<T>>::typeName = Traits<T>::typeName + "Provider";
}

namespace Inscription
{
    template<class T>
    class Scribe<::Atmos::UnownedProvider<T>, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::UnownedProvider<T>, BinaryArchive>
    {};
}