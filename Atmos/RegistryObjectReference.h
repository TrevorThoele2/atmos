#pragma once

#include "Serialization.h"

namespace Atmos
{
    template<class T>
    class Registry;

    // Works like a pointer
    template<class T>
    class RegistryObjectReference
    {
    public:
        typedef T ObjectT;
        typedef Registry<typename std::remove_const<ObjectT>::type> RegistryT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        const ObjectT *ptr;
    public:
        RegistryObjectReference();
        RegistryObjectReference(const ObjectT &obj);
        RegistryObjectReference(const ObjectT *obj);

        RegistryObjectReference(const RegistryObjectReference &arg) = default;
        RegistryObjectReference& operator=(const RegistryObjectReference &arg) = default;

        bool operator==(const RegistryObjectReference &arg) const;
        bool operator!=(const RegistryObjectReference &arg) const;

        RegistryObjectReference& operator=(const ObjectT &set);
        RegistryObjectReference& operator=(const ObjectT *set);
        void Set();
        void Set(const ObjectT &set);
        void Set(const ObjectT *set);
        void Set(const RegistryObjectReference &set);

        const ObjectT* operator->() const;
        const ObjectT& operator*() const;
        const ObjectT* Get() const;
        explicit operator const ObjectT*() const;

        bool IsValid() const;
        explicit operator bool() const;
    };

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(RegistryObjectReference<T>)
    {
        if (scribe.IsOutput())
        {
            bool valid = ptr != nullptr;
            scribe.Save(valid);
            if (valid)
                scribe.Save(ptr->GetID());
        }
        else // INPUT
        {
            bool valid = false;
            scribe.Load(valid);
            if (valid)
            {
                typename ObjectT::ID id;
                scribe.Load(id);
                *this = RegistryT::Find(id);
            }
        }
    }

    template<class T>
    RegistryObjectReference<T>::RegistryObjectReference() : ptr(nullptr)
    {}

    template<class T>
    RegistryObjectReference<T>::RegistryObjectReference(const ObjectT &obj) : ptr(&obj)
    {}

    template<class T>
    RegistryObjectReference<T>::RegistryObjectReference(const ObjectT *obj) : ptr(obj)
    {}

    template<class T>
    bool RegistryObjectReference<T>::operator==(const RegistryObjectReference &arg) const
    {
        return ptr == arg.ptr;
    }

    template<class T>
    bool RegistryObjectReference<T>::operator!=(const RegistryObjectReference &arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    RegistryObjectReference<T>& RegistryObjectReference<T>::operator=(const ObjectT &set)
    {
        ptr = &set;
        return *this;
    }

    template<class T>
    RegistryObjectReference<T>& RegistryObjectReference<T>::operator=(const ObjectT *set)
    {
        ptr = set;
        return *this;
    }

    template<class T>
    void RegistryObjectReference<T>::Set()
    {
        ptr = nullptr;
    }

    template<class T>
    void RegistryObjectReference<T>::Set(const ObjectT &set)
    {
        ptr = &set;
    }

    template<class T>
    void RegistryObjectReference<T>::Set(const ObjectT *set)
    {
        ptr = set;
    }

    template<class T>
    void RegistryObjectReference<T>::Set(const RegistryObjectReference &set)
    {
        ptr = set.ptr;
    }

    template<class T>
    typename const RegistryObjectReference<T>::ObjectT* RegistryObjectReference<T>::operator->() const
    {
        return ptr;
    }

    template<class T>
    typename const RegistryObjectReference<T>::ObjectT& RegistryObjectReference<T>::operator*() const
    {
        return *ptr;
    }

    template<class T>
    typename const RegistryObjectReference<T>::ObjectT* RegistryObjectReference<T>::Get() const
    {
        return ptr;
    }

    template<class T>
    RegistryObjectReference<T>::operator const ObjectT*() const
    {
        return ptr;
    }

    template<class T>
    bool RegistryObjectReference<T>::IsValid() const
    {
        return ptr != nullptr;
    }

    template<class T>
    RegistryObjectReference<T>::operator bool() const
    {
        return ptr != nullptr;
    }
}

namespace std
{
    template<class T>
    struct hash<::Atmos::RegistryObjectReference<T>>
    {
        typedef ::Atmos::RegistryObjectReference<T> argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type &arg) const
        {
            const result_type first(std::hash<typename const argument_type::ObjectT*>()(arg.Get()));
            return first;
        }
    };
}