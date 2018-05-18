#pragma once

#include <type_traits>

#include "Serialization.h"

namespace Atmos
{
    template<class T>
    class AssetRegistry;

    // Works like a pointer
    template<class T>
    class AssetReference
    {
    public:
        typedef T AssetT;
        typedef AssetRegistry<typename std::remove_const<AssetT>::type> RegistryT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        const AssetT *ptr;
    public:
        AssetReference();
        AssetReference(const AssetT &res);
        AssetReference(const AssetT *res);

        AssetReference(const AssetReference &arg) = default;
        AssetReference& operator=(const AssetReference &arg) = default;

        bool operator==(const AssetReference &arg) const;
        bool operator!=(const AssetReference &arg) const;

        AssetReference& operator=(const AssetT &set);
        AssetReference& operator=(const AssetT *set);
        void Set();
        void Set(const AssetT &set);
        void Set(const AssetT *set);
        void Set(const AssetReference &set);

        const AssetT* operator->() const;
        const AssetT& operator*() const;
        const AssetT* Get() const;
        explicit operator const AssetT*() const;

        bool IsValid() const;
        explicit operator bool() const;
    };

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AssetReference<T>)
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
                typename AssetT::ID id;
                scribe.Load(id);
                *this = RegistryT::Find(id);
            }
        }
    }

    template<class T>
    AssetReference<T>::AssetReference() : ptr(nullptr)
    {}

    template<class T>
    AssetReference<T>::AssetReference(const AssetT &res) : ptr(&res)
    {}

    template<class T>
    AssetReference<T>::AssetReference(const AssetT *res) : ptr(res)
    {}

    template<class T>
    bool AssetReference<T>::operator==(const AssetReference &arg) const
    {
        return ptr == arg.ptr;
    }

    template<class T>
    bool AssetReference<T>::operator!=(const AssetReference &arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    AssetReference<T>& AssetReference<T>::operator=(const AssetT &set)
    {
        ptr = &set;
        return *this;
    }

    template<class T>
    AssetReference<T>& AssetReference<T>::operator=(const AssetT *set)
    {
        ptr = set;
        return *this;
    }

    template<class T>
    void AssetReference<T>::Set()
    {
        ptr = nullptr;
    }

    template<class T>
    void AssetReference<T>::Set(const AssetT &set)
    {
        ptr = &set;
    }

    template<class T>
    void AssetReference<T>::Set(const AssetT *set)
    {
        ptr = set;
    }

    template<class T>
    void AssetReference<T>::Set(const AssetReference &set)
    {
        ptr = set.ptr;
    }

    template<class T>
    typename const AssetReference<T>::AssetT* AssetReference<T>::operator->() const
    {
        return ptr;
    }

    template<class T>
    typename const AssetReference<T>::AssetT& AssetReference<T>::operator*() const
    {
        return *ptr;
    }

    template<class T>
    typename const AssetReference<T>::AssetT* AssetReference<T>::Get() const
    {
        return ptr;
    }

    template<class T>
    AssetReference<T>::operator const AssetT*() const
    {
        return ptr;
    }

    template<class T>
    bool AssetReference<T>::IsValid() const
    {
        return ptr != nullptr;
    }

    template<class T>
    AssetReference<T>::operator bool() const
    {
        return IsValid();
    }
}

namespace std
{
    template<class T>
    struct hash<::Atmos::AssetReference<T>>
    {
        typedef ::Atmos::AssetReference<T> argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type &arg) const
        {
            const result_type first(std::hash<typename argument_type::AssetT>()(arg.Get()));
            return first;
        }
    };
}