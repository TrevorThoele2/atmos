#pragma once

#include <memory>

namespace Atmos
{
    template<class T>
    class UniqueProvider
    {
    public:
        UniqueProvider() = default;

        virtual ~UniqueProvider();

        template<class U, class... Args>
        void Setup(Args&& ... args);

        T& operator*();
        const T& operator*() const;
        T* operator->();
        const T* operator->() const;
    protected:
        using ValuePtr = std::unique_ptr<T>;
        ValuePtr value;
    protected:
        virtual void OnSetup();
    };

    template<class T>
    UniqueProvider<T>::~UniqueProvider() = default;

    template<class T>
    template<class U, class... Args>
    void UniqueProvider<T>::Setup(Args&& ... args)
    {
        value = std::make_unique<U>(std::forward<Args>(args)...);
        OnSetup();
    }

    template<class T>
    T& UniqueProvider<T>::operator*()
    {
        return *value;
    }

    template<class T>
    const T& UniqueProvider<T>::operator*() const
    {
        return *value;
    }

    template<class T>
    T* UniqueProvider<T>::operator->()
    {
        return value.get();
    }

    template<class T>
    const T* UniqueProvider<T>::operator->() const
    {
        return value.get();
    }

    template<class T>
    void UniqueProvider<T>::OnSetup()
    {}
}