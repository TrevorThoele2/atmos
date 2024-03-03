#pragma once

#include <memory>
#include <Arca/Relic.h>

namespace Atmos
{
    template<class T>
    class UniqueProviderRelic
    {
    public:
        using Value = T;
        using ValuePtr = std::unique_ptr<Value>;
    public:
        UniqueProviderRelic() = default;
        explicit UniqueProviderRelic(ValuePtr&& initialValue);

        Value& operator*() const;
        Value* operator->() const;

        void Change(ValuePtr&& ptr);
        [[nodiscard]] Value* Get() const;
    protected:
        ValuePtr value;
    };

    template<class T>
    UniqueProviderRelic<T>::UniqueProviderRelic(ValuePtr&& initialValue) :
        value(std::move(initialValue))
    {}

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
        static inline const TypeName typeName = "Atmos::UniqueProvider<" + Traits<T>::typeName + ">";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class T, class Archive>
    struct ScribeTraits<Atmos::UniqueProviderRelic<T>, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::UniqueProviderRelic<T>>;
    };
}