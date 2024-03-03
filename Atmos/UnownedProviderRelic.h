#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

namespace Atmos
{
    template<class T>
    class UnownedProviderRelic : public Arca::ClosedTypedRelicAutomation<UnownedProviderRelic<T>>
    {
    public:
        using Value = T;
    public:
        UnownedProviderRelic() = default;

        Value& operator*() const;
        Value* operator->() const;

        void Change(Value* ptr);
        [[nodiscard]] Value* Get() const;
    protected:
        Value* value = nullptr;
    };

    template<class T>
    auto UnownedProviderRelic<T>::operator*() const -> Value&
    {
        return *Get();
    }

    template<class T>
    auto UnownedProviderRelic<T>::operator->() const -> Value*
    {
        return Get();
    }

    template<class T>
    void UnownedProviderRelic<T>::Change(Value* ptr)
    {
        value = ptr;
    }

    template<class T>
    auto UnownedProviderRelic<T>::Get() const -> Value*
    {
        return value;
    }
}

namespace Arca
{
    template<class T>
    struct Traits<Atmos::UnownedProviderRelic<T>>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = Traits<T>::typeName + "Provider";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class T>
    class Scribe<::Atmos::UnownedProviderRelic<T>, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::UnownedProviderRelic<T>, BinaryArchive>
    {};
}