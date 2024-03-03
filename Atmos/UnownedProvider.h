#pragma once

namespace Atmos
{
    template<class T>
    class UnownedProvider
    {
    public:
        using Value = T;
    public:
        virtual ~UnownedProvider() = 0;
    public:
        Value& operator*() const;
        Value* operator->() const;

        void Change(Value* ptr);
        Value* Get() const;
    protected:
        Value* value = nullptr;
        virtual void OnChange() {}
    };

    template <class T>
    UnownedProvider<T>::~UnownedProvider() = default;

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
        OnChange();
    }

    template<class T>
    auto UnownedProvider<T>::Get() const -> Value*
    {
        return value;
    }
}