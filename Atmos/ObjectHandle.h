
#pragma once

#include <memory>
#include "Function.h"

namespace Atmos
{
    template<class Manager, class Object>
    class ObjectHandle
    {
    private:
        friend Manager;
        friend Object;
    private:
        typedef FunctionBase<Manager*, Manager*> OnChangeFunc;
    private:
        Manager *wrapped;
        std::unique_ptr<OnChangeFunc> onChange;

        ObjectHandle(Manager *handler = nullptr, OnChangeFunc *func = nullptr);
        ObjectHandle(const ObjectHandle &arg, OnChangeFunc *func = nullptr);
        ObjectHandle(ObjectHandle &&arg);
        ObjectHandle& operator=(const ObjectHandle &arg);
        ObjectHandle& operator=(ObjectHandle &&arg);

        ObjectHandle& operator=(Manager *arg);
    public:
        operator Manager*();
        operator Manager*() const;
        Manager* operator->();
        const Manager* operator->() const;
        Manager* Get();
        const Manager* Get() const;
    };

    template<class Manager, class Object>
    ObjectHandle<Manager, Object>::ObjectHandle(Manager *handler, OnChangeFunc *func) : wrapped(handler), onChange(func)
    {}

    template<class Manager, class Object>
    ObjectHandle<Manager, Object>::ObjectHandle(const ObjectHandle &arg, OnChangeFunc *func) : wrapped(arg.wrapped), onChange(func)
    {}

    template<class Manager, class Object>
    ObjectHandle<Manager, Object>::ObjectHandle(ObjectHandle &&arg) : wrapped(arg.wrapped), onChange(std::move(arg.onChange))
    {
        arg.wrapped = nullptr;
    }

    template<class Manager, class Object>
    ObjectHandle<Manager, Object>& ObjectHandle<Manager, Object>::operator=(const ObjectHandle &arg)
    {
        wrapped = arg.wrapped;
        return *this;
    }

    template<class Manager, class Object>
    ObjectHandle<Manager, Object>& ObjectHandle<Manager, Object>::operator=(ObjectHandle &&arg)
    {
        wrapped = std::move(arg.wrapped);
        arg.wrapped = nullptr;
        onChange = std::move(arg.onChange);
        return *this;
    }

    template<class Manager, class Object>
    ObjectHandle<Manager, Object>& ObjectHandle<Manager, Object>::operator=(Manager *arg)
    {
        Manager *prev = wrapped;
        wrapped = arg;
        if (onChange)
            onChange->Execute(std::move(prev), std::move(wrapped));

        return *this;
    }

    template<class Manager, class Object>
    ObjectHandle<Manager, Object>::operator Manager*()
    {
        return wrapped;
    }

    template<class Manager, class Object>
    ObjectHandle<Manager, Object>::operator Manager*() const
    {
        return wrapped;
    }

    template<class Manager, class Object>
    Manager* ObjectHandle<Manager, Object>::operator->()
    {
        return wrapped;
    }

    template<class Manager, class Object>
    const Manager* ObjectHandle<Manager, Object>::operator->() const
    {
        return wrapped;
    }

    template<class Manager, class Object>
    Manager* ObjectHandle<Manager, Object>::Get()
    {
        return wrapped;
    }

    template<class Manager, class Object>
    const Manager* ObjectHandle<Manager, Object>::Get() const
    {
        return wrapped;
    }
}