
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
        typedef std::vector<iterator> Vector;
    public:
        typedef std::unique_ptr<FunctionBase> FunctionPtr;
    private:
        Set set;
        Vector vector;
        FunctionPtr ifRemoved;
        FunctionPtr ifPlaced;
        void MakeTopCommon(std::pair<iterator, bool> &arg);
        void DoFunc(const FunctionPtr &func);
    public:
        UniqueStack() = default;
        UniqueStack(FunctionPtr &&ifRemoved, FunctionPtr &&ifPlaced);
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
            DoFunc(ifRemoved);
            vector.push_back(arg.first);
            DoFunc(ifPlaced);
        }
        else
        {
            auto loop = --vector.end();
            while (**loop != *arg.first)
            {
                DoFunc(ifRemoved);
                set.erase(*loop);
                loop = --vector.erase(loop);
            }

            DoFunc(ifPlaced);
        }
    }

    template<class T>
    void UniqueStack<T>::DoFunc(const FunctionPtr &func)
    {
        if (func && !vector.empty())
            func->Execute(***--vector.end());
    }

    template<class T>
    UniqueStack<T>::UniqueStack(FunctionPtr &&ifRemoved, FunctionPtr &&ifPlaced) : ifRemoved(std::move(ifRemoved)), ifPlaced(std::move(ifPlaced))
    {}

    template<class T>
    void UniqueStack<T>::MakeTop(T *push)
    {
        MakeTopCommon(set.emplace(push));
    }

    template<class T>
    void UniqueStack<T>::Pop()
    {
        DoFunc(ifRemoved);
        set.erase(vector.back());
        vector.pop_back();
    }

    template<class T>
    void UniqueStack<T>::Pop(T &pop)
    {
        for (auto loop = vector.begin(); loop != vector.end(); loop++)
        {
            if (**loop == &pop)
            {
                DoFunc(ifRemoved);
                vector.erase(loop);
                set.erase(&pop);
                return;
            }
        }
    }

    template<class T>
    T* UniqueStack<T>::Top()
    {
        if (vector.empty())
            return nullptr;
        else
            return *vector.back();
    }

    template<class T>
    const T* UniqueStack<T>::Top() const
    {
        if (vector.empty())
            return nullptr;
        else
            return *vector.back();
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
        return set.begin();
    }

    template<class T>
    typename UniqueStack<T>::const_iterator UniqueStack<T>::begin() const
    {
        return set.begin();
    }

    template<class T>
    typename UniqueStack<T>::iterator UniqueStack<T>::end()
    {
        return set.end();
    }

    template<class T>
    typename UniqueStack<T>::const_iterator UniqueStack<T>::end() const
    {
        return set.end();
    }

    template<class T>
    typename UniqueStack<T>::SizeT UniqueStack<T>::size() const
    {
        return set.size();
    }

    template<class T>
    bool UniqueStack<T>::empty() const
    {
        return set.empty();
    }
}