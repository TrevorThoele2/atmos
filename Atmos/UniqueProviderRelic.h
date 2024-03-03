#pragma once

#include <memory>
#include <Arca/ClosedTypedRelicAutomation.h>

namespace Atmos
{
    template<class T>
    class UniqueProviderRelic : public Arca::ClosedTypedRelicAutomation<UniqueProviderRelic<T>>
    {
    public:
        using Value = T;
        using ValuePtr = std::unique_ptr<Value>;
    public:
        UniqueProviderRelic() = default;

        Value& operator*() const;
        Value* operator->() const;

        void Change(ValuePtr&& ptr);
        [[nodiscard]] Value* Get() const;
    protected:
        ValuePtr value;
    };

    template<class T>
    auto UniqueProviderRelic<T>::operator*() const -> Value&
    {
        return *Get();
    }

    template<class T>
    auto UniqueProviderRelic<T>::operator->() const -> Value*
    {
        return Get();
    }

    template<class T>
    void UniqueProviderRelic<T>::Change(ValuePtr&& ptr)
    {
        value = std::move(ptr);
    }

    template<class T>
    auto UniqueProviderRelic<T>::Get() const -> Value*
    {
        return value.get();
    }
}

namespace Arca
{
    template<class T>
    struct Traits<Atmos::UniqueProviderRelic<T>>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = Traits<T>::typeName + "Provider";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class T>
    class Scribe<::Atmos::UniqueProviderRelic<T>, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::UniqueProviderRelic<T>, BinaryArchive>
    {};
}