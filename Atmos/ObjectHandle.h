
#pragma once

#include <memory>
#include "Function.h"

namespace Atmos
{
    template<class Handler, class Object>
    class ObjectHandle
    {
    private:
        friend Handler;
        friend Object;
    private:
        typedef FunctionBase<Handler*, Handler*> OnChangeFunc;
    private:
        Handler *wrapped;
        std::unique_ptr<OnChangeFunc> onChange;

        ObjectHandle(Handler *handler = nullptr, OnChangeFunc *func = nullptr);
        ObjectHandle(const ObjectHandle &arg, OnChangeFunc *func = nullptr);
        ObjectHandle(ObjectHandle &&arg);
        ObjectHandle& operator=(const ObjectHandle &arg);
        ObjectHandle& operator=(ObjectHandle &&arg);

        ObjectHandle& operator=(Handler *arg);
    public:
        operator Handler*();
        operator Handler*() const;
        Handler* operator->();
        const Handler* operator->() const;
        Handler* Get();
        const Handler* Get() const;
    };

    template<class Handler, class Object>
    ObjectHandle<Handler, Object>::ObjectHandle(Handler *handler, OnChangeFunc *func) : wrapped(handler), onChange(func)
    {}

    template<class Handler, class Object>
    ObjectHandle<Handler, Object>::ObjectHandle(const ObjectHandle &arg, OnChangeFunc *func) : wrapped(arg.wrapped), onChange(func)
    {}

    template<class Handler, class Object>
    ObjectHandle<Handler, Object>::ObjectHandle(ObjectHandle &&arg) : wrapped(arg.wrapped), onChange(std::move(arg.onChange))
    {
        arg.wrapped = nullptr;
    }

    template<class Handler, class Object>
    ObjectHandle<Handler, Object>& ObjectHandle<Handler, Object>::operator=(const ObjectHandle &arg)
    {
        wrapped = arg.wrapped;
        return *this;
    }

    template<class Handler, class Object>
    ObjectHandle<Handler, Object>& ObjectHandle<Handler, Object>::operator=(ObjectHandle &&arg)
    {
        wrapped = std::move(arg.wrapped);
        arg.wrapped = nullptr;
        onChange = std::move(arg.onChange);
        return *this;
    }

    template<class Handler, class Object>
    ObjectHandle<Handler, Object>& ObjectHandle<Handler, Object>::operator=(Handler *arg)
    {
        Handler *prev = wrapped;
        wrapped = arg;
        if (onChange)
            onChange->Execute(std::move(prev), std::move(wrapped));

        return *this;
    }

    template<class Handler, class Object>
    ObjectHandle<Handler, Object>::operator Handler*()
    {
        return wrapped;
    }

    template<class Handler, class Object>
    ObjectHandle<Handler, Object>::operator Handler*() const
    {
        return wrapped;
    }

    template<class Handler, class Object>
    Handler* ObjectHandle<Handler, Object>::operator->()
    {
        return wrapped;
    }

    template<class Handler, class Object>
    const Handler* ObjectHandle<Handler, Object>::operator->() const
    {
        return wrapped;
    }

    template<class Handler, class Object>
    Handler* ObjectHandle<Handler, Object>::Get()
    {
        return wrapped;
    }

    template<class Handler, class Object>
    const Handler* ObjectHandle<Handler, Object>::Get() const
    {
        return wrapped;
    }
}