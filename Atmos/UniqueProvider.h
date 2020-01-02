#pragma once

#include <memory>
#include <Arca/ClosedTypedRelicAutomation.h>

namespace Atmos
{
    template<class T>
    class UniqueProvider : public Arca::ClosedTypedRelicAutomation<UniqueProvider<T>>
    {
    public:
        using Value = T;
        using ValuePtr = std::unique_ptr<Value>;
    public:
        UniqueProvider() = default;

        Value& operator*() const;
        Value* operator->() const;

        void Change(ValuePtr&& ptr);
        [[nodiscard]] Value* Get() const;
    protected:
        ValuePtr value;
    };

    template<class T>
    auto UniqueProvider<T>::operator*() const -> Value&
    {
        return *Get();
    }

    template<class T>
    auto UniqueProvider<T>::operator->() const -> Value*
    {
        return Get();
    }

    template<class T>
    void UniqueProvider<T>::Change(ValuePtr&& ptr)
    {
        value = std::move(ptr);
    }

    template<class T>
    auto UniqueProvider<T>::Get() const -> Value*
    {
        return value.get();
    }
}

namespace Arca
{
    template<class T>
    struct Traits<Atmos::UniqueProvider<T>>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static const Locality locality = Locality::Global;
    };

    template<class T>
    const TypeName Traits<Atmos::UniqueProvider<T>>::typeName = Traits<T>::typeName + "Provider";
}

namespace Inscription
{
    template<class T>
    class Scribe<::Atmos::UniqueProvider<T>, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::UniqueProvider<T>, BinaryArchive>
    {};
}