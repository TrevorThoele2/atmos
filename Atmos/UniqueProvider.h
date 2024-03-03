#pragma once

#include <memory>

namespace Atmos
{
    template<class T>
    class UniqueProvider
    {
    public:
        using Value = T;
        using ValuePtr = std::unique_ptr<Value>;
    public:
        virtual ~UniqueProvider() = 0;
    public:
        Value& operator*() const;
        Value* operator->() const;

        void Change(ValuePtr&& ptr);
        Value* Get() const;
    protected:
        ValuePtr value;
        virtual void OnChange() {}
    };

    template <class T>
    UniqueProvider<T>::~UniqueProvider() = default;

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
        OnChange();
    }

    template<class T>
    auto UniqueProvider<T>::Get() const -> Value*
    {
        return value.get();
    }
}