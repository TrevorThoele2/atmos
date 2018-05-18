
#pragma once

#include <functional>

namespace Atmos
{
    template<class... FuncArgs>
    class FunctionBase
    {
    public:
        virtual ~FunctionBase() = 0 {}
        virtual FunctionBase* Clone() const = 0;
        virtual void Execute(FuncArgs && ... args) const = 0;
    };

    template<class Ret, class Obj, class... Args>
    class Functor : public FunctionBase<Args...>
    {
    public:
        typedef Ret(Obj::*Func)(Args...);
    private:
        Func func;
        Obj &obj;
    public:
        Functor(Func func, Obj &obj);
        Functor* Clone() const override;
        void Execute(Args && ... args) const override;
    };

    template<class Ret, class Obj, class... Args>
    Functor<Ret, Obj, Args...>::Functor(Func func, Obj &obj) : func(func), obj(obj)
    {}

    template<class Ret, class Obj, class... Args>
    Functor<Ret, Obj, Args...>* Functor<Ret, Obj, Args...>::Clone() const
    {
        return new Functor(*this);
    }

    template<class Ret, class Obj, class... Args>
    void Functor<Ret, Obj, Args...>::Execute(Args && ... args) const
    {
        (obj.*func)(std::forward<Args>(args)...);
    }
    // Functor

    template<class Ret, class Obj, class... Args>
    Functor<Ret, Obj, Args...>* MakeFunctor(Ret(Obj::*func)(Args...), Obj &obj)
    {
        return new Functor<Ret, Obj, Args...>(func, obj);
    }

    template<class Ret, class... Args>
    class Function : public FunctionBase<Args...>
    {
    public:
        typedef Ret(*Func)(Args...);
    private:
        Func func;
    public:
        Function(Func func);
        Function* Clone() const override;
        void Execute(Args && ... args) const override;
    };

    template<class Ret, class... Args>
    Function<Ret, Args...>::Function(Func func) : func(func)
    {}

    template<class Ret, class... Args>
    Function<Ret, Args...>* Function<Ret, Args...>::Clone() const
    {
        return new Function(*this);
    }

    template<class Ret, class... Args>
    void Function<Ret, Args...>::Execute(Args && ... args) const
    {
        (func)(args...);
    }
    // Function

    template<class Ret, class... Args>
    Function<Ret, Args...>* MakeFunction(Ret(*func)(Args...))
    {
        return new Function<Ret, Args...>(func);
    }

    template<class Ret>
    class FunctionBoundBase
    {
    public:
        virtual ~FunctionBoundBase() = 0 {}
        virtual FunctionBoundBase* Clone() const = 0;
        virtual void Execute() const = 0;
    };

    template<class Ret, class Obj, class... Args>
    class FunctorBound : public FunctionBoundBase<Ret>
    {
    private:
        std::function<Ret(Args...)> func;
    public:
        FunctorBound(Obj &obj, Ret(Obj::*func)(Args...), Args && ... args);
        FunctorBound* Clone() const override;
        void Execute() const override;
    };

    template<class Ret, class Obj, class... Args>
    FunctorBound<Ret, Obj, Args...>::FunctorBound(Obj &obj, Ret(Obj::*func)(Args...), Args && ... args) : func(std::bind(&obj, args...))
    {}

    template<class Ret, class Obj, class... Args>
    FunctorBound<Ret, Obj, Args...>* FunctorBound<Ret, Obj, Args...>::Clone() const
    {
        return new FunctorBound(*this);
    }

    template<class Ret, class Obj, class... Args>
    void FunctorBound<Ret, Obj, Args...>::Execute() const
    {
        func();
    }
    // FunctorBound

    template<class Ret, class Obj, class... Args>
    FunctorBound<Ret, Obj, Args...>* MakeFunctorBound(Obj &obj, Ret(Obj::*func)(Args...), Args && ... args)
    {
        return new FunctorBound<Ret, Obj, Args...>(obj, func, std::forward<Args>(args)...);
    }

    template<class Ret, class... Args>
    class FunctionBound : public FunctionBoundBase<Ret>
    {
    private:
        std::function<Ret(Args...)> func;
    public:
        FunctionBound(Ret(*func)(Args...), Args && ... args);
        FunctionBound* Clone() const override;
        void Execute() const override;
    };

    template<class Ret, class... Args>
    FunctionBound<Ret, Args...>::FunctionBound(Ret(*func)(Args...), Args && ... args) : func(std::bind(args...))
    {}

    template<class Ret, class... Args>
    FunctionBound<Ret, Args...>* FunctionBound<Ret, Args...>::Clone() const
    {
        return new FunctionBound(*this);
    }

    template<class Ret, class... Args>
    void FunctionBound<Ret, Args...>::Execute() const
    {
        func();
    }
    // FunctionBound

    template<class Ret, class... Args>
    FunctionBound<Ret, Args...>* MakeFunctionBound(Ret(*func)(Args...), Args && ... args)
    {
        return new FunctionBound<Ret, Args...>(func, std::forward<Args>(args)...);
    }
}