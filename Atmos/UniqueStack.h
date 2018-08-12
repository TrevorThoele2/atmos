
#pragma once

#include <unordered_set>
#include <vector>
#include <memory>

namespace Atmos
{
    template<class T>
    class UniqueStack
    {
    private:
        class FunctionBase
        {
        public:
            virtual void Execute(T &obj) const = 0;
        };

        template<class Ret>
        class Function : public FunctionBase
        {
        private:
            typedef Ret(T::*Func)();
            Func func;
        public:
            Function(Func func) : func(func) {}
            void Execute(T &obj) const override;
        };
    private:
        typedef std::unordered_set<T*> Set;
    public:
        typedef typename Set::iterator iterator;
        typedef typename Set::const_iterator const_iterator;
        typedef typename Set::size_type SizeT;
    private:
        typedef std::vector<iterator> Order;
    public:
        typedef std::unique_ptr<FunctionBase> FunctionPtr;
    private:
        Set baseSet;
        Order order;
        FunctionPtr whenRemoved;
        FunctionPtr whenMadeTop;

        void MakeTopCommon(std::pair<iterator, bool> &arg);
        void PopCommon(typename Order::iterator popThis);
        void DoFunc(const FunctionPtr &func);
    public:
        UniqueStack() = default;
        UniqueStack(FunctionPtr &&whenRemoved, FunctionPtr &&whenMadeTop);
        void MakeTop(T *push);
        void Pop();
        void Pop(T &pop);
        T* Top();
        const T* Top() const;
        template<class Ret>
        static FunctionBase* MakeFunction(Ret(T::*func)());

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        SizeT size() const;
        bool empty() const;
    };

    template<class T>
    template<class Ret>
    void UniqueStack<T>::Function<Ret>::Execute(T &obj) const
    {
        (obj.*func)();
    }

    template<class T>
    void UniqueStack<T>::MakeTopCommon(std::pair<iterator, bool> &arg)
    {
        if (arg.second)
        {
            DoFunc(whenRemoved);
            order.push_back(arg.first);
            DoFunc(whenMadeTop);
        }
        else
        {
            auto loop = --order.end();
            while (**loop != *arg.first)
            {
                DoFunc(whenRemoved);
                baseSet.erase(*loop);
                loop = --order.erase(loop);
            }

            DoFunc(whenMadeTop);
        }
    }

    template<class T>
    void UniqueStack<T>::PopCommon(typename Order::iterator popThis)
    {
        auto top = Top();

        DoFunc(whenRemoved);
        baseSet.erase(*popThis);
        order.erase(popThis);
        if (top != Top() && !order.empty())
            DoFunc(whenMadeTop);
    }

    template<class T>
    void UniqueStack<T>::DoFunc(const FunctionPtr &func)
    {
        if (func && !order.empty())
            func->Execute(***--order.end());
    }

    template<class T>
    UniqueStack<T>::UniqueStack(FunctionPtr &&whenRemoved, FunctionPtr &&whenMadeTop) : whenRemoved(std::move(whenRemoved)), whenMadeTop(std::move(whenMadeTop))
    {}

    template<class T>
    void UniqueStack<T>::MakeTop(T *push)
    {
        MakeTopCommon(baseSet.emplace(push));
    }

    template<class T>
    void UniqueStack<T>::Pop()
    {
        if(empty())
            return;

        PopCommon(--order.end());
    }

    template<class T>
    void UniqueStack<T>::Pop(T &pop)
    {
        for (auto loop = order.begin(); loop != order.end(); ++loop)
        {
            if (**loop == &pop)
            {
                PopCommon(loop);
                return;
            }
        }
    }

    template<class T>
    T* UniqueStack<T>::Top()
    {
        if (empty())
            return nullptr;

        return *order.back();
    }

    template<class T>
    const T* UniqueStack<T>::Top() const
    {
        if (empty())
            return nullptr;

        return *order.back();
    }

    template<class T>
    template<class Ret>
    typename UniqueStack<T>::FunctionBase* UniqueStack<T>::MakeFunction(Ret(T::*func)())
    {
        return new Function<Ret>(func);
    }

    template<class T>
    typename UniqueStack<T>::iterator UniqueStack<T>::begin()
    {
        return baseSet.begin();
    }

    template<class T>
    typename UniqueStack<T>::const_iterator UniqueStack<T>::begin() const
    {
        return baseSet.begin();
    }

    template<class T>
    typename UniqueStack<T>::iterator UniqueStack<T>::end()
    {
        return baseSet.end();
    }

    template<class T>
    typename UniqueStack<T>::const_iterator UniqueStack<T>::end() const
    {
        return baseSet.end();
    }

    template<class T>
    typename UniqueStack<T>::SizeT UniqueStack<T>::size() const
    {
        return baseSet.size();
    }

    template<class T>
    bool UniqueStack<T>::empty() const
    {
        return baseSet.empty();
    }
}